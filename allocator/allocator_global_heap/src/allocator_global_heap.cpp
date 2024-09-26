#include <not_implemented.h>
#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger) : _logger(logger)
{
	debug_with_guard("Start constructor of " + get_typename());;
}

allocator_global_heap::~allocator_global_heap()
{
	debug_with_guard("Start destructor of " + get_typename());;
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept : _logger(other._logger)
{
	debug_with_guard("Start move constructor of " + get_typename());
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
	if (this != &other)
	{
		_logger = other._logger;
	}
	debug_with_guard("Start assignment operator of " + get_typename());

	return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
	const size_t meta_size = sizeof(size_t);
	size_t need_result_mem = value_size * values_count;
	debug_with_guard("Start alloc " + get_typename() + ":\n[value_size = " + std::to_string(value_size) + "]\n[values_count = " +
					 std::to_string(values_count) + "]\n[meta = " + std::to_string(meta_size) +
					 "]\n[Result size = value_size * values_count = " + std::to_string(need_result_mem) + "]");

	void *allocated_block_memory;

	try
	{
		allocated_block_memory = ::operator new(need_result_mem + meta_size + sizeof(allocator_global_heap*)); // Allocates memory but does not initialize
	}
	catch (std::bad_alloc const &)
	{
		error_with_guard(get_typename() + " failed to allocate " + std::to_string(need_result_mem) + " bytes of memory");
		throw;
	}
	debug_with_guard(
			"Success alloc " + get_typename() + "with " + std::to_string(need_result_mem) + " bytes of memory");


	*reinterpret_cast<allocator_global_heap**>(allocated_block_memory) = this; //указатель на наш аллокатор
	auto* ptr = reinterpret_cast<unsigned char*> (allocated_block_memory);
	*reinterpret_cast<size_t *> (ptr + sizeof(allocator_global_heap*)) = need_result_mem; //записываем в метаданные размер блока


	return reinterpret_cast<void*>(ptr + meta_size + sizeof(allocator_global_heap*)); // возвращаем адрес на начало чистой пользовательской памяти
}

void allocator_global_heap::deallocate(
    void *at)
{
	debug_with_guard("Start deallocate " + get_typename());

	auto *ptr = reinterpret_cast<unsigned char*> (at);
	ptr -= sizeof(size_t); // переходим к метаданным
	size_t size = *reinterpret_cast<size_t *>(ptr); //получаем размер из метаданных
	std::string dump = get_block_dump(reinterpret_cast<char*>(at), size);
	//std::cout << dump;

	ptr -= sizeof(allocator_global_heap *);

	//получаем фактический указатель на объект allocator_global_heap, который выделил этот лок памяти
	allocator_global_heap *alloc_ptr = *reinterpret_cast<allocator_global_heap **>(ptr);

	if (alloc_ptr != this)
	{
		error_with_guard(get_typename() + " calling another allocator");
		throw std::runtime_error(get_typename() + " calling another allocator");
	}

	debug_with_guard(get_typename() + " finished deallocate " + std::to_string(size) + " bytes dump :" + dump);
	::operator delete(ptr);
}

inline logger *allocator_global_heap::get_logger() const
{
	return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
	return "allocator_global_heap";;
}

std::string allocator_global_heap::get_block_dump(char *at, size_t size)
{
	std::string string_dump;
	for(size_t i = 0; i < size; ++i) // Проходим по каждому байту в блоке памяти
	{
		string_dump += std::to_string(static_cast<int>(at[i])) + " "; // Преобразуем текущий байт в строку и добавляем его к результату
	}
	return string_dump; // Возвращаем строковое представление блока памяти
}
