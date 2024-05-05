#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H

#include <binary_search_tree.h>
#include <stack>
#include <queue>

template<
    typename tkey,
    typename tvalue>
class AVL_tree final:
    public binary_search_tree<tkey, tvalue>
{

private:

	struct node final:
			binary_search_tree<tkey, tvalue>::node {
	public:

		size_t subtree_height;

	public:

		explicit node(
			tkey const &key,
			tvalue const &value);

		explicit node(
			tkey const &key,
			tvalue &&value);


	public:

		void update_height() noexcept;

		int get_balance() noexcept;
	};

public:

    struct iterator_data final:
        public binary_search_tree<tkey, tvalue>::iterator_data
    {

    public:

        size_t subtree_height;

    public:

        explicit iterator_data(
            unsigned int depth = 0,
            tkey const &key = tkey(),
            tvalue const &value = tvalue(),
            size_t subtree_height = 0);

		void update_iterator(const binary_search_tree<tkey, tvalue>::node *node, unsigned int _depth) override;

		binary_search_tree<tkey, tvalue>::iterator_data* create_deep_copy_of_iterator_data()  override;

    };

public:

	class prefix_const_iterator : public binary_search_tree<tkey, tvalue>::prefix_const_iterator
	{
	public:
		explicit prefix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
				: binary_search_tree<tkey, tvalue>::prefix_const_iterator(path, it_data) {}
	};

	class infix_const_iterator : public binary_search_tree<tkey, tvalue>::infix_const_iterator
	{
	public:
		explicit infix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
				: binary_search_tree<tkey, tvalue>::infix_const_iterator(path, it_data) {}
	};

	class postfix_const_iterator : public binary_search_tree<tkey, tvalue>::postfix_const_iterator
	{
	public:
		explicit postfix_const_iterator(const std::stack<typename binary_search_tree<tkey, tvalue>::node**> path = std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), iterator_data* it_data = nullptr)
				: binary_search_tree<tkey, tvalue>::postfix_const_iterator(path, it_data) {}
	};

	prefix_const_iterator cbegin_prefix() const noexcept;

	prefix_const_iterator cend_prefix() const noexcept;

	infix_const_iterator cbegin_infix() const noexcept;

	infix_const_iterator cend_infix() const noexcept;

	postfix_const_iterator cbegin_postfix() const noexcept;

	postfix_const_iterator cend_postfix() const noexcept;


public:

    explicit AVL_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator = nullptr,
		logger *logger = nullptr,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    ~AVL_tree() noexcept final = default;

    AVL_tree(
        AVL_tree<tkey, tvalue> const &other);

    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> const &other);

    AVL_tree(
        AVL_tree<tkey, tvalue> &&other) noexcept;

    AVL_tree<tkey, tvalue> &operator=(
        AVL_tree<tkey, tvalue> &&other) noexcept;


private:

	void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	template<typename avl_tvalue>
	void avl_insert(const tkey& key, avl_tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

};

// region node methods

template<
	typename tkey,
	typename tvalue>
int AVL_tree<tkey, tvalue>::node::get_balance() noexcept
{
	size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->subtree_height : 0;
	size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->subtree_height : 0;

	return right_height - left_height;
}

template<
		typename tkey,
		typename tvalue>
void AVL_tree<tkey, tvalue>::node::update_height() noexcept
{
	size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->subtree_height : 0;
	size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->subtree_height : 0;

	subtree_height = std::max(right_height, left_height) + 1;
}

// endregion node methods

// region iterator_data

template<
	typename tkey,
	typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::iterator_data::create_deep_copy_of_iterator_data()
{
	return new iterator_data(*this);
}


template<
	typename tkey,
	typename tvalue>
void AVL_tree<tkey, tvalue>::iterator_data::update_iterator(const typename binary_search_tree<tkey, tvalue>::node *node, unsigned int _depth)
{
	binary_search_tree<tkey, tvalue>::iterator_data::key = node->key;
	binary_search_tree<tkey, tvalue>::iterator_data::value = node->value;
	binary_search_tree<tkey, tvalue>::iterator_data::depth = _depth;
	subtree_height = static_cast<const struct node*>(node)->subtree_height;
}

template<
    typename tkey,
    typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value,
    size_t subtree_height):
    binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), subtree_height(subtree_height)
	{}


template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_iterator AVL_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
	std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
	if(binary_search_tree<tkey, tvalue>::_root != nullptr)
	{
		stk.push((&(binary_search_tree<tkey, tvalue>::_root)));
	}

	return prefix_const_iterator(stk, new iterator_data);
}

template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::prefix_const_iterator AVL_tree<tkey, tvalue>::cend_prefix() const noexcept
{
	return AVL_tree<tkey, tvalue>::prefix_const_iterator();
}

template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_iterator AVL_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
	std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
	typename binary_search_tree<tkey, tvalue>::node** current_node = &(binary_search_tree<tkey, tvalue>::_root);
	while(*current_node != nullptr)
	{
		stk.push(current_node);
		current_node = (&((*current_node)->left_subtree));
	}

	return infix_const_iterator(stk, new iterator_data);
}

template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::infix_const_iterator AVL_tree<tkey, tvalue>::cend_infix() const noexcept
{
	return AVL_tree<tkey, tvalue>::infix_const_iterator();
}


template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_iterator AVL_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
	std::stack<typename binary_search_tree<tkey, tvalue>::node**> stk;
	typename binary_search_tree<tkey, tvalue>::node** current_node = (&(binary_search_tree<tkey, tvalue>::_root));
	while(*current_node != nullptr)
	{
		stk.push(current_node);
		current_node = (*current_node)->left_subtree != nullptr ? (&((*current_node)->left_subtree)) : (&((*current_node)->right_subtree));
	}
	return postfix_const_iterator(stk, new iterator_data);
}

template<
	typename tkey,
	typename tvalue>
typename AVL_tree<tkey, tvalue>::postfix_const_iterator AVL_tree<tkey, tvalue>::cend_postfix() const noexcept
{
	return AVL_tree<tkey, tvalue>::postfix_const_iterator();
}


// endregion iterator_data

// region AVL rules construct

template<
	typename tkey,
	typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
	 std::function<int(tkey const &, tkey const &)> comparer,
	 allocator *allocator,
	 logger *logger,
	 typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
	 typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(comparer, allocator, logger, insertion_strategy, disposal_strategy) {}

template<
	typename tkey,
	typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(AVL_tree<tkey, tvalue> const &other) : binary_search_tree<tkey, tvalue>(other) {}

template<
	typename tkey,
	typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(AVL_tree<tkey, tvalue> const &other)
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::operator=(other);
	}
	return *this;
}

template<
	typename tkey,
	typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
		AVL_tree<tkey, tvalue> &&other) noexcept:
		binary_search_tree<tkey, tvalue>(std::move(other)) {}

template<
	typename tkey,
	typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
		AVL_tree<tkey, tvalue> &&other) noexcept
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::operator=(std::move(other));
	}

	return *this;
}

// endregion AVL rules construct


template<
	typename tkey,
	typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stack)
{
	avl_insert(key, value, stack);
}

template<
	typename tkey,
	typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stack)
{
	avl_insert(key, std::move(value), stack);
}

template<
	typename tkey,
	typename tvalue>
template<typename avl_tvalue>
void AVL_tree<tkey, tvalue>::avl_insert(const tkey &key, avl_tvalue &&value, std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path)
{
	(*path.top()) = static_cast<typename binary_search_tree<tkey, tvalue>::node*>(static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node))));

	try {
		allocator::construct(*path.top(), key, std::forward<avl_tvalue>(value));
	}
	catch(std::bad_alloc & ex)
	{
		allocator_guardant::deallocate_with_guard(*path.top());
		throw ex;
	}

	static_cast<node*>(*path.top())->update_height();

	while (!path.empty())
	{
		static_cast<node*>(*path.top())->update_height();

		int balance = static_cast<node*>(*path.top())->get_balance();

		if (balance > 1 && (*path.top())->right_subtree != nullptr && (*path.top())->right_subtree->left_subtree != nullptr){
			binary_search_tree<tkey, tvalue>::big_left_rotation(*path.top());
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
			static_cast<node*>((*path.top()))->update_height();

		}
		else if (balance > 1) {
			binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top());
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
			static_cast<node*>((*path.top()))->update_height();
		}
		else if (balance < -1 && (*path.top())->left_subtree != nullptr && (*path.top())->left_subtree->right_subtree != nullptr) {
			binary_search_tree<tkey, tvalue>::big_right_rotation(*path.top());
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
			static_cast<node*>((*path.top()))->update_height();
		}
		else if (balance < -1){
			binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top());
			static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
			static_cast<node*>((*path.top()))->update_height();
		}

		path.pop();
	}
}

template<
	typename tkey,
	typename tvalue>
tvalue AVL_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path)
{
	tvalue res = (*path.top())->value;

	typename binary_search_tree<tkey, tvalue>::node* current_node = *path.top();

	if (current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
	{
		*path.top() = nullptr;
		path.pop();
	}
	else
	{
		if (current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
		{
			*path.top() = current_node->right_subtree != nullptr ? current_node->right_subtree : current_node->left_subtree;
		}
		else
		{
			std::queue<typename binary_search_tree<tkey, tvalue>::node**> que;
			typename binary_search_tree<tkey, tvalue>::node** update = &current_node->left_subtree;

			//наибольший узел в левом поддереве
			while ((*update)->right_subtree != nullptr)
			{
				update = &(*update)->right_subtree;
				que.push(update);
			}

			*path.top() = *update;
			*update = (*update)->left_subtree;
			(*path.top())->left_subtree = current_node->left_subtree;
			(*path.top())->right_subtree = current_node->right_subtree;

			// обновляем путь к новому узлу
			if (!que.empty())
			{
				path.push(&(*path.top())->left_subtree);
			}

			while (!que.empty())
			{
				if (que.size() == 1)
				{
					break;
				}
				path.push(que.front());
				que.pop();
			}
		}
	}

	allocator::destruct(current_node);
	allocator_guardant::deallocate_with_guard(current_node);

	while (!path.empty())
	{
		typename binary_search_tree<tkey, tvalue>::node* current = *path.top();
		static_cast<node*>(current)->update_height();
		int balance = static_cast<node*>(current)->get_balance();

		if (balance > 1)
		{
			int child_balance = static_cast<node*>(current->right_subtree)->get_balance();
			if (child_balance >= 0)
			{
				binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top());
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
				static_cast<node*>((*path.top()))->update_height();
			}
			else
			{
				binary_search_tree<tkey, tvalue>::big_right_rotation(*path.top());
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
				static_cast<node*>((*path.top()))->update_height();
			}
		}
		else if (balance < -1)
		{
			int child_balance = static_cast<node*>(current->right_subtree)->get_balance();
			if (child_balance <= 0)
			{
				binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top());
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
				static_cast<node*>((*path.top()))->update_height();
			}
			else
			{
				binary_search_tree<tkey, tvalue>::big_left_rotation(*path.top());
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->update_height();
				static_cast<node*>((*path.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->update_height();
				static_cast<node*>((*path.top()))->update_height();
			}
		}
		path.pop();
	}

	return res;

}



#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H