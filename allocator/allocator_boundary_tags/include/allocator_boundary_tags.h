#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

class allocator_boundary_tags final:
    private allocator_guardant,
    public allocator_test_utils,
    public allocator_with_fit_mode,
    private logger_guardant,
    private typename_holder
{

private:
    
    void *_trusted_memory;

	size_t left_bytes;

public:
    
    ~allocator_boundary_tags() override;
    
    allocator_boundary_tags(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags const &other) = delete;
    
    allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept;
    
    allocator_boundary_tags &operator=(
        allocator_boundary_tags &&other) noexcept;

public:
    
    explicit allocator_boundary_tags(
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

private:
    
    inline allocator *get_allocator() const override;

public:
    
    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    
    inline logger *get_logger() const override;

private:
    
    inline std::string get_typename() const noexcept override;

private:

	std::mutex &get_mutex() const noexcept;

	allocator_with_fit_mode::fit_mode& get_fit_mode() const noexcept;

	inline size_t &get_allocator_size() const noexcept;

	inline block_pointer_t & get_first_block_by_alloc() const noexcept;

	inline allocator::block_size_t get_block_size_of_meta() const noexcept;

	inline allocator::block_size_t get_allocator_size_of_meta() const noexcept;

	inline allocator *&get_allocator_block(block_pointer_t cur_block) const noexcept;

	inline block_size_t &get_size_block(block_pointer_t cur_block) const noexcept;

	inline block_pointer_t& get_prev_loaded_block(block_pointer_t cur_block) const noexcept;

	inline block_pointer_t& get_next_loaded_block(block_pointer_t cur_block) const noexcept;

	block_pointer_t get_first_suitable(size_t need_size)  const noexcept;

	block_pointer_t get_suitable(size_t need_size, bool is_best_fit) const noexcept;

	std::string get_blocks_info_to_string(const std::vector<allocator_test_utils::block_info>& vector) const noexcept;

	std::string get_dump(char* at, size_t size);

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H