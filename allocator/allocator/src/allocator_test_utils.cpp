#include "../include/allocator_test_utils.h"

bool allocator_test_utils::block_info::operator==(
		allocator_test_utils::block_info const &other) const noexcept
{
	// Проверяем, равны ли размеры блоков и занятость блоков
	return block_size == other.block_size
		   && is_block_occupied == other.is_block_occupied;
}

bool allocator_test_utils::block_info::operator!=(
		allocator_test_utils::block_info const &other) const noexcept
{
	// Используем оператор == для проверки неравенства
	return !(*this == other);
}
