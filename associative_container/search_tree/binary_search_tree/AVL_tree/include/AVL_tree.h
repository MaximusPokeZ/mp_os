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
			binary_search_tree<tkey, tvalue>::node
	{
	public:

		size_t subtree_height;

	public:

		explicit  node(
				tkey const& key,
				tvalue const& value);

		explicit node(
				tkey const& key,
				tvalue&& value);

		~node() noexcept = default;


	public:

		void change_balance() noexcept;

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


		iterator_data(
				iterator_data const &other);

		iterator_data(
				iterator_data &&other) noexcept;

		iterator_data &operator=(
				iterator_data const &other);

		iterator_data &operator=(
				iterator_data &&other) noexcept;

		~iterator_data() noexcept final;

	public:

		void update_iterator_data(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
		{
			if (!path.empty())
			{
				binary_search_tree<tkey, tvalue>::iterator_data::key = (*(path.top()))->key;
				binary_search_tree<tkey, tvalue>::iterator_data::value = (*(path.top()))->value;
				binary_search_tree<tkey, tvalue>::iterator_data::depth = path.size() - 1;
				subtree_height = static_cast<node *>(*(path.top()))->subtree_height;
			}
		}

		iterator_data* clone_iterator_data() const override
		{
			return new iterator_data(*this);
		}

	};


public:

	explicit AVL_tree(
			std::function<int(tkey const &, tkey const &)> keys_comparer,
			allocator *allocator = nullptr,
			logger *logger = nullptr,
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

	~AVL_tree() noexcept final;

	AVL_tree(
			AVL_tree<tkey, tvalue> const &other);

	AVL_tree<tkey, tvalue> &operator=(
			AVL_tree<tkey, tvalue> const &other);

	AVL_tree(
			AVL_tree<tkey, tvalue> &&other) noexcept;

	AVL_tree<tkey, tvalue> &operator=(
			AVL_tree<tkey, tvalue> &&other) noexcept;

private:

	void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stack) override;

	void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stack) override;

	tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stack) override;

	void copy_subtree(typename binary_search_tree<tkey, tvalue>::node** target, typename binary_search_tree<tkey, tvalue>::node* src);

	template<typename avl_tvalue>
	void avl_insert(const tkey& key, avl_tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stack);

	typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data() const override;

	inline void update_node_data(
			typename binary_search_tree<tkey,tvalue>::node *node) const noexcept override;

	static inline unsigned int get_subtree_height(
			typename binary_search_tree<tkey,tvalue>::node* node) noexcept;

};

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
		unsigned int depth,
		tkey const &key,
		tvalue const &value,
		size_t subtree_height):
		binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value), subtree_height(subtree_height) {}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data const &other):
		binary_search_tree<tkey, tvalue>::iterator_data(other),
		subtree_height(other.subtree_height) {}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data &&other) noexcept:
		binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
		subtree_height(std::move(other.subtree_height)) {}

template<
		typename tkey,
		typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data const &other)
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
		subtree_height = other.subtree_height;
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename AVL_tree<tkey, tvalue>::iterator_data &AVL_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data &&other) noexcept
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
		subtree_height = std::move(other.subtree_height);
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::iterator_data::~iterator_data() noexcept {}


template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
		const tkey &key, const tvalue &value) :
		binary_search_tree<tkey, tvalue>::node(key, value) {}

template<typename tkey, typename tvalue>
AVL_tree<tkey, tvalue>::node::node(
		const tkey &key, tvalue&& value) :
		binary_search_tree<tkey, tvalue>::node(key, std::move(value)) {}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::node::change_balance() noexcept
{
	size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->subtree_height : 0;
	size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree != nullptr ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->subtree_height : 0;
	subtree_height = std::max(left_height, right_height) + 1;
}

template<typename tkey, typename tvalue>
int AVL_tree<tkey, tvalue>::node::get_balance() noexcept
{
	if (this == nullptr)
		return 0;
	size_t left_height = binary_search_tree<tkey, tvalue>::node::left_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->subtree_height : 0;
	size_t right_height = binary_search_tree<tkey, tvalue>::node::right_subtree ? static_cast<node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->subtree_height : 0;

	return right_height - left_height;
}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) :
		binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy) {}


template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::copy_subtree(typename binary_search_tree<tkey, tvalue>::node** target, typename binary_search_tree<tkey, tvalue>::node* src)
{
	if(src == nullptr)
	{
		*target = nullptr;
		return;
	}

	*target = static_cast<typename binary_search_tree<tkey, tvalue>::node*>(reinterpret_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node))));
	try
	{
		allocator::construct(static_cast<node*>(*target), *static_cast<node*>(src));
	} catch(std::bad_alloc & ex)
	{
		allocator_guardant::deallocate_with_guard(static_cast<node*>(*target));
		*target = nullptr;
		throw ex;
	}

	(*target)->right_subtree = nullptr;
	(*target)->left_subtree = nullptr;

	copy_subtree(&((*target)->left_subtree), src->left_subtree);
	copy_subtree(&((*target)->right_subtree), src->right_subtree);
}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::~AVL_tree() noexcept = default;

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue>::AVL_tree(
		AVL_tree<tkey, tvalue> const &other) : binary_search_tree<tkey, tvalue>(other) {}

template<
		typename tkey,
		typename tvalue>
AVL_tree<tkey, tvalue> &AVL_tree<tkey, tvalue>::operator=(
		AVL_tree<tkey, tvalue> const &other)
{

	if (this != other)
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


template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *AVL_tree<tkey, tvalue>::create_iterator_data() const
{
	return new iterator_data();
}

template<
		typename tkey,
		typename tvalue>
inline void AVL_tree<tkey, tvalue>::update_node_data(
		typename binary_search_tree<tkey,tvalue>::node *node) const noexcept
{
	AVL_tree<tkey, tvalue>::node *avl_node = static_cast<AVL_tree<tkey, tvalue>::node*>(node);

	size_t left_subtree_height = get_subtree_height(avl_node->left_subtree);
	size_t right_subtree_height = get_subtree_height(avl_node->right_subtree);

	avl_node->subtree_height = std::max(left_subtree_height, right_subtree_height) + 1;
}

template<
		typename tkey,
		typename tvalue>
inline unsigned int AVL_tree<tkey, tvalue>::get_subtree_height(
		typename binary_search_tree<tkey,tvalue>::node* node) noexcept
{
	auto *avl_node = static_cast<AVL_tree<tkey,tvalue>::node*>(node);

	return avl_node ? avl_node->subtree_height : 0;
}

// endregion iterator_implem


// AVL_rules

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stack)
{
	avl_insert(key, value, stack);
}

template<typename tkey, typename tvalue>
void AVL_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &stack)
{
	avl_insert(key, std::move(value), stack);
}

template<typename tkey, typename tvalue>
template<typename avl_tvalue>
void AVL_tree<tkey, tvalue>::avl_insert(const tkey &key, avl_tvalue &&value, std::stack<typename binary_search_tree<tkey, tvalue>::node**> & stack)
{
	(*stack.top()) = static_cast<typename binary_search_tree<tkey, tvalue>::node*>(static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node), 1)));

	try {
		allocator::construct(*stack.top(), key, std::forward<avl_tvalue>(value));
	}
	catch(...)
	{
		allocator_guardant::deallocate_with_guard(*stack.top());
		throw;
	}

	static_cast<node*>(*stack.top())->change_balance();

	while (!stack.empty())
	{
		static_cast<node*>(*stack.top())->change_balance();

		int balance = static_cast<node*>(*stack.top())->get_balance();

		if (balance > 1 && (*stack.top())->right_subtree != nullptr && (*stack.top())->right_subtree->left_subtree != nullptr)
		{
			binary_search_tree<tkey, tvalue>::big_left_rotation(*stack.top());
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
			static_cast<node*>((*stack.top()))->change_balance();

		}
		else if (balance > 1)
		{
			binary_search_tree<tkey, tvalue>::small_left_rotation(*stack.top());
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
			static_cast<node*>((*stack.top()))->change_balance();
		}
		else if (balance < -1 && (*stack.top())->left_subtree != nullptr && (*stack.top())->left_subtree->right_subtree != nullptr)
		{
			binary_search_tree<tkey, tvalue>::big_right_rotation(*stack.top());
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
			static_cast<node*>((*stack.top()))->change_balance();
		}
		else if (balance < -1)
		{
			binary_search_tree<tkey, tvalue>::small_right_rotation(*stack.top());
			static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
			static_cast<node*>((*stack.top()))->change_balance();
		}

		stack.pop();
	}
}

template<typename tkey, typename tvalue>
tvalue AVL_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& stack)
{
	tvalue res = (*stack.top())->value;

	typename binary_search_tree<tkey, tvalue>::node* current_node = *stack.top();

	if (current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
	{
		*stack.top() = nullptr;
		stack.pop();
	}
	else
	{
		if (current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
		{
			*stack.top() = current_node->right_subtree != nullptr ? current_node->right_subtree : current_node->left_subtree;
		}
		else
		{
			std::queue<typename binary_search_tree<tkey, tvalue>::node**> que;
			typename binary_search_tree<tkey, tvalue>::node** update = &current_node->left_subtree;

			while ((*update)->right_subtree != nullptr)
			{
				update = &(*update)->right_subtree;
				que.push(update);
			}

			*stack.top() = *update;
			*update = (*update)->left_subtree;
			(*stack.top())->left_subtree = current_node->left_subtree;
			(*stack.top())->right_subtree = current_node->right_subtree;

			if (!que.empty())
			{
				stack.push(&(*stack.top())->left_subtree);
			}

			while (!que.empty())
			{
				if (que.size() == 1)
				{
					break;
				}
				stack.push(que.front());
				que.pop();
			}
		}
	}

	allocator::destruct(current_node);
	allocator_guardant::deallocate_with_guard(current_node);

	while (!stack.empty())
	{
		typename binary_search_tree<tkey, tvalue>::node* current = *stack.top();
		static_cast<node*>(current)->change_balance();
		int balance = static_cast<node*>(current)->get_balance();

		if (balance > 1)
		{
			int child_balance = static_cast<node*>(current->left_subtree)->get_balance();
			if (child_balance >= 0)
			{
				binary_search_tree<tkey, tvalue>::small_left_rotation(*stack.top());
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
				static_cast<node*>((*stack.top()))->change_balance();
			}
			else
			{
				binary_search_tree<tkey, tvalue>::big_right_rotation(*stack.top());
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
				static_cast<node*>((*stack.top()))->change_balance();
			}
		}
		else if (balance < -1)
		{
			int child_balance = static_cast<node*>(current->right_subtree)->get_balance();
			if (child_balance <= 0)
			{
				binary_search_tree<tkey, tvalue>::small_right_rotation(*stack.top());
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
				static_cast<node*>((*stack.top()))->change_balance();
			}
			else
			{
				binary_search_tree<tkey, tvalue>::big_left_rotation(*stack.top());
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::left_subtree)->change_balance();
				static_cast<node*>((*stack.top())->binary_search_tree<tkey, tvalue>::node::right_subtree)->change_balance();
				static_cast<node*>((*stack.top()))->change_balance();
			}
		}
		stack.pop();
	}

	return res;

}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_AVL_TREE_H