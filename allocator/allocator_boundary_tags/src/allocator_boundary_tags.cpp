#include <not_implemented.h>
#include <mutex>
#include <sstream>

#include "../include/allocator_boundary_tags.h"

// meta_alloc : alloc* -> logger* -> fit_mode -> mutex -> space_size -> block_loaded *

// meta block : alloc* -> size_block -> left_block -> right_block


allocator_boundary_tags::~allocator_boundary_tags()
{
	debug_with_guard(get_typename() + " destructor called");
	get_mutex().~mutex(); //destroy mutex
	deallocate_with_guard(_trusted_memory);
	left_bytes = 0;
}


allocator_boundary_tags::allocator_boundary_tags(
    allocator_boundary_tags &&other) noexcept : _trusted_memory(other._trusted_memory), left_bytes(other.left_bytes)
{
	debug_with_guard("Start move constructor of " + get_typename());
	other._trusted_memory = nullptr;
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
    allocator_boundary_tags &&other) noexcept
{
	debug_with_guard("Start assignment operator of " + get_typename());
	if (this != &other)
	{
		get_mutex().~mutex();
		deallocate_with_guard(_trusted_memory);

		_trusted_memory = other._trusted_memory;
		left_bytes = other.left_bytes;
		other._trusted_memory = nullptr;
	}

	return *this;
}

allocator_boundary_tags::allocator_boundary_tags(
    size_t space_size,
    allocator *parent_allocator,
    logger *logger,
    allocator_with_fit_mode::fit_mode allocate_fit_mode)
{

	if (space_size < get_block_size_of_meta())
	{
		error_with_guard(get_typename() + " construct with param error: " + std::to_string(space_size) + " bytes. Requested size is less than the minimum possible size " + std::to_string(get_block_size_of_meta()));
		throw std::logic_error("Impossible to initialize allocator");
	}

	size_t allocator_size = space_size + get_allocator_size_of_meta();

	if(parent_allocator == nullptr)
	{
		try
		{
			_trusted_memory = ::operator new(allocator_size);
		}
		catch (std::bad_alloc const& )
		{
			error_with_guard(get_typename() + " failed to allocate " + std::to_string(allocator_size) + " bytes of memory");
			throw;
		}
	}
	else
	{
		_trusted_memory = parent_allocator->allocate(allocator_size, 1);
	}

	auto *ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

	*reinterpret_cast<allocator**>(ptr) = parent_allocator;
	ptr += sizeof(allocator *);

	*reinterpret_cast<class logger**>(ptr) = logger;
	ptr += sizeof(class logger *);

	*reinterpret_cast<fit_mode*>(ptr) = allocate_fit_mode;
	ptr += sizeof(fit_mode);

	new(reinterpret_cast<std::mutex *>(ptr)) std::mutex();
	ptr += sizeof(std::mutex);

	*reinterpret_cast<block_size_t *>(ptr) = space_size;
	ptr += sizeof(block_size_t);
	left_bytes = space_size;

	*reinterpret_cast<block_pointer_t *>(ptr) = nullptr;

	debug_with_guard(get_typename() + " constructor with parameters completed successfully");
}

[[nodiscard]] void *allocator_boundary_tags::allocate(
    size_t value_size,
    size_t values_count)
{
	debug_with_guard(
			get_typename() + " allocate with value_size = " + std::to_string(value_size) + "; values_count = " +
			std::to_string(values_count) + "; Now it is having " + std::to_string(left_bytes) + " bytes");

	std::lock_guard<std::mutex> lock(get_mutex());

	size_t need_result_mem = value_size * values_count;
	size_t block_size = need_result_mem + get_block_size_of_meta();

	block_pointer_t before_loaded_block;
	block_pointer_t cur_loaded_block;

	switch(get_fit_mode())
	{
		case allocator_with_fit_mode::fit_mode::the_best_fit:
			before_loaded_block = get_suitable(block_size, true);
			break;
		case allocator_with_fit_mode::fit_mode::first_fit:
			before_loaded_block = get_first_suitable(block_size);
			break;
		case allocator_with_fit_mode::fit_mode::the_worst_fit:
			before_loaded_block = get_suitable(block_size, false);
			break;
	}

	if (before_loaded_block == nullptr)
	{
		error_with_guard(get_typename() + "No suitable block found for " + std::to_string(block_size) + " bytes");
		throw std::bad_alloc();
	}

	if (before_loaded_block == _trusted_memory)
	{
		cur_loaded_block = reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_size_of_meta();
	}
	else {
		cur_loaded_block = reinterpret_cast<unsigned char *>(before_loaded_block) + get_block_size_of_meta() + get_size_block(before_loaded_block);
	}

	get_allocator_block(cur_loaded_block) = this;
	get_size_block(cur_loaded_block) = need_result_mem;
	get_prev_loaded_block(cur_loaded_block) = before_loaded_block;
	get_next_loaded_block(cur_loaded_block) = before_loaded_block == _trusted_memory ? get_first_block_by_alloc() : get_next_loaded_block(before_loaded_block);

	if (before_loaded_block == _trusted_memory)
	{
		get_first_block_by_alloc() = cur_loaded_block;
	}
	else get_next_loaded_block(before_loaded_block) = cur_loaded_block;

	if (get_next_loaded_block(cur_loaded_block) != nullptr)
		get_prev_loaded_block(get_next_loaded_block(cur_loaded_block)) = cur_loaded_block;

	auto difference = reinterpret_cast<unsigned char*>(get_next_loaded_block(cur_loaded_block))
			- (reinterpret_cast<unsigned char*>(cur_loaded_block) + get_size_block(cur_loaded_block) + get_block_size_of_meta());
	if (difference < get_block_size_of_meta())
	{
		get_size_block(cur_loaded_block) = need_result_mem + difference;
		left_bytes -= difference;
	}

	left_bytes -= block_size;
	debug_with_guard("Allocation completed. Allocated memory size: " + std::to_string(need_result_mem) + " bytes. " + std::to_string(left_bytes) + " bytes left");
	information_with_guard(get_typename() + "current state of blocks: " + get_blocks_info_to_string(get_blocks_info()));

	return reinterpret_cast<unsigned char *>(cur_loaded_block) + get_block_size_of_meta();
}

void allocator_boundary_tags::deallocate(
    void *at)
{
	std::lock_guard<std::mutex> lock(get_mutex());
	debug_with_guard("Start deallocate " + get_typename());

	block_pointer_t block_ptr = reinterpret_cast<unsigned char *>(at) - get_block_size_of_meta();
	block_pointer_t prev = get_prev_loaded_block(block_ptr);
	block_pointer_t next = get_next_loaded_block(block_ptr);
	if (get_allocator_block(block_ptr) != this)
	{
		error_with_guard("Invalid memory block");
		throw std::logic_error("Logic error: The received memory block does not belong to this allocator");
	}

	debug_with_guard("This block before deallocate " + get_dump(reinterpret_cast<char*>(at), get_size_block(block_ptr)));

	if (prev == _trusted_memory) get_first_block_by_alloc() = next;
	else get_next_loaded_block(get_prev_loaded_block(block_ptr)) = next;

	if (next != nullptr) get_prev_loaded_block(next) = prev;

	left_bytes += get_block_size_of_meta() + get_size_block(block_ptr);

	debug_with_guard("Deallocation completed. Deallocated memory size: " + std::to_string(get_block_size_of_meta() + get_size_block(block_ptr)) + " bytes. Current available memory : " + std::to_string(left_bytes) + " bytes");
	information_with_guard(get_typename() + " current state of blocks: " + get_blocks_info_to_string(get_blocks_info()));
}

inline void allocator_boundary_tags::set_fit_mode(allocator_with_fit_mode::fit_mode mode) { get_fit_mode() = mode; }

inline allocator *allocator_boundary_tags::get_allocator() const
{
	return *reinterpret_cast<allocator**>(_trusted_memory);
}

inline logger *allocator_boundary_tags::get_logger() const
{
	auto* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(allocator*);

	return *reinterpret_cast<logger**>(ptr);
}

allocator_with_fit_mode::fit_mode& allocator_boundary_tags::get_fit_mode() const noexcept
{
	auto* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(allocator*) + sizeof(logger*);

	return *reinterpret_cast<fit_mode*>(ptr);
}

inline std::mutex &  allocator_boundary_tags::get_mutex() const noexcept
{
	auto* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(fit_mode);

	return *reinterpret_cast<std::mutex*>(ptr);
}

inline size_t &allocator_boundary_tags::get_allocator_size() const noexcept
{
	auto* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(fit_mode) + sizeof(std::mutex);

	return *reinterpret_cast<size_t*>(ptr);
}

inline void* & allocator_boundary_tags::get_first_block_by_alloc() const noexcept
{
	auto* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(allocator*) + sizeof(logger*) + sizeof(fit_mode) + sizeof(std::mutex) + sizeof(block_size_t);

	return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline allocator::block_size_t allocator_boundary_tags::get_block_size_of_meta() const noexcept
{
	return 2 * sizeof(block_pointer_t) + sizeof(block_size_t) + sizeof(allocator*);
}

inline allocator::block_size_t allocator_boundary_tags::get_allocator_size_of_meta() const noexcept
{
	return sizeof(allocator *) + sizeof(logger *) + sizeof(fit_mode) + sizeof(std::mutex) + sizeof(block_size_t) +
		   sizeof(block_pointer_t);
}

inline allocator *&allocator_boundary_tags::get_allocator_block(block_pointer_t cur_block) const noexcept
{
	return *reinterpret_cast<allocator**>(cur_block);
}

inline allocator::block_size_t &allocator_boundary_tags::get_size_block(block_pointer_t cur_block) const noexcept
{
	auto *ptr = reinterpret_cast<unsigned char*>(cur_block);
	ptr += sizeof(allocator *);

	return *reinterpret_cast<block_size_t*>(ptr);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_prev_loaded_block(block_pointer_t cur_block) const noexcept
{
	auto *ptr = reinterpret_cast<unsigned char*>(cur_block);
	ptr += sizeof(allocator *) + sizeof(block_size_t);

	return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline allocator::block_pointer_t &allocator_boundary_tags::get_next_loaded_block(block_pointer_t cur_block) const noexcept
{
	auto *ptr = reinterpret_cast<unsigned char*>(cur_block);
	ptr += sizeof(allocator *) + sizeof(block_size_t) + sizeof(block_pointer_t);

	return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
	return "allocator_boundary_tags";
}

std::string allocator_boundary_tags::get_dump(char* at, size_t size)
{
	std::string result;
	for(size_t i = 0; i < size; i++)
		result += std::to_string(static_cast<int>(at[i])) + " ";
	return result;
}

void* allocator_boundary_tags::get_first_suitable(size_t need_size) const noexcept
{
	block_pointer_t first_block = get_first_block_by_alloc();

	if (first_block == nullptr)
	{
		size_t available_space = get_allocator_size();
		if (available_space >= need_size)
			return _trusted_memory;
		else
			return nullptr;
	}

	block_pointer_t current_block = first_block, previous_block = _trusted_memory, next_block = get_next_loaded_block(current_block);
	block_size_t available_space;

	while (current_block != nullptr)
	{
		next_block = get_next_loaded_block(current_block);
		if (previous_block == _trusted_memory)
		{
			available_space = reinterpret_cast<unsigned char *>(current_block) - (reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_size_of_meta()) ;
		}
		else
		{
			available_space = reinterpret_cast<unsigned char *>(current_block) - (reinterpret_cast<unsigned char *>(previous_block) + get_size_block(previous_block) + get_block_size_of_meta());
		}

		if (available_space >= need_size) return previous_block != _trusted_memory ? previous_block : _trusted_memory;

		previous_block = current_block;
		current_block = next_block;
	}

	available_space = (reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_size() + get_allocator_size_of_meta()) -
			(reinterpret_cast<unsigned char *>(previous_block) + get_size_block(previous_block) + get_block_size_of_meta());
	if (available_space >= need_size) {
		return previous_block;
	}
	return nullptr;
}

void* allocator_boundary_tags::get_suitable(size_t need_size, bool is_best_fit) const noexcept
{
	block_pointer_t first_block = get_first_block_by_alloc();

	if (first_block == nullptr)
	{
		size_t available_space = get_allocator_size();
		if (available_space >= need_size)
			return _trusted_memory;
		else
			return nullptr;
	}

	block_pointer_t current_block = first_block, previous_block = _trusted_memory, next_block;
	block_pointer_t result;
	block_size_t available_space;
	block_size_t best_fit_size = get_allocator_size(), worst_fit_size = 0;

	while (current_block != nullptr)
	{
		next_block = get_next_loaded_block(current_block);
		if (previous_block == _trusted_memory)
			available_space = reinterpret_cast<unsigned char*>(current_block) - (reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size_of_meta());
		else
			available_space = reinterpret_cast<unsigned char*>(current_block) - (reinterpret_cast<unsigned char*>(previous_block) + get_size_block(previous_block) + get_block_size_of_meta());

		if (available_space >= need_size)
		{
			if (is_best_fit && available_space < best_fit_size)
			{
				best_fit_size = available_space;
				result = previous_block != _trusted_memory ? previous_block : _trusted_memory;
			}
			else if (!is_best_fit && available_space > worst_fit_size)
			{
				worst_fit_size = available_space;
				result = previous_block != _trusted_memory ? previous_block : _trusted_memory;
			}
		}
		previous_block = current_block;
		current_block = next_block;
	}

	available_space = (reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size() + get_allocator_size_of_meta()) -
					  (reinterpret_cast<unsigned char*>(previous_block) + get_size_block(previous_block) + get_block_size_of_meta());
	if (available_space >= need_size && ((available_space > worst_fit_size && !is_best_fit) || (available_space < best_fit_size && is_best_fit)))
		return previous_block;
	return result;
}


std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const noexcept
{
	std::vector<allocator_test_utils::block_info> result;
	block_pointer_t head = get_first_block_by_alloc();

	if (head == nullptr)
	{
		result.push_back({get_allocator_size(), false});
		return result;
	}

	block_pointer_t current_block = head, prev_block = _trusted_memory, next_block = get_next_loaded_block(current_block);
	block_size_t available_space;

	while (current_block != nullptr)
	{
		next_block = get_next_loaded_block(current_block);
		if (prev_block == _trusted_memory)
		{
			available_space = reinterpret_cast<unsigned char *>(current_block) - (reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_size_of_meta()) ;
			if (available_space > 0)
				result.push_back({available_space, false});
			result.push_back({get_size_block(current_block), true});
		}
		else
			result.push_back({get_size_block(current_block), true});

		if (next_block == nullptr)
		{
			available_space = (reinterpret_cast<unsigned char *>(_trusted_memory) + get_allocator_size() + get_allocator_size_of_meta()) -
					(reinterpret_cast<unsigned char *>(current_block) + get_size_block(current_block) + get_block_size_of_meta());
			if (available_space > 0)
				result.push_back({available_space, false});
		}
		else
		{
			available_space = reinterpret_cast<unsigned char *>(next_block) -
					(reinterpret_cast<unsigned char *>(current_block) + get_size_block(current_block) + get_block_size_of_meta());
			if (available_space > 0)
				result.push_back({available_space, false});
		}
		prev_block = current_block;
		current_block = next_block;
	}
	return result;
}

std::string allocator_boundary_tags::get_blocks_info_to_string(const std::vector<allocator_test_utils::block_info>& vector) const noexcept
{
	std::ostringstream str;
	for(auto& it : vector)
	{
		if(it.is_block_occupied) str << "<occup>";
		else str << "<avail>";

		str << "<" + std::to_string(it.block_size) + "> | ";
	}
	return str.str();
}
