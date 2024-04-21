#include <not_implemented.h>

#include "../include/allocator_sorted_list.h"

// meta_alloc : alloc* -> logger* -> fit_mode -> mutex -> space_size -> block_loaded *

// meta block : ptr* -> size_block

allocator_sorted_list::~allocator_sorted_list()
{
	debug_with_guard(get_typename() + " destructor called");
	get_mutex().~mutex();
	deallocate_with_guard(_trusted_memory);
	left_bytes = 0;
}

allocator_sorted_list::allocator_sorted_list(
		allocator_sorted_list &&other) noexcept : _trusted_memory(other._trusted_memory), left_bytes(other.left_bytes)
{
	debug_with_guard("Start move constructor of " + get_typename());
	other._trusted_memory = nullptr;
}
allocator_sorted_list &allocator_sorted_list::operator=(
		allocator_sorted_list &&other) noexcept
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

allocator_sorted_list::allocator_sorted_list(
		size_t space_size,
		allocator *parent_allocator,
		logger *logger,
		allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
	size_t allocator_size = space_size + get_allocator_size_of_meta();

	try
	{
		_trusted_memory = parent_allocator == nullptr ? ::operator new(allocator_size) : parent_allocator->allocate(allocator_size, 1);
	}
	catch (std::bad_alloc const & ex)
	{
		error_with_guard(get_typename() + " failed to allocate " + std::to_string(allocator_size) + " bytes of memory");
		throw ex;
	}

	auto ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

	*reinterpret_cast<class logger**>(ptr) = logger;
	ptr += sizeof(class logger*);

	*reinterpret_cast<allocator**>(ptr) = parent_allocator;
	ptr += sizeof(allocator*);

	*reinterpret_cast<fit_mode*>(ptr) = allocate_fit_mode;
	ptr += sizeof(fit_mode);

	*reinterpret_cast<block_size_t*>(ptr) = space_size;
	ptr += sizeof(block_size_t);

	construct(reinterpret_cast<std::mutex*>(ptr));
	ptr += sizeof(std::mutex);

	*reinterpret_cast<void**>(ptr) = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size_of_meta();
	ptr += sizeof(void*);

	*reinterpret_cast<size_t*>(ptr) = space_size - get_block_size_of_meta(); //size_current
	ptr += sizeof(size_t);
	left_bytes = space_size - get_block_size_of_meta();

	*reinterpret_cast<void**>(ptr) = nullptr; // next_block_ptr

	debug_with_guard(get_typename() + " constructor with parameters completed successfully");
}

[[nodiscard]] void *allocator_sorted_list::allocate(
		size_t value_size,
		size_t values_count)
{
	debug_with_guard(get_typename() + " allocate with value_size = " + std::to_string(value_size) + "; values_count = " +
					 std::to_string(values_count) + "; Now it is having " + std::to_string(left_bytes ) + " bytes");

	std::lock_guard<std::mutex> lock(get_mutex());

	size_t need_result_mem = value_size * values_count;
	block_pointer_t before_loaded_block, cur_loaded_block;

	switch (get_fit_mode())
	{
		case allocator_sorted_list::fit_mode::first_fit:
			before_loaded_block = get_first_suitable(need_result_mem);
			break;
		case allocator_sorted_list::fit_mode::the_best_fit:
			before_loaded_block = get_best_suitable(need_result_mem);
			break;
		case allocator_sorted_list::fit_mode::the_worst_fit:
			before_loaded_block = get_worst_suitable(need_result_mem);
			break;
	}

	if (before_loaded_block == nullptr)
	{
		error_with_guard(get_typename() + "No suitable block found for " + std::to_string(need_result_mem) + " bytes");
		throw std::bad_alloc();
	}

	cur_loaded_block = before_loaded_block == _trusted_memory ? get_first_block(_trusted_memory) : get_ptr_to_block_from_block(before_loaded_block);

	block_pointer_t next_block = get_ptr_to_block_from_block(cur_loaded_block);

	block_size_t size_current_block = get_size_block(cur_loaded_block);

	if(size_current_block < need_result_mem + get_block_size_of_meta())
	{
		warning_with_guard(get_typename() + " changed allocating block size to " + std::to_string(size_current_block));
		need_result_mem = size_current_block;
	}

	auto size_new_block = reinterpret_cast<block_size_t *>(cur_loaded_block);
	*size_new_block = need_result_mem;

	*reinterpret_cast<void**>(size_new_block + 1) = _trusted_memory;

	if(size_current_block - need_result_mem >= get_block_size_of_meta())
	{
		auto ptr = reinterpret_cast<unsigned char*>(cur_loaded_block) + get_block_size_of_meta() + get_size_block(cur_loaded_block);
		*reinterpret_cast<block_size_t *>(ptr) = size_current_block - need_result_mem - get_block_size_of_meta();
		ptr += sizeof(block_size_t);
		*reinterpret_cast<void**>(ptr) = next_block;
		next_block = reinterpret_cast<unsigned char*>(cur_loaded_block) + get_block_size_of_meta() + get_size_block(cur_loaded_block);
	}
	if (before_loaded_block == _trusted_memory)
		get_first_block(_trusted_memory) = next_block;
	else
		get_ptr_to_block_from_block(before_loaded_block) = next_block;

	left_bytes -= need_result_mem;
	debug_with_guard("Allocation completed. Allocated memory size: " + std::to_string(need_result_mem) + " bytes. " + std::to_string(left_bytes) + " bytes left");
	information_with_guard(get_typename() + "current state of blocks: " + get_blocks_info_to_string(get_blocks_info()));

	return reinterpret_cast<unsigned char*>(cur_loaded_block) + get_block_size_of_meta();
}

void allocator_sorted_list::deallocate(
		void *at)
{
	std::lock_guard lock(get_mutex());

	debug_with_guard("Start deallocate " + get_typename());

	void* block_ptr = reinterpret_cast<unsigned char*>(at) - get_block_size_of_meta();

	if(get_ptr_to_block_from_block(block_ptr) != _trusted_memory)
	{
		error_with_guard("Invalid memory block");
		throw std::logic_error("Logic error: The received memory block does not belong to this allocator");
	}

	debug_with_guard("This block before deallocate " + get_dump(reinterpret_cast<char*>(at), get_size_block(block_ptr)));

	block_pointer_t previous_free = get_previous_for_loaded(block_ptr);
	void* next_free;

	if(previous_free == _trusted_memory)
		next_free = get_first_block(_trusted_memory);
	else
		next_free = get_ptr_to_block_from_block(previous_free);


	if(next_free != nullptr && reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(block_ptr) + get_block_size_of_meta() + get_size_block(block_ptr)) == next_free)
	{
		auto byte_ptr = reinterpret_cast<unsigned char*>(block_ptr);
		*reinterpret_cast<size_t*>(byte_ptr) = get_size_block(block_ptr) + get_block_size_of_meta() + get_size_block(next_free);
		byte_ptr += sizeof(size_t);
		*reinterpret_cast<void**>(byte_ptr) = get_ptr_to_block_from_block(next_free);
	}
	else
	{
		if (block_ptr == _trusted_memory)
			get_first_block(_trusted_memory) = next_free;
		else
			get_ptr_to_block_from_block(block_ptr) = next_free;
	}

	if (previous_free == _trusted_memory)
		get_first_block(_trusted_memory) = block_ptr;
	else
		get_ptr_to_block_from_block(previous_free) = block_ptr;

	if(previous_free != _trusted_memory && reinterpret_cast<unsigned char*>(previous_free) + get_block_size_of_meta() + get_size_block(previous_free) == block_ptr)
	{
		*reinterpret_cast<block_size_t *>(previous_free) = get_size_block(previous_free) + get_block_size_of_meta() + get_size_block(block_ptr);
		auto byte_ptr = reinterpret_cast<unsigned char*>(previous_free) + sizeof(block_size_t);
		*reinterpret_cast<block_pointer_t*>(byte_ptr) = get_ptr_to_block_from_block(block_ptr);
	}

	left_bytes += get_size_block(block_ptr);

	debug_with_guard("Deallocation completed. Deallocated memory size: " + std::to_string(get_block_size_of_meta() + get_size_block(block_ptr)) + " bytes. Current available memory : " + std::to_string(left_bytes) + " bytes");
	information_with_guard(get_typename() + " current state of blocks: " + get_blocks_info_to_string(get_blocks_info()));
}

inline void allocator_sorted_list::set_fit_mode(
		allocator_with_fit_mode::fit_mode mode)
{
	std::lock_guard lock(get_mutex());
	auto ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(logger*) + sizeof(allocator*);
	*reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr) = mode;
}

inline allocator *allocator_sorted_list::get_allocator() const
{
	auto ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(logger*);
	return *reinterpret_cast<allocator**>(ptr);
}

std::vector<allocator_test_utils::block_info> allocator_sorted_list::get_blocks_info() const noexcept
{
	std::vector<allocator_test_utils::block_info> result;
	auto block = get_first_block(_trusted_memory);

	block_pointer_t previous_block = _trusted_memory;

	if(block == nullptr)
	{
		auto start = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size_of_meta();
		auto end_ptr = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size_of_meta() + get_size_full();

		while(start != end_ptr)
		{
			result.push_back({get_size_block(start), true});
			start += get_block_size_of_meta() + get_size_block(start);
		}
	}

	while (block != nullptr)
	{
		size_t current_meta = previous_block == _trusted_memory ? get_allocator_size_of_meta() : get_block_size_of_meta() + get_size_block(previous_block);
		auto ptr = reinterpret_cast<unsigned char*>(previous_block) + current_meta;

		while(ptr != block)
		{
			result.push_back({get_size_block(ptr), true});
			ptr += get_block_size_of_meta() + get_size_block(ptr);
		}
		result.push_back({get_size_block(block), false});

		auto check_end = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(block_size_t ));
		if(check_end == nullptr)
		{
			auto ptr2 = reinterpret_cast<unsigned char*>(block) + get_block_size_of_meta() + get_size_block(block);
			auto end_allocator = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_size_of_meta() + get_size_full();

			while(ptr2 != end_allocator)
			{
				result.push_back({get_size_block(ptr2), true});
				ptr2 += get_block_size_of_meta() + get_size_block(ptr2);
			}
		}
		previous_block = block;
		block = get_ptr_to_block_from_block(block);
	}
	return result;
}

inline allocator::block_size_t allocator_sorted_list::get_allocator_size_of_meta() const noexcept
{
	return sizeof(allocator *) + sizeof(logger *) + sizeof(fit_mode) + sizeof(std::mutex) + sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t allocator_sorted_list::get_block_size_of_meta() const noexcept
{
	return sizeof(block_pointer_t) + sizeof(block_size_t);
}

inline logger *allocator_sorted_list::get_logger() const
{
	return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_sorted_list::get_typename() const noexcept
{
	return "allocator_sorted_list";
}

inline std::mutex& allocator_sorted_list::get_mutex() noexcept
{
	auto ptr = reinterpret_cast<std::byte*>(_trusted_memory);
	ptr += sizeof(logger*) + sizeof(allocator*)  + sizeof(fit_mode) + sizeof(size_t);
	return *reinterpret_cast<std::mutex*>(ptr);
}

inline allocator_with_fit_mode::fit_mode& allocator_sorted_list::get_fit_mode() const noexcept
{
	auto ptr = reinterpret_cast<std::byte*>(_trusted_memory);
	ptr += sizeof(logger*) + sizeof(allocator*);
	return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr);
}

void* allocator_sorted_list::get_first_suitable(size_t need_size) const noexcept
{
	void* result = _trusted_memory;

	auto block = get_first_block(_trusted_memory);
	while (block != nullptr)
	{
		if (get_size_block(block) >= need_size) return result;
		result = block;
		block = get_ptr_to_block_from_block(block);
	}
	return nullptr;
}

void* allocator_sorted_list::get_worst_suitable(size_t need_size) const noexcept
{
	block_size_t max = 0;
	block_pointer_t result = nullptr, previous_block = _trusted_memory;

	auto block = get_first_block(_trusted_memory);
	while (block != nullptr)
	{
		if (get_size_block(block) >= need_size && get_size_block(block) > max)
		{
			max = get_size_block(block);
			result = previous_block;
		}
		previous_block = block;
		block = get_ptr_to_block_from_block(block);
	}
	return result;
}

void* allocator_sorted_list::get_best_suitable(size_t need_size)  const noexcept
{
	block_size_t max = get_size_full();
	block_pointer_t result = nullptr, previous_block = _trusted_memory;

	auto block = get_first_block(_trusted_memory);
	while (block != nullptr)
	{
		if (get_size_block(block) >= need_size && get_size_block(block) < max)
		{
			max = get_size_block(block);
			result = previous_block;
		}
		previous_block = block;
		block = get_ptr_to_block_from_block(block);
	}
	return result;
}

inline size_t allocator_sorted_list::get_size_full() const noexcept
{
	auto ptr = reinterpret_cast<unsigned char*>(_trusted_memory);
	ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode);
	return *reinterpret_cast<block_size_t *>(ptr);
}

std::string allocator_sorted_list::get_dump(char* at, size_t size)
{
	std::string result;
	for(size_t i = 0; i < size; ++i)
		result += std::to_string(static_cast<int>(at[i])) + " ";
	return result;
}

inline allocator::block_pointer_t& allocator_sorted_list::get_first_block(allocator::block_pointer_t current_block) noexcept
{
	auto ptr = reinterpret_cast<unsigned char*>(current_block);
	ptr += sizeof(logger*) + sizeof(allocator*) + sizeof(fit_mode) +sizeof(block_size_t) + sizeof(std::mutex);
	return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline allocator::block_pointer_t& allocator_sorted_list::get_ptr_to_block_from_block(allocator::block_pointer_t current_block) noexcept
{
	auto ptr = reinterpret_cast<unsigned char *>(current_block);
	ptr += sizeof(block_size_t);
	return *reinterpret_cast<block_pointer_t*>(ptr);
}

inline void* allocator_sorted_list::get_previous_for_loaded(void* loaded_block) const noexcept
{
	block_pointer_t previous = _trusted_memory;

	block_pointer_t block = get_first_block(_trusted_memory);

	while (block != nullptr)
	{
		auto diff = reinterpret_cast<unsigned char*>(block) - reinterpret_cast<unsigned char*>(loaded_block);
		if(diff > 0) return previous;

		previous = block;
		block = get_ptr_to_block_from_block(block);
	}
	return previous;
}

inline size_t allocator_sorted_list::get_size_block(void* current_block) noexcept
{
	return *reinterpret_cast<block_size_t *>(current_block);
}

std::string allocator_sorted_list::get_blocks_info_to_string(const std::vector<allocator_test_utils::block_info>& vector) const noexcept
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