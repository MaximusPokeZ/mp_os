#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H

#include <binary_search_tree.h>
#include <queue>

template<
		typename tkey,
		typename tvalue>
class scapegoat_tree final:
		public binary_search_tree<tkey, tvalue>
{

private:

	struct scapegoat_node final:
			binary_search_tree<tkey, tvalue>::node
	{
	public:

		size_t size_crnt_node;

		explicit scapegoat_node(const tkey& key, const tvalue& value);

		explicit scapegoat_node(const tkey& key, tvalue&& value);

	public:

		void update_count_nodes() noexcept
		{
			size_t left_size = binary_search_tree<tkey, tvalue>::node::left_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->size_crnt_node;
			size_t right_size = binary_search_tree<tkey, tvalue>::node::right_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->size_crnt_node;
			size_crnt_node = left_size + right_size + 1;
		}

		bool need_balance(double alpha) noexcept
		{
			size_t left_size = binary_search_tree<tkey, tvalue>::node::left_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::left_subtree)->size_crnt_node;
			size_t right_size = binary_search_tree<tkey, tvalue>::node::right_subtree == nullptr ? 0 : static_cast<scapegoat_node*>(binary_search_tree<tkey, tvalue>::node::right_subtree)->size_crnt_node;

			return !((size_crnt_node * alpha >= left_size) && (size_crnt_node * alpha >= right_size));
		}

	};


public:

	explicit scapegoat_tree(
			std::function<int(tkey const &, tkey const &)> keys_comparer,
			allocator *allocator = nullptr,
			logger *logger = nullptr,
			double alpha = 0.5,
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

	~scapegoat_tree() noexcept final = default;

	scapegoat_tree(
			scapegoat_tree<tkey, tvalue> const &other) = default;

	scapegoat_tree<tkey, tvalue> &operator=(
			scapegoat_tree<tkey, tvalue> const &other) = default;

	scapegoat_tree(
			scapegoat_tree<tkey, tvalue> &&other) noexcept = default;

	scapegoat_tree<tkey, tvalue> &operator=(
			scapegoat_tree<tkey, tvalue> &&other) noexcept = default;

private:

	double _alpha;

	void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	template<typename scg_value>
	void insert_inside_to_scape_goat(const tkey& key, scg_value&& val, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

	void collect_vector(typename binary_search_tree<tkey, tvalue>::node** goat);

	void rebuilding_tree(typename binary_search_tree<tkey, tvalue>::node** new_top_node, std::vector<scapegoat_node*>::iterator begin, std::vector<scapegoat_node*>::iterator end);

	void balance_afrer_new_alpha(typename binary_search_tree<tkey, tvalue>::node** root);

	tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

public:

	void setup_new_alpha(double alpha);

};


template<
	typename tkey,
	typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_node::scapegoat_node(const tkey &key, const tvalue &value) :
				binary_search_tree<tkey, tvalue>::node(key, value),
				size_crnt_node(1) {}

template<
	typename tkey,
	typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_node::scapegoat_node(const tkey &key, tvalue&& value) :
				binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
				size_crnt_node(1) {}


template<
		typename tkey,
		typename tvalue>
scapegoat_tree<tkey, tvalue>::scapegoat_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		double alpha,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) : binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy)
{
	alpha = alpha < 0.5 ? 0.5 : alpha > 1 ? 1 : alpha;
	_alpha = alpha;
}


template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::setup_new_alpha(
		double alpha)
{
	alpha = alpha < 0.5 ? 0.5 : alpha > 1 ? 1 : alpha;
	_alpha = alpha;

	balance_afrer_new_alpha(&(binary_search_tree<tkey, tvalue>::_root));
}

template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	insert_inside_to_scape_goat(key, value, path);
}

template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	insert_inside_to_scape_goat(key, std::move(value), path);
}

template<
	typename tkey,
	typename tvalue>
template<typename scg_value>
void scapegoat_tree<tkey, tvalue>::insert_inside_to_scape_goat(const tkey &key, scg_value &&val,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	(*path.top()) = static_cast<binary_search_tree<tkey, tvalue>::node*>(static_cast<scapegoat_node*>(allocator_guardant::allocate_with_guard(sizeof(scapegoat_node))));
	try
	{
		allocator::construct(*path.top(), key, std::forward<scg_value>(val));
	}
	catch(std::bad_alloc & ex)
	{
		allocator_guardant::deallocate_with_guard(*path.top());
		throw ex;
	}

	typename binary_search_tree<tkey, tvalue>::node** goat_node = nullptr;

	while(!path.empty())
	{
		static_cast<scapegoat_node*>(*path.top())->update_count_nodes();
		bool needBalance = static_cast<scapegoat_node*>(*path.top())->need_balance(_alpha);
		if(needBalance)
		{
			goat_node = path.top();
		}
		path.pop();
	}


	if(goat_node != nullptr)
	{
		collect_vector(goat_node);
	}

}

template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::collect_vector(typename binary_search_tree<tkey, tvalue>::node **goat)
{
	std::vector<scapegoat_node*> vector;

	std::stack<typename binary_search_tree<tkey, tvalue>::node**> path;

	path.push(goat);

	while((*(path.top()))->left_subtree != nullptr) // begin infix
	{
		path.push(&((*path.top())->left_subtree));
	}

	typename binary_search_tree<tkey, tvalue>::infix_iterator it(path, new typename binary_search_tree<tkey, tvalue>::iterator_data);
	typename binary_search_tree<tkey, tvalue>::infix_iterator end(std::stack<typename binary_search_tree<tkey, tvalue>::node**>(), nullptr);

	for(; it != end; ++it)
	{
		vector.push_back(static_cast<scapegoat_node*>(*(it.get_node())));
	}

	auto start = vector.begin();
	auto finish = vector.end();

	rebuilding_tree(goat, start, finish);

}

template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::rebuilding_tree(typename binary_search_tree<tkey, tvalue>::node **new_top_node, std::vector<scapegoat_node*>::iterator begin, std::vector<scapegoat_node*>::iterator end)
{
	size_t size = std::distance(begin, end);

	if(size > 0)
	{
		auto median = begin + size / 2;

		*new_top_node = *median;

		(*new_top_node)->left_subtree = nullptr;
		(*new_top_node)->right_subtree = nullptr;

		rebuilding_tree(&((*new_top_node)->left_subtree), begin, median);
		rebuilding_tree(&((*new_top_node)->right_subtree), median + 1, end);

		static_cast<scapegoat_node*>(*new_top_node)->update_count_nodes();
	}

}

template<
	typename tkey,
	typename tvalue>
void scapegoat_tree<tkey, tvalue>::balance_afrer_new_alpha(typename binary_search_tree<tkey, tvalue>::node ** root)
{
	if(*root != nullptr)
	{
		bool need = static_cast<scapegoat_node*>(*root)->need_balance(_alpha);
		if(need)
		{
			collect_vector(root);
		}
		else
		{
			balance_afrer_new_alpha(&((*root)->left_subtree));
			balance_afrer_new_alpha(&((*root)->right_subtree));
		}
	}
}

template<
	typename tkey,
	typename tvalue>
tvalue scapegoat_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**> &path)
{

	tvalue res = (*path.top())->value;

	typename binary_search_tree<tkey, tvalue>::node* current_node = *path.top();

	if(current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
	{
		*path.top() = nullptr;
		path.pop();
	}
	else if(current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
	{
		typename binary_search_tree<tkey, tvalue>::node* update_node = current_node->right_subtree != nullptr ? (current_node->right_subtree) : (current_node->left_subtree);
		*path.top() = update_node;
	}
	else
	{
		std::queue<typename binary_search_tree<tkey, tvalue>::node**> que;

		typename binary_search_tree<tkey, tvalue>::node** update = &((*path.top())->left_subtree);


		while((*update)->right_subtree != nullptr)
		{
			update = &((*update)->right_subtree);
			que.push(update);
		}

		typename binary_search_tree<tkey, tvalue>::node* previous_node = *path.top();
		*(path.top()) = *update;
		*update = (*update)->left_subtree;

		(*path.top())->left_subtree = previous_node->left_subtree == *(path.top()) ? *update : previous_node->left_subtree;
		(*path.top())->right_subtree = previous_node->right_subtree;

		if(!que.empty())
		{
			path.push(&((*path.top())->left_subtree));
		}

		while(que.size() > 1)
		{
			path.push(que.front());
			que.pop();
		}

	}

	allocator::destruct(current_node);
	allocator_guardant::deallocate_with_guard(current_node);
	typename binary_search_tree<tkey, tvalue>::node** goat = nullptr;

	while(!path.empty())
	{
		static_cast<scapegoat_node*>(*path.top())->update_count_nodes();
		bool needBalance = static_cast<scapegoat_node*>(*path.top())->need_balance(_alpha);
		if(needBalance)
		{
			goat = path.top();
		}
		path.pop();
	}

	if(goat != nullptr)
	{
		collect_vector(goat);
	}

	return res;
}


#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SCAPEGOAT_TREE_H