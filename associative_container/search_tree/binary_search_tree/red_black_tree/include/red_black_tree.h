#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H

#include <binary_search_tree.h>
#include <queue>

template<
		typename tkey,
		typename tvalue>
class red_black_tree final:
		public binary_search_tree<tkey, tvalue>
{

public:

	enum class node_color
	{
		RED,
		BLACK
	};

private:

	struct node final:
			binary_search_tree<tkey, tvalue>::node
	{
	public:

		node_color _color;


	public:

		explicit  node(tkey const& key, tvalue const& value);

		explicit node(tkey const& key, tvalue&& value);

		~node() noexcept override = default;


	public:

		void change_color(node_color new_color)
		{
			_color = new_color;
		}

	};

public:

	struct iterator_data final:
			public binary_search_tree<tkey, tvalue>::iterator_data
	{

	public:

		node_color color;

	public:

		explicit iterator_data(
				unsigned int depth = 0,
				tkey const &key = tkey(),
				tvalue const &value = tvalue(),
				node_color color = node_color::RED);

		iterator_data(
				iterator_data const &other);

		iterator_data(
				iterator_data &&other) noexcept;

		iterator_data &operator=(
				iterator_data const &other);

		iterator_data &operator=(
				iterator_data &&other) noexcept;

		~iterator_data() noexcept final = default;

	public:

		void update_iterator_data(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override
		{
			if (!path.empty())
			{
				binary_search_tree<tkey, tvalue>::iterator_data::key = (*(path.top()))->key;
				binary_search_tree<tkey, tvalue>::iterator_data::value = (*(path.top()))->value;
				binary_search_tree<tkey, tvalue>::iterator_data::depth = path.size() - 1;
				color = static_cast<node *>(*(path.top()))->_color;
			}
		}

		iterator_data* clone_iterator_data() const override
		{
			return new iterator_data(*this);
		}

	};

public:

	explicit red_black_tree(
			std::function<int(tkey const &, tkey const &)> keys_comparer,
			allocator *allocator = nullptr,
			logger *logger = nullptr,
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

	~red_black_tree() noexcept final = default;

	red_black_tree(
			red_black_tree<tkey, tvalue> const &other);

	red_black_tree<tkey, tvalue> &operator=(
			red_black_tree<tkey, tvalue> const &other);

	red_black_tree(
			red_black_tree<tkey, tvalue> &&other) noexcept;

	red_black_tree<tkey, tvalue> &operator=(
			red_black_tree<tkey, tvalue> &&other) noexcept;

private:

	void insert_inside(const tkey& key, const tvalue& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	void insert_inside(const tkey& key, tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	template<typename rb_tvalue>
	void red_black_insert(const tkey& key, rb_tvalue&& value, std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

	tvalue dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path) override;

	typename binary_search_tree<tkey,tvalue>::iterator_data *create_iterator_data() const override;

	static inline node_color get_color(
			typename red_black_tree<tkey,tvalue>::node* node) noexcept;

	void remove_node_with_two_children(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path);

};

// region node implement
template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
		const tkey &key, const tvalue &value) :
		binary_search_tree<tkey, tvalue>::node(key, value) ,
		_color(node_color::RED)
{
}

template<typename tkey, typename tvalue>
red_black_tree<tkey, tvalue>::node::node(
		const tkey &key, tvalue&& value) :
		binary_search_tree<tkey, tvalue>::node(key, std::move(value)),
		_color(node_color::RED)
{
}
//endregion node

// region iterator_data

template<
		typename tkey,
		typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data const &other):
		binary_search_tree<tkey, tvalue>::iterator_data(other),
		color(other.color)
{ }

template<
		typename tkey,
		typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
		iterator_data &&other) noexcept:
		binary_search_tree<tkey, tvalue>::iterator_data(std::move(other)),
		color(std::move(other.color))
{ }

template<
		typename tkey,
		typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data const &other)
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::iterator_data::operator=(other);
		color = other.color;
	}

	return *this;
}

template<
		typename tkey,
		typename tvalue>
typename red_black_tree<tkey, tvalue>::iterator_data &red_black_tree<tkey, tvalue>::iterator_data::operator=(
		iterator_data &&other) noexcept
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::iterator_data::operator=(std::move(other));
		color = std::move(other.color);
	}

	return *this;
}


template<
		typename tkey,
		typename tvalue>
red_black_tree<tkey, tvalue>::iterator_data::iterator_data(
		unsigned int depth,
		tkey const &key,
		tvalue const &value,
		typename red_black_tree<tkey, tvalue>::node_color color):
		binary_search_tree<tkey, tvalue>::iterator_data(depth, key, value) , color(color)
{}

// endregion iterator_data

// region red_black_tree constructors
template<
		typename tkey,
		typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
		binary_search_tree<tkey, tvalue>(keys_comparer, allocator, logger, insertion_strategy, disposal_strategy) {}

template<
	typename tkey,
	typename tvalue>
red_black_tree<tkey, tvalue>::red_black_tree(
		red_black_tree<tkey, tvalue> const &other):
		binary_search_tree<tkey, tvalue>(other)
{}

template<
	typename tkey,
	typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
		red_black_tree<tkey, tvalue> const &other)
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
red_black_tree<tkey, tvalue>::red_black_tree(
		red_black_tree<tkey, tvalue> &&other) noexcept:
		binary_search_tree<tkey, tvalue>(std::move(other)){}

template<
	typename tkey,
	typename tvalue>
red_black_tree<tkey, tvalue> &red_black_tree<tkey, tvalue>::operator=(
		red_black_tree<tkey, tvalue> &&other) noexcept
{
	if (this != &other)
	{
		binary_search_tree<tkey, tvalue>::operator=(std::move(other));
	}

	return *this;
}

//endregion rbt constructors

// region rbt functions

template<
	typename tkey,
	typename tvalue>
typename red_black_tree<tkey, tvalue>::node_color red_black_tree<tkey, tvalue>::get_color(
		typename red_black_tree<tkey,tvalue>::node* node) noexcept
{
	if (node != nullptr) return node->_color;
	else return node_color::BLACK;
}


template<
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *red_black_tree<tkey, tvalue>::create_iterator_data() const
{
	return new iterator_data();
}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	red_black_insert(key, value, path);
}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue&& value,std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	red_black_insert(key, std::move(value), path);
}

template<typename tkey, typename tvalue>
void red_black_tree<tkey, tvalue>::remove_node_with_two_children(std::stack<typename binary_search_tree<tkey, tvalue>::node **> &path)
{
	node * current_node = static_cast<node*> (*path.top());
	std::queue<typename binary_search_tree<tkey, tvalue>::node **> que;
	typename binary_search_tree<tkey, tvalue>::node **update = &(current_node->left_subtree);

	while ((*update)->right_subtree != nullptr) // находим самый правый узел в левом поддереве
	{
		update = &((*update)->right_subtree);
		que.push(update);
	}

	node_color removable_color = static_cast<node *>(*update)->_color;
	typename binary_search_tree<tkey, tvalue>::node *previous_node = *path.top();

	*(path.top()) = *update; // обновляем указатель в пути на найденный узел и его цвет
	static_cast<node *>(*path.top())->_color = node_color::RED;
	typename binary_search_tree<tkey, tvalue>::node *leftist = (*update)->left_subtree; // cохраняем левое поддерево найденного узла

	*update = previous_node;
	static_cast<node *>(*update)->change_color(removable_color);
	(*path.top())->left_subtree = previous_node->left_subtree == *(path.top()) ? *update : previous_node->left_subtree;
	(*path.top())->right_subtree = previous_node->right_subtree;
	(*update)->right_subtree = nullptr;
	(*update)->left_subtree = leftist;

	path.push(&((*path.top())->left_subtree)); // для последующей обработке

	while (!que.empty())
	{
		path.push(que.front());
		que.pop();
	}
}

template<typename tkey, typename tvalue>
template<typename rb_tvalue>
void red_black_tree<tkey, tvalue>::red_black_insert(const tkey &key, rb_tvalue &&value, std::stack<typename binary_search_tree<tkey, tvalue>::node**> & path)
{
	(*path.top()) = static_cast<typename binary_search_tree<tkey, tvalue>::node *>(static_cast<node *>(allocator_guardant::allocate_with_guard(
			sizeof(node), 1)));

	try {
		allocator::construct(*path.top(), key, std::forward<rb_tvalue>(value));
	}
	catch (std::bad_alloc & ex) {
		allocator_guardant::deallocate_with_guard(*path.top());
		throw ex;
	}

	node *current_node = static_cast<node *>(*path.top());

	path.pop();

	if (path.empty())
	{
		current_node->change_color(node_color::BLACK); // 1 случай - это корень --> перекрашиваем в черный
		return;
	}

	while (get_color(static_cast<node*>(*path.top())) == node_color::RED) // начинаем с родителя
	{
		node *parent = static_cast<node *>(*path.top());
		path.pop();

		if (path.empty()) return;

		node *grandparent = static_cast<node *>(*path.top());
		node *uncle = (parent == grandparent->left_subtree) ? static_cast<node *>(grandparent->right_subtree) : static_cast<node *>(grandparent->left_subtree);

		if (parent == grandparent->left_subtree)
		{
			if (get_color(uncle) == node_color::RED) // 2 случай - дядя узла красный --> отец и дядя - черные; дедушка - красный
			{
				parent->change_color(node_color::BLACK);
				uncle->change_color(node_color::BLACK);
				if (path.size() != 1) // если дед - то не корень
					grandparent->change_color(node_color::RED);
				break;
			}
			else // если дядя черный
			{
				if (current_node == parent->right_subtree)
				{
					parent = current_node;
					binary_search_tree<tkey, tvalue>::small_left_rotation(grandparent->left_subtree); // делаем линию из двух красных
				}
				parent->change_color(node_color::BLACK);
				grandparent->change_color(node_color::RED);
				binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top());
			}
		}
		else
		{
			if (get_color(uncle) == node_color::RED) // 2 случай - дядя узла красный --> отец и дядя - черные; дедушка - красный
			{
				parent->change_color(node_color::BLACK);
				uncle->change_color(node_color::BLACK);
				if (path.size() != 1) // если дед - то не корень
					grandparent->change_color(node_color::RED);
				break;
			}
			else  // если дядя черный
			{
				if (current_node == parent->left_subtree)
				{
					parent = current_node;
					binary_search_tree<tkey, tvalue>::small_right_rotation(grandparent->right_subtree); // делаем линию из двух красных
				}
				parent->change_color(node_color::BLACK);
				grandparent->change_color(node_color::RED);
				binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top());
			}
		}
		if (path.size() == 1)
		{
			static_cast<node *>(*path.top())->change_color(node_color::BLACK); // корень черный
		}
	}
}

template<
	typename tkey,
	typename tvalue>
tvalue red_black_tree<tkey, tvalue>::dispose_inside(std::stack<typename binary_search_tree<tkey, tvalue>::node**>& path)
{
	tvalue res = (*path.top())->value;
	typename binary_search_tree<tkey, tvalue>::node *current_node = *path.top();
	bool need_delete = true, need_continue = true;

	while (!path.empty() && need_continue)
	{
		if (static_cast<node *>(current_node)->_color == node_color::RED)
		{
			if (!current_node->left_subtree && !current_node->right_subtree) // красный и лист
			{
				*path.top() = nullptr;

				if (need_delete)
				{
					allocator::destruct(current_node);
					allocator_guardant::deallocate_with_guard(current_node);
				}
				break;
			}

			if (current_node->right_subtree && current_node->left_subtree)
			{
				remove_node_with_two_children(path);
				continue;
			}
		}
		else if (static_cast<node *>(current_node)->_color == node_color::BLACK) //если черный (сложно)
		{
			if ((*path.top())->left_subtree != nullptr && (*path.top())->right_subtree != nullptr)
			{
				remove_node_with_two_children(path);
				continue;
			}
			else if (current_node->left_subtree != nullptr || current_node->right_subtree != nullptr)
			{
				if (current_node->left_subtree != nullptr && current_node->right_subtree == nullptr) // если только левый
				{
					typename binary_search_tree<tkey, tvalue>::node **update = &(current_node->left_subtree);
					typename binary_search_tree<tkey, tvalue>::node *previous_node = current_node;

					*(path.top()) = *update;
					static_cast<node *>(*path.top())->_color = node_color::BLACK;
					typename binary_search_tree<tkey, tvalue>::node *leftist = (*update)->left_subtree;
					typename binary_search_tree<tkey, tvalue>::node *rightist = (*update)->right_subtree;

					// меняем местами отца с сыном и перекрашиваем их в противоположные цвета
					*update = previous_node;
					static_cast<node *>(*update)->change_color(node_color::RED);
					(*path.top())->left_subtree = *update;
					(*path.top())->right_subtree = nullptr;
					((*path.top())->left_subtree)->right_subtree = rightist;
					((*path.top())->left_subtree)->left_subtree = leftist;

					path.push(&((*path.top())->left_subtree));

					continue;
				}
				else if (current_node->right_subtree != nullptr && current_node->left_subtree == nullptr) // если только правый
				{
					typename binary_search_tree<tkey, tvalue>::node **update = &(current_node->right_subtree);
					typename binary_search_tree<tkey, tvalue>::node *previous_node = current_node;

					*(path.top()) = *update;
					static_cast<node *>(*path.top())->_color = node_color::BLACK;
					typename binary_search_tree<tkey, tvalue>::node *leftist = (*update)->left_subtree;
					typename binary_search_tree<tkey, tvalue>::node *rightist = (*update)->right_subtree;

					*update = previous_node;
					static_cast<node *>(*update)->change_color(node_color::RED);
					(*path.top())->right_subtree = *update;
					(*path.top())->left_subtree = nullptr;
					((*path.top())->right_subtree)->right_subtree = rightist;
					((*path.top())->right_subtree)->left_subtree = leftist;

					path.push(&((*path.top())->right_subtree));

					continue;
				}
			}
			else if (current_node->left_subtree == nullptr && current_node->right_subtree == nullptr) // если черный и лист
			{
				if (path.size() == 1) // Если текущий узел является корнем дерева
				{
					*path.top() = nullptr;
					if (need_delete)
					{
						allocator::destruct(current_node);
						allocator_guardant::deallocate_with_guard(current_node);
					}
					break;
				}

				while (path.size() > 1 && static_cast<node *>(*path.top())->_color == node_color::BLACK) // Пока мы не достигли корня и текущий узел черный
				{
					typename binary_search_tree<tkey, tvalue>::node *current = *path.top();
					path.pop();

					typename binary_search_tree<tkey, tvalue>::node *parent;
					typename binary_search_tree<tkey, tvalue>::node *brother;

					if (!path.empty())
					{
						parent = *path.top();
					}
					else
					{
						if (need_delete)
						{
							allocator::destruct(current);
							allocator_guardant::deallocate_with_guard(current);
						}
						need_delete = false;
						break;
					}

					if (parent->left_subtree == current) // Если текущий узел является левым потомком родителя
					{
						if (static_cast<node *>(parent->right_subtree)->_color == node_color::RED) // Если правый потомок родителя красный
						{
							binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top());

							// Меняем цвета узлов для соблюдения свойств красно-черного дерева
							static_cast<node *>(*path.top())->change_color(node_color::BLACK);
							static_cast<node *>((*path.top())->left_subtree)->change_color(node_color::RED);
							path.push(&((*path.top())->left_subtree));
							typename binary_search_tree<tkey, tvalue>::node *removable = (*path.top())->left_subtree;
							path.push(&(removable));

							continue;
						}
						else if (static_cast<node *>(parent->right_subtree)->_color == node_color::BLACK) // Если правый потомок родителя черный
						{
							brother = parent->right_subtree; // Устанавливаем брата текущего узла


							if ((brother->right_subtree == nullptr && brother->left_subtree == nullptr) ||
								(brother->right_subtree != nullptr &&
								 static_cast<node *>(brother->right_subtree)->_color == node_color::BLACK &&
								 brother->left_subtree != nullptr &&
								 static_cast<node *>(brother->left_subtree)->_color == node_color::BLACK))
							{
								if (need_delete)
								{
									(*path.top())->left_subtree = nullptr;
									allocator::destruct(current);
									allocator_guardant::deallocate_with_guard(current);
									need_delete = false;
								}

								static_cast<node *>(brother)->change_color(node_color::RED); // Меняем цвет брата на красный
								node_color color_of_parent = static_cast<node *>(parent)->_color; // Цвет родителя
								static_cast<node *>(parent)->change_color(node_color::BLACK); // Меняем цвет родителя на черный
								if (color_of_parent == node_color::BLACK) // Если цвет родителя черный
								{
									continue;
								}
								else
								{
									break;
								}
							}
							else if (brother->right_subtree != nullptr &&
									 static_cast<node *>(brother->right_subtree)->_color == node_color::RED) // Если правый потомок брата красный
							{
								if (need_delete)
								{
									(*path.top())->left_subtree = nullptr;
									allocator::destruct(current);
									allocator_guardant::deallocate_with_guard(current);
									need_delete = false;
								}

								node_color parent_color = static_cast<node *>(parent)->_color;
								binary_search_tree<tkey, tvalue>::small_left_rotation(*path.top());
								static_cast<node *>(*path.top())->change_color(parent_color); // Меняем цвет родителя на его первоначальный цвет
								static_cast<node *>((*path.top())->right_subtree)->change_color(node_color::BLACK); // Меняем цвет правого потомка родителя на черный
								static_cast<node *>((*path.top())->left_subtree)->change_color(node_color::BLACK); // Меняем цвет левого потомка родителя на черный
								break;
							}
							else if (static_cast<node *>(brother->left_subtree)->_color == node_color::RED &&
									 (brother->right_subtree == nullptr ||
									  static_cast<node *>(brother->right_subtree)->_color == node_color::BLACK)) // Если левый потомок брата красный и правый потомок черный
							{
								binary_search_tree<tkey, tvalue>::small_right_rotation((*path.top())->right_subtree);
								static_cast<node *>((*path.top())->right_subtree)->change_color(node_color::BLACK); // Меняем цвет правого потомка родителя на черный
								static_cast<node *>((*path.top())->right_subtree->right_subtree)->change_color(node_color::RED); // Меняем цвет правого потомка правого потомка родителя на красный
								path.push(&((*path.top())->left_subtree));
								continue;
							}
						}
					}
					else if (parent->right_subtree == current) // Если текущий узел является правым потомком родителя
					{
						if (static_cast<node *>(parent->left_subtree)->_color == node_color::RED) // Если левый потомок родителя красный
						{
							binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top());
							static_cast<node *>(*path.top())->change_color(node_color::BLACK);
							static_cast<node *>((*path.top())->right_subtree)->change_color(node_color::RED); // Меняем цвет правого потомка родителя на красный
							path.push(&((*path.top())->right_subtree));
							typename binary_search_tree<tkey, tvalue>::node *removable = (*path.top())->right_subtree;
							path.push(&(removable));
							continue;
						}
						else if (static_cast<node *>(parent->left_subtree)->_color == node_color::BLACK) // Если левый потомок родителя черный
						{
							brother = parent->left_subtree;


							if ((brother->left_subtree == nullptr && brother->right_subtree == nullptr) ||
								(brother->left_subtree != nullptr &&
								 static_cast<node *>(brother->left_subtree)->_color == node_color::BLACK &&
								 brother->right_subtree != nullptr &&
								 static_cast<node *>(brother->right_subtree)->_color == node_color::BLACK))
							{
								if (need_delete)
								{
									(*path.top())->right_subtree = nullptr;
									allocator::destruct(current);
									allocator_guardant::deallocate_with_guard(current);
									need_delete = false;
								}

								static_cast<node *>(brother)->change_color(node_color::RED); // Меняем цвет брата на красный
								node_color color_of_parent = static_cast<node *>(parent)->_color;
								static_cast<node *>(parent)->change_color(node_color::BLACK); // Меняем цвет родителя на черный
								if (color_of_parent == node_color::BLACK) // Если цвет родителя черный
								{
									continue;
								}
								else
								{
									break;
								}
							}
							else if (brother->left_subtree != nullptr &&
									 static_cast<node *>(brother->left_subtree)->_color == node_color::RED) // Если левый потомок брата красный
							{
								if (need_delete)
								{
									(*path.top())->right_subtree = nullptr;
									allocator::destruct(current);
									allocator_guardant::deallocate_with_guard(current);
									need_delete = false;
								}

								node_color parent_color = static_cast<node *>(parent)->_color;
								binary_search_tree<tkey, tvalue>::small_right_rotation(*path.top());
								static_cast<node *>(*path.top())->change_color(parent_color); // Меняем цвет родителя на его первоначальный цвет
								static_cast<node *>((*path.top())->left_subtree)->change_color(node_color::BLACK); // Меняем цвет левого потомка родителя на черный
								static_cast<node *>((*path.top())->right_subtree)->change_color(node_color::BLACK); // Меняем цвет правого потомка родителя на черный
								break;
							}
							else if (static_cast<node *>(brother->right_subtree)->_color == node_color::RED &&
									 (brother->left_subtree == nullptr ||
									  static_cast<node *>(brother->left_subtree)->_color == node_color::BLACK)) // Если правый потомок брата красный и левый потомок черный
							{
								binary_search_tree<tkey, tvalue>::small_left_rotation((*path.top())->left_subtree);
								static_cast<node *>((*path.top())->left_subtree)->change_color(node_color::BLACK); // Меняем цвет левого потомка родителя на черный
								static_cast<node *>((*path.top())->left_subtree->left_subtree)->change_color(node_color::RED); // Меняем цвет левого потомка левого потомка родителя на красный
								path.push(&((*path.top())->right_subtree));
								continue;
							}
						}
					}
				}
				need_continue = false; // Устанавливаем флаг продолжения в false
			}
		}
	}
	return res;

}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_RED_BLACK_TREE_H