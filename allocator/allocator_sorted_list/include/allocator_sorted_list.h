#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>
#include <sstream>

class allocator_sorted_list final:
		private allocator_guardant,
		public allocator_test_utils,
		public allocator_with_fit_mode,
		private logger_guardant,
		private typename_holder
{

private:

	void *_trusted_memory;

	block_size_t left_bytes;

public:

	~allocator_sorted_list() override;

	allocator_sorted_list(
			allocator_sorted_list const &other) = delete;

	allocator_sorted_list &operator=(
			allocator_sorted_list const &other) = delete;

	allocator_sorted_list(
			allocator_sorted_list &&other) noexcept;

	allocator_sorted_list &operator=(
			allocator_sorted_list &&other) noexcept;

public:

	explicit allocator_sorted_list(
			size_t space_size,
			allocator *parent_allocator = nullptr,
			logger *logger = nullptr,
			allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:

	[[nodiscard]] void *allocate(
			size_t value_size,
			size_t values_count) override;

	void deallocate(
			void *at) override;

public:

	inline void set_fit_mode(
			allocator_with_fit_mode::fit_mode mode) override;

public:

	std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

	inline allocator *get_allocator() const override;

private:

	inline logger *get_logger() const override;

private:

	inline std::string get_typename() const noexcept override;

private:

	inline allocator::block_size_t get_allocator_size_of_meta() const noexcept;

	inline allocator::block_size_t get_block_size_of_meta() const noexcept;

	inline std::mutex& get_mutex() noexcept;

	inline allocator_with_fit_mode::fit_mode& get_fit_mode() const noexcept;

	[[nodiscard]] void* get_first_suitable(size_t need_size)  const noexcept;

	[[nodiscard]] void* get_worst_suitable(size_t need_size) const noexcept;

	[[nodiscard]] void* get_best_suitable(size_t need_size)  const noexcept;

	inline size_t get_size_full() const noexcept;

	static inline size_t get_size_block(void* current_block) noexcept;

	static inline allocator::block_pointer_t& get_first_block(allocator::block_pointer_t current_block) noexcept;

	inline static allocator::block_pointer_t& get_ptr_to_block_from_block(allocator::block_pointer_t current_block) noexcept;

	inline void* get_previous_for_loaded(void* loaded_ptr) const noexcept;

	std::string get_blocks_info_to_string(const std::vector<allocator_test_utils::block_info>& vector) const noexcept;

	static std::string get_dump(char* at, size_t size);

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_SORTED_LIST_H