#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H

#include <binary_search_tree.h>

template<
	typename tkey,
	typename tvalue>
class splay_tree final:
		public binary_search_tree<tkey, tvalue>
{

public:

	explicit splay_tree(std::function<int(tkey const &, tkey const &)> keys_comparer,
						allocator *allocator = nullptr,
						logger *logger = nullptr,
						typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
						typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

	~splay_tree() noexcept final = default;

	splay_tree(
			splay_tree<tkey, tvalue> const &other) = default;

	splay_tree<tkey, tvalue> &operator=(
			splay_tree<tkey, tvalue> const &other) = default;

	splay_tree(
			splay_tree<tkey, tvalue> &&other) noexcept = default;

	splay_tree<tkey, tvalue> &operator=(
			splay_tree<tkey, tvalue> &&other) noexcept = default;


private:

	void splay(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

	void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	template<typename splay_tree_value>
	void insert_splay_tree(const tkey& key, splay_tree_value&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

	tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

};

template<
		typename tkey,
		typename tvalue>
splay_tree<tkey, tvalue>::splay_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy)
			: binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy) {}



template<
	typename tkey,
	typename tvalue>
void splay_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	insert_splay_tree(key, value, path);
}

template<
	typename tkey,
	typename tvalue>
void splay_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	insert_splay_tree(key, std::move(value), path);
}


template<typename tkey, typename tvalue>
template<typename splay_tree_value>
void splay_tree<tkey, tvalue>::insert_splay_tree(const tkey &key, splay_tree_value &&val, std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	(*path.top()) = reinterpret_cast<binary_search_tree<tkey, tvalue>::node*>(allocator_guardant::allocate_with_guard(sizeof(typename binary_search_tree<tkey, tvalue>::node)));
	try
	{
		allocator::construct(*path.top(), key, std::forward<splay_tree_value>(val));
	}
	catch(std::bad_alloc & ex)
	{
		allocator_guardant::deallocate_with_guard(*path.top());
		throw ex;
	}

	splay(path); // вставили - сразу балансируем
}


template<typename tkey, typename tvalue>
tvalue splay_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{
	splay(path);

	tvalue res = binary_search_tree<tkey, tvalue>::_root->value;

	typename binary_search_tree<tkey, tvalue>::node* left = binary_search_tree<tkey, tvalue>::_root ->left_subtree;
	typename binary_search_tree<tkey, tvalue>::node* right = binary_search_tree<tkey, tvalue>::_root ->right_subtree;

	allocator::destruct(binary_search_tree<tkey, tvalue>::_root);
	allocator_guardant::deallocate_with_guard(binary_search_tree<tkey, tvalue>::_root);

	typename binary_search_tree<tkey, tvalue>::node* current_node = left;
	std::stack<typename binary_search_tree<tkey, tvalue>::node**> new_path;

	new_path.push(&current_node);

	while(current_node->right_subtree != nullptr)
	{
		current_node = current_node->right_subtree;
		new_path.push(&current_node);
	}

	splay(new_path);

	binary_search_tree<tkey, tvalue>::_root = left;
	binary_search_tree<tkey, tvalue>::_root->right_subtree = right;

	return res;
}

template<
		typename tkey,
		typename tvalue>
void splay_tree<tkey, tvalue>::splay(std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	bool is_parent_left_child, is_grandparent_left_child;

	while(path.size() > 1)
	{
		while(path.size() >= 3)
		{
			typename binary_search_tree<tkey, tvalue>::node* parent = *(path.top()); path.pop();
			is_parent_left_child = (*(path.top()))->left_subtree == parent;

			typename binary_search_tree<tkey, tvalue>::node* grand = (*(path.top())); path.pop();
			is_grandparent_left_child = (*(path.top()))->left_subtree == grand;

			if(is_parent_left_child && is_grandparent_left_child)
			{
				binary_search_tree<tkey, tvalue>::double_right_rotation(*path.top()); // zig zig right
			}
			else if(!is_parent_left_child && !is_grandparent_left_child)
			{
				binary_search_tree<tkey, tvalue>::double_left_rotation(*path.top()); // zig zig left
			}
			else if(!is_parent_left_child)
			{
				binary_search_tree<tkey, tvalue>::big_right_rotation(*path.top()); // zig zag right
			}
			else
			{
				binary_search_tree<tkey, tvalue>::big_left_rotation(*path.top()); // zig zag left
			}

		}
		if(path.size() >= 2)
		{

			typename binary_search_tree<tkey, tvalue>::node* parent = *(path.top());
			path.pop();
			is_grandparent_left_child = (*(path.top()))->left_subtree == parent;

			if(is_grandparent_left_child)
			{
				binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top()); // zig right
			}
			else
			{
				binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top()); // zig left
			}
		}
	}
}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SPLAY_TREE_H