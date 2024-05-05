#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <logger.h>
#include <logger_guardant.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <memory>


template<
    typename tkey,
    typename tvalue>
class binary_search_tree:
    public search_tree<tkey, tvalue>
{

protected:

    struct node
    {

        tkey key;

        tvalue value;

        node *left_subtree;

        node *right_subtree;

    public:

        explicit node(
            tkey const &key,
            tvalue const &value);

        explicit node(
            tkey const &key,
            tvalue &&value);

    };

public:

    // region iterators definition

    struct iterator_data
    {

    public:

        unsigned int depth;

        tkey key;

        tvalue value;

    public:

        explicit iterator_data(
            unsigned int depth = 0,
            tkey const &key = tkey(),
            tvalue const &value = tvalue());

		virtual void update_iterator(const node* node, unsigned int _depth);

		virtual iterator_data* create_deep_copy_of_iterator_data();

    };

    class prefix_iterator
    {

    public:

		explicit prefix_iterator(const std::stack<node**>& path, iterator_data* it_data = nullptr);

		prefix_iterator(const prefix_iterator& other);

		prefix_iterator(prefix_iterator&& other) noexcept = default;

		prefix_iterator& operator=(const prefix_iterator& other);

		prefix_iterator& operator=(prefix_iterator&& other) noexcept = default;

		virtual ~prefix_iterator() = default;


	public:

        bool operator==(
            prefix_iterator const &other) const noexcept;

        bool operator!=(
            prefix_iterator const &other) const noexcept;

        prefix_iterator &operator++();

        prefix_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data = std::make_unique<iterator_data>();
		std::stack<node**> _path;
    };

    class prefix_const_iterator
    {

    public:

		explicit prefix_const_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		prefix_const_iterator(const prefix_const_iterator& other);

		prefix_const_iterator(prefix_const_iterator&& other) noexcept = default;

		prefix_const_iterator& operator=(const prefix_const_iterator& other);

		prefix_const_iterator& operator=(prefix_const_iterator&& other) noexcept = default;

		virtual ~prefix_const_iterator() = default;

    public:

        bool operator==(
            prefix_const_iterator const &other) const noexcept;

        bool operator!=(
            prefix_const_iterator const &other) const noexcept;

        prefix_const_iterator &operator++();

        prefix_const_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data = std::make_unique<iterator_data>();
		std::stack<node**> _path;
    };

    class prefix_reverse_iterator
    {

    public:

		explicit prefix_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		prefix_reverse_iterator(const prefix_reverse_iterator& other);

		prefix_reverse_iterator(prefix_reverse_iterator&& other) noexcept = default;

		prefix_reverse_iterator& operator=(const prefix_reverse_iterator& other);

		prefix_reverse_iterator& operator=(prefix_reverse_iterator&& other) noexcept = default;

		virtual ~prefix_reverse_iterator() = default;


	public:

        bool operator==(
            prefix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            prefix_reverse_iterator const &other) const noexcept;

        prefix_reverse_iterator &operator++();

        prefix_reverse_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data = std::make_unique<iterator_data>();
		std::stack<node**> _path;

    };

    class prefix_const_reverse_iterator
    {

    public:

		explicit prefix_const_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		prefix_const_reverse_iterator(const prefix_const_reverse_iterator& other);

		prefix_const_reverse_iterator(prefix_const_reverse_iterator&& other) noexcept = default;

		prefix_const_reverse_iterator& operator=(const prefix_const_reverse_iterator& other);

		prefix_const_reverse_iterator& operator=(prefix_const_reverse_iterator&& other) noexcept = default;

		virtual ~prefix_const_reverse_iterator() = default;

    public:

        bool operator==(
            prefix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            prefix_const_reverse_iterator const &other) const noexcept;

        prefix_const_reverse_iterator &operator++();

        prefix_const_reverse_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;

    };

    class infix_iterator
    {

    public:

		explicit infix_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		infix_iterator(const infix_iterator& other);

		infix_iterator(infix_iterator&& other) noexcept = default;

		infix_iterator& operator=(const infix_iterator& other);

		infix_iterator& operator=(infix_iterator&& other) noexcept = default;

		virtual ~infix_iterator() = default;

    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;

    };

    class infix_const_iterator
    {

    public:

		explicit infix_const_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		infix_const_iterator(const infix_const_iterator& other);

		infix_const_iterator(infix_const_iterator&& other) noexcept = default;

		infix_const_iterator& operator=(const infix_const_iterator& other);

		infix_const_iterator& operator=(infix_const_iterator&& other) noexcept = default;

		virtual ~infix_const_iterator() = default;


	public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;

    };

    class infix_reverse_iterator
    {

    public:

		explicit infix_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		infix_reverse_iterator(const infix_reverse_iterator& other);

		infix_reverse_iterator(infix_reverse_iterator&& other) noexcept = default;

		infix_reverse_iterator& operator=(const infix_reverse_iterator& other);

		infix_reverse_iterator& operator=(infix_reverse_iterator&& other) noexcept = default;

		virtual ~infix_reverse_iterator() = default;

    public:

        bool operator==(
            infix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            infix_reverse_iterator const &other) const noexcept;

        infix_reverse_iterator &operator++();

        infix_reverse_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;
	};

    class infix_const_reverse_iterator
    {

    public:

		explicit infix_const_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		infix_const_reverse_iterator(const infix_const_reverse_iterator& oth);

		infix_const_reverse_iterator(infix_const_reverse_iterator&& oth) noexcept = default;

		infix_const_reverse_iterator& operator=(const infix_const_reverse_iterator& oth);

		infix_const_reverse_iterator& operator=(infix_const_reverse_iterator&& oth) noexcept = default;

		virtual ~infix_const_reverse_iterator() = default;


	public:

        bool operator==(
            infix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_reverse_iterator const &other) const noexcept;

        infix_const_reverse_iterator &operator++();

        infix_const_reverse_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;
    };

    class postfix_iterator
    {

    public:

		explicit postfix_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		postfix_iterator(const postfix_iterator& oth);

		postfix_iterator(postfix_iterator&& oth) noexcept = default;

		postfix_iterator& operator=(const postfix_iterator& oth);

		postfix_iterator& operator=(postfix_iterator&& oth) noexcept = default;

		virtual ~postfix_iterator() = default;

    public:

        bool operator==(
            postfix_iterator const &other) const noexcept;

        bool operator!=(
            postfix_iterator const &other) const noexcept;

        postfix_iterator &operator++();

        postfix_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;

    };

    class postfix_const_iterator
    {

    public:

        explicit postfix_const_iterator(const std::stack<node**> & path = std::stack<node**>(), iterator_data * data = nullptr);

		postfix_const_iterator(const postfix_const_iterator& other);

		postfix_const_iterator(postfix_const_iterator&& other) noexcept = default;

		postfix_const_iterator& operator=(const postfix_const_iterator& other);

		postfix_const_iterator& operator=(postfix_const_iterator&& other) noexcept = default;

		virtual ~postfix_const_iterator() = default;

    public:

        bool operator==(
            postfix_const_iterator const &other) const noexcept;

        bool operator!=(
            postfix_const_iterator const &other) const noexcept;

        postfix_const_iterator &operator++();

        postfix_const_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;

    };

    class postfix_reverse_iterator
    {

    public:

		explicit postfix_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		postfix_reverse_iterator(const postfix_reverse_iterator& other);

		postfix_reverse_iterator(postfix_reverse_iterator&& other) noexcept = default;

		postfix_reverse_iterator& operator=(const postfix_reverse_iterator& other);

		postfix_reverse_iterator& operator=(postfix_reverse_iterator&& other) noexcept = default;

		virtual ~postfix_reverse_iterator() = default;

	public:

        bool operator==(
            postfix_reverse_iterator const &other) const noexcept;

        bool operator!=(
            postfix_reverse_iterator const &other) const noexcept;

        postfix_reverse_iterator &operator++();

        postfix_reverse_iterator const operator++(
            int unused);

        iterator_data *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;
    };

    class postfix_const_reverse_iterator
    {

    public:

		explicit postfix_const_reverse_iterator(const std::stack<node**>& path = std::stack<node**>(), iterator_data* data = nullptr);

		postfix_const_reverse_iterator(const postfix_const_reverse_iterator& other);

		postfix_const_reverse_iterator(postfix_const_reverse_iterator&& other) noexcept = default;

		postfix_const_reverse_iterator& operator=(const postfix_const_reverse_iterator& other);

		postfix_const_reverse_iterator& operator=(postfix_const_reverse_iterator&& other) noexcept = default;

		virtual ~postfix_const_reverse_iterator() = default;

	public:

        bool operator==(
            postfix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
            postfix_const_reverse_iterator const &other) const noexcept;

        postfix_const_reverse_iterator &operator++();

        postfix_const_reverse_iterator const operator++(
            int unused);

        iterator_data const *operator*() const;

	protected:

		std::unique_ptr<iterator_data> _data;
		std::stack<node**> _path;
    };

	// для других

	friend class prefix_iterator;
	friend class prefix_const_iterator;
	friend class prefix_reverse_iterator;
	friend class prefix_const_reverse_iterator;

	friend class postfix_iterator;
	friend class postfix_const_iterator;
	friend class postfix_reverse_iterator;
	friend class postfix_const_reverse_iterator;

	friend class infix_iterator;
	friend class infix_const_iterator;
	friend class infix_reverse_iterator;
	friend class infix_const_reverse_iterator;

    // endregion iterators definition

protected:

    // region target operations strategies definition

    enum class insertion_of_existent_key_attempt_strategy
    {
        update_value,
        throw_an_exception
    };

    enum class disposal_of_nonexistent_key_attempt_strategy
    {
        do_nothing,
        throw_an_exception
    };

    // endregion target operations strategies definition

    // region target operations associated exception types

    class insertion_of_existent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit insertion_of_existent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {

    private:

        tkey _key;

    public:

        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);

    public:

        tkey const &get_key() const noexcept;

    };

    // endregion target operations associated exception types

    // region template methods definition

    class template_method_basics:
        public logger_guardant
    {

    private:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit template_method_basics(
            binary_search_tree<tkey, tvalue> *tree);

    protected:

        [[nodiscard]] logger *get_logger() const noexcept final;

    };

    class insertion_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit insertion_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy);

    public:

        void insert(
            tkey const &key,
            tvalue const &value);

        void insert(
            tkey const &key,
            tvalue &&value);

        void set_insertion_strategy(
            typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    class obtaining_template_method:
        public template_method_basics
    {

    private:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit obtaining_template_method(
            binary_search_tree<tkey, tvalue> *tree);

    public:

        tvalue const &obtain(
            tkey const &key);

    };

    class disposal_template_method:
        public template_method_basics,
        public allocator_guardant
    {

    private:

        binary_search_tree<tkey, tvalue> *_tree;

    public:

        explicit disposal_template_method(
            binary_search_tree<tkey, tvalue> *tree,
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy);

    public:

        tvalue dispose(
            tkey const &key);

        void set_disposal_strategy(
            typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;


    private:

        [[nodiscard]] allocator *get_allocator() const noexcept final;

    };

    // endregion template methods definition

protected:

	mutable node *_root;

	insertion_of_existent_key_attempt_strategy _insert_exist_strategy;

	disposal_of_nonexistent_key_attempt_strategy _dispose_nonexist_strategy;

public:

	explicit binary_search_tree(
			std::function<int(tkey const &, tkey const &)> comparer,
			allocator *allocator = nullptr,
			logger *logger = nullptr,
			typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy = binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy::throw_an_exception,
			typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy = binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy::throw_an_exception);

public:

    binary_search_tree(
        binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree(
        binary_search_tree<tkey, tvalue> &&other) noexcept;

    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> const &other);

    binary_search_tree<tkey, tvalue> &operator=(
        binary_search_tree<tkey, tvalue> &&other) noexcept;

    ~binary_search_tree() override;

public:

    void insert(
        tkey const &key,
        tvalue const &value) final;

    void insert(
        tkey const &key,
        tvalue &&value) final;

    tvalue const &obtain(
        tkey const &key) final;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) final;

    tvalue dispose(
        tkey const &key) final;

public:

    void set_insertion_strategy(
        typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept;

    void set_removal_strategy(
        typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept;

public:

    // region iterators requests definition

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    prefix_const_iterator cbegin_prefix() const noexcept;

    prefix_const_iterator cend_prefix() const noexcept;

    prefix_reverse_iterator rbegin_prefix() const noexcept;

    prefix_reverse_iterator rend_prefix() const noexcept;

    prefix_const_reverse_iterator crbegin_prefix() const noexcept;

    prefix_const_reverse_iterator crend_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() const noexcept;

    infix_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

    postfix_const_iterator cbegin_postfix() const noexcept;

    postfix_const_iterator cend_postfix() const noexcept;

    postfix_reverse_iterator rbegin_postfix() const noexcept;

    postfix_reverse_iterator rend_postfix() const noexcept;

    postfix_const_reverse_iterator crbegin_postfix() const noexcept;

    postfix_const_reverse_iterator crend_postfix() const noexcept;

    // endregion iterators requests definition

protected:

	std::stack<node**> find_path_to_crnt_node(const tkey&);

	virtual void insert_inside(const tkey& key, const tvalue& value, std::stack<node**>& path);

	virtual void insert_inside(const tkey& key, tvalue&& value, std::stack<node**>& path);

	virtual tvalue const& obtain_inside(std::stack<node**>& path);

	virtual tvalue dispose_inside(std::stack<node**>& path);

	virtual void copy_subtree(node** dest, node* src);

	virtual void destroy_subtree(node* src) noexcept;

protected:

    // region subtree rotations definition

	void small_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;

	void small_right_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;

	void big_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;

	void big_right_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root ) const;

	void double_left_rotation( typename binary_search_tree<tkey, tvalue>::node*& subtree_root) const;

	void double_right_rotation( typename binary_search_tree<tkey, tvalue>::node *&subtree_root ) const;

	// endregion subtree rotations definition

};

// region binary_search_tree<tkey, tvalue>::node methods implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue const &value) : key(key), value(value), left_subtree(nullptr), right_subtree(nullptr) {}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::node::node(
    tkey const &key,
    tvalue &&value) : key(key), value(std::move(value)), left_subtree(nullptr), right_subtree(nullptr) {}

// endregion binary_search_tree<tkey, tvalue>::node methods implementation

// region iterators implementation

// region iterator data implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::iterator_data::iterator_data(
    unsigned int depth,
    tkey const &key,
    tvalue const &value): depth(depth), key(key), value(value) {}

// endregion iterator data implementation

// region iterator implementation

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::iterator_data::update_iterator(const binary_search_tree::node *node, unsigned int _depth)
{
	key = node->key;
	value = node->value;
	depth = _depth;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data* binary_search_tree<tkey, tvalue>::iterator_data::create_deep_copy_of_iterator_data()
{
	return new iterator_data(*this);
}

// region prefix_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const std::stack<node**>& path, iterator_data* data) : _path(path), _data(data)
{
	if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
}

template<
	typename tkey,
	typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_iterator::prefix_iterator(const prefix_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
	typename tkey,
	typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator& binary_search_tree<tkey, tvalue>::prefix_iterator::operator=(const binary_search_tree::prefix_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
	typename tkey,
	typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator &binary_search_tree<tkey, tvalue>::prefix_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->left_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->left_subtree));
	}
	else if((*(_path.top()))->right_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->right_subtree));
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		while(!_path.empty())
		{
			if((*(_path.top()))->left_subtree == current_node && (*(_path.top()))->right_subtree == nullptr)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}

			if((*(_path.top()))->left_subtree == current_node && (*(_path.top()))->right_subtree != nullptr)
			{
				_path.push(&((*(_path.top()))->right_subtree));
				break;
			}

			if((*(_path.top()))->right_subtree == current_node)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator const binary_search_tree<tkey, tvalue>::prefix_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_iterator::operator*() const
{
	return _data.get();
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_iterator::prefix_const_iterator(const prefix_const_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator& binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator=(const binary_search_tree::prefix_const_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator &binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->left_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->left_subtree));
	}
	else if((*(_path.top()))->right_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->right_subtree));
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		while(!_path.empty())
		{
			if((*(_path.top()))->left_subtree == current_node && (*(_path.top()))->right_subtree == nullptr)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}

			if((*(_path.top()))->left_subtree == current_node && (*(_path.top()))->right_subtree != nullptr)
			{
				_path.push(&((*(_path.top()))->right_subtree));
				break;
			}

			if((*(_path.top()))->right_subtree == current_node)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator const binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_iterator::operator*() const
{
	return _data.get();
}

// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::prefix_reverse_iterator(const prefix_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator=(const binary_search_tree::prefix_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->right_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->right_subtree));
	}
	else if((*(_path.top()))->left_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->left_subtree));
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		while(!_path.empty())
		{
			if((*(_path.top()))->right_subtree == current_node && (*(_path.top()))->left_subtree == nullptr)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}

			if((*(_path.top()))->right_subtree == current_node && (*(_path.top()))->left_subtree != nullptr)
			{
				_path.push(&((*(_path.top()))->left_subtree));
				break;
			}

			if((*(_path.top()))->left_subtree == current_node)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::prefix_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const prefix_const_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator=(const binary_search_tree::prefix_const_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const &other) const noexcept
{
	return !(*this == other);;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->right_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->right_subtree));
	}
	else if((*(_path.top()))->left_subtree != nullptr)
	{
		_path.push(&((*(_path.top()))->left_subtree));
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		while(!_path.empty())
		{
			if((*(_path.top()))->right_subtree == current_node && (*(_path.top()))->left_subtree == nullptr)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}

			if((*(_path.top()))->right_subtree == current_node && (*(_path.top()))->left_subtree != nullptr)
			{
				_path.push(&((*(_path.top()))->left_subtree));
				break;
			}

			if((*(_path.top()))->left_subtree == current_node)
			{
				current_node = *(_path.top());
				_path.pop();
				continue;
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_iterator::infix_iterator(const infix_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator& binary_search_tree<tkey, tvalue>::infix_iterator::operator=(const binary_search_tree::infix_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator &binary_search_tree<tkey, tvalue>::infix_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->right_subtree != nullptr)
	{
		node** current_node = &((*(_path.top()))->right_subtree);
		while(*current_node != nullptr)
		{
			_path.push(current_node);
			current_node = &((*current_node)->left_subtree);
		}
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		if(!_path.empty() && (*(_path.top()))->left_subtree == current_node) {}
		else
		{
			while(!_path.empty() && (*(_path.top()))->left_subtree != current_node)
			{
				current_node = *(_path.top());
				_path.pop();
			}
		}
	}


	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator const binary_search_tree<tkey, tvalue>::infix_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_iterator::operator*() const
{
	return _data.get();
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(const infix_const_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator& binary_search_tree<tkey, tvalue>::infix_const_iterator::operator=(const binary_search_tree::infix_const_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator &binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->right_subtree != nullptr)
	{
		node** current_node = &((*(_path.top()))->right_subtree);
		while(*current_node != nullptr)
		{
			_path.push(current_node);
			current_node = &((*current_node)->left_subtree);
		}
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		if(!_path.empty() && (*(_path.top()))->left_subtree == current_node) {}
		else
		{
			while(!_path.empty() && (*(_path.top()))->left_subtree != current_node)
			{
				current_node = *(_path.top());
				_path.pop();
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator const binary_search_tree<tkey, tvalue>::infix_const_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
	return _data.get();
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(const infix_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator=(const binary_search_tree::infix_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->left_subtree != nullptr)
	{
		node** current_node = &((*(_path.top()))->left_subtree);
		while(*current_node != nullptr)
		{
			_path.push(current_node);
			current_node = &((*current_node)->right_subtree);
		}
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		if(!_path.empty() && (*(_path.top()))->right_subtree == current_node) {}
		else
		{
			while(!_path.empty() && (*(_path.top()))->right_subtree != current_node)
			{
				current_node = *(_path.top());
				_path.pop();
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(const infix_const_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator=(const binary_search_tree::infix_const_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
	if(_path.empty())
	{
		return *this;
	}

	if((*(_path.top()))->left_subtree != nullptr)
	{
		node** current_node = &((*(_path.top()))->left_subtree);
		while(*current_node != nullptr)
		{
			_path.push(current_node);
			current_node = &((*current_node)->right_subtree);
		}
	}
	else
	{
		node* current_node = *_path.top();
		_path.pop();

		if(!_path.empty() && (*(_path.top()))->right_subtree == current_node) {}
		else
		{
			while(!_path.empty() && (*(_path.top()))->right_subtree != current_node)
			{
				current_node = *(_path.top());
				_path.pop();
			}
		}
	}

	if (!_path.empty())
		_data->update_iterator(*_path.top(), _path.size() - 1);

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_iterator::postfix_iterator(const postfix_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator& binary_search_tree<tkey, tvalue>::postfix_iterator::operator=(const binary_search_tree::postfix_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator &binary_search_tree<tkey, tvalue>::postfix_iterator::operator++()
{
	node* current_node = *(_path.top());
	_path.pop();

	if(!_path.empty())
	{
		if((*(_path.top()))->left_subtree == current_node)
		{
			if((*(_path.top()))->right_subtree != nullptr)
			{
				_path.push(&((*_path.top())->right_subtree));

				while((*_path.top())->left_subtree != nullptr)
				{
					_path.push(&((*_path.top())->left_subtree));
				}
			}
		}

		if (!_path.empty())
			_data->update_iterator(*_path.top(), _path.size() - 1);
	}

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator const binary_search_tree<tkey, tvalue>::postfix_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_iterator::operator*() const
{
	return _data.get();
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_iterator::postfix_const_iterator(const postfix_const_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator& binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator=(const binary_search_tree::postfix_const_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator &binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++()
{
	node* current_node = *(_path.top());
	_path.pop();

	if(!_path.empty())
	{
		if((*(_path.top()))->left_subtree == current_node)
		{
			if((*(_path.top()))->right_subtree != nullptr)
			{
				_path.push(&((*_path.top())->right_subtree));

				while((*_path.top())->left_subtree != nullptr)
				{
					_path.push(&((*_path.top())->left_subtree));
				}
			}
		}

		if (!_path.empty())
			_data->update_iterator(*_path.top(), _path.size() - 1);
	}

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator const binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_iterator::operator*() const
{
	return _data.get();
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::postfix_reverse_iterator(const postfix_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator=(const binary_search_tree::postfix_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++()
{
	node* current_node = *(_path.top());
	_path.pop();

	if(!_path.empty())
	{
		if((*(_path.top()))->right_subtree == current_node)
		{
			if((*(_path.top()))->left_subtree != nullptr)
			{
				_path.push(&((*_path.top())->left_subtree));

				while((*_path.top())->right_subtree != nullptr)
				{
					_path.push(&((*_path.top())->right_subtree));
				}
			}
		}
		if (!_path.empty())
			_data->update_iterator(*_path.top(), _path.size() - 1);
	}

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data *binary_search_tree<tkey, tvalue>::postfix_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const std::stack<node**>& path, iterator_data* data)  : _path(path), _data(data)
{
	if(!_path.empty())
	{
		if (!_path.empty()) _data->update_iterator(*_path.top(), _path.size() - 1);
	}
}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const postfix_const_reverse_iterator& other) : _path(other._path), _data(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data()) {}

template <
		typename tkey,
		typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator& binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator=(const binary_search_tree::postfix_const_reverse_iterator &other)
{
	if (*this != other)
	{
		_path = other._path;
		_data = std::unique_ptr<iterator_data>(other._data.get() == nullptr ? nullptr : other._data->create_deep_copy_of_iterator_data());
	}
	return *this;
}

template<
		typename tkey,
		typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator==(
		typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
	return ((_path.empty() && other._path.empty()) || (!_path.empty() && !other._path.empty() && _path.top() == other._path.top()));
}

template<typename tkey, typename tvalue>
bool binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator!=(
		typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const &other) const noexcept
{
	return !(*this == other);
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator &binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++()
{
	node* current_node = *(_path.top());
	_path.pop();

	if(!_path.empty())
	{
		if((*(_path.top()))->right_subtree == current_node)
		{
			if((*(_path.top()))->left_subtree != nullptr)
			{
				_path.push(&((*_path.top())->left_subtree));

				while((*_path.top())->right_subtree != nullptr)
				{
					_path.push(&((*_path.top())->right_subtree));
				}
			}
		}

		if (!_path.empty())
			_data->update_iterator(*_path.top(), _path.size() - 1);
	}

	return *this;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator const binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator++ (int unused)
{
	auto tmp = *this;
	++(*this);
	return tmp;
}

template<typename tkey, typename tvalue>
typename binary_search_tree<tkey, tvalue>::iterator_data const *binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator::operator*() const
{
	return _data.get();
}

// endregion postfix_const_reverse_iterator implementation

// endregion iterators implementation

// region target operations associated exception types implementation

template<
	typename tkey,
	typename tvalue>
binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::insertion_of_existent_key_attempt_exception(
		tkey const &key):
		std::logic_error("Attempt to insert already existing key inside the tree."), _key(key) {}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to obtain a value by non-existing key from the tree."), _key(key) {}


template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
    std::logic_error("Attempt to dispose a value by non-existing key from the tree."), _key(key) {}

template<
    typename tkey,
    typename tvalue>
tkey const &binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

// endregion target operations associated exception types implementation

// region construction, assignment, destruction implementation

template<
	typename tkey,
	typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		std::function<int(tkey const &, tkey const &)> keys_comparer,
		allocator *allocator,
		logger *logger,
		typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy,
		typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy):
		search_tree<tkey, tvalue>(keys_comparer, logger, allocator), _insert_exist_strategy(insertion_strategy), _dispose_nonexist_strategy(disposal_strategy), _root(nullptr) {}

template<
		typename tkey,
		typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		binary_search_tree<tkey, tvalue> const &other) : search_tree<tkey, tvalue>(other), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)
{
	try
	{
		other.copy_subtree(&_root, other._root);
	}
	catch (...)
	{
		destroy_subtree(_root);
		throw;
	}
}

template<typename tkey, typename tvalue>
binary_search_tree<tkey, tvalue>::binary_search_tree(
		binary_search_tree<tkey, tvalue> &&other) noexcept : search_tree<tkey, tvalue>(std::move(other)), _insert_exist_strategy(other._insert_exist_strategy), _dispose_nonexist_strategy(other._dispose_nonexist_strategy)
{
	_root = std::exchange(other._root, nullptr);
}


template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
		binary_search_tree<tkey, tvalue> const &other)
{
	if (this != &other)
	{
		search_tree<tkey, tvalue>::operator=(other);
		_insert_exist_strategy = other._insert_exist_strategy;
		_dispose_nonexist_strategy = other._dispose_nonexist_strategy;

		try
		{
			copy_subtree(&_root, other._root);
		} catch (...)
		{
			destroy_subtree(_root);
			throw;
		}
	}
	return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue> &binary_search_tree<tkey, tvalue>::operator=(
		binary_search_tree<tkey, tvalue> &&other) noexcept
{
	if (this != &other)
	{
		search_tree<tkey, tvalue>::operator=(std::move(other));
		_insert_exist_strategy = other._insert_exist_strategy;
		_dispose_nonexist_strategy = other._dispose_nonexist_strategy;

		std::swap(_root, other._root);
	}
	return *this;
}

template<
    typename tkey,
    typename tvalue>
binary_search_tree<tkey, tvalue>::~binary_search_tree()
{
	destroy_subtree(_root);
}

// endregion construction, assignment, destruction implementation

// region associative_container<tkey, tvalue> contract implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
	tkey const &key,
	tvalue const &value)
{
	std::stack<node**> path = find_path_to_crnt_node(key);

	if(*path.top() == nullptr) // нет узла с key -> вставляем новый узел
	{
		insert_inside(key, value, path);
	}
	else // исключение либо меняем
	{
		if(_insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
		{
			throw insertion_of_existent_key_attempt_exception(key);
		}
		else
		{
			(*path.top())->value = value;
		}
	}

}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
	std::stack<node**> path = find_path_to_crnt_node(key);

	if(*path.top() == nullptr) // нет узла с key -> вставляем новый узел
	{
		insert_inside(key, std::move(value), path);
	}
	else // исключение либо меняем
	{
		if(_insert_exist_strategy == insertion_of_existent_key_attempt_strategy::throw_an_exception)
		{
			throw insertion_of_existent_key_attempt_exception(key);
		}
		else
		{
			(*path.top())->value = std::move(value);
		}
	}

}

template<
    typename tkey,
    typename tvalue>
tvalue const &binary_search_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
	std::stack<node**> path = find_path_to_crnt_node(key);

	if(*path.top() == nullptr)
	{
		throw obtaining_of_nonexistent_key_attempt_exception(key);
	}

	return obtain_inside(path);
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> binary_search_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
	std::stack<node**> path = find_path_to_crnt_node(lower_bound);

	if(*path.top() == nullptr)
	{
		path.pop();
	}

	infix_iterator it(path, new iterator_data), end = end_infix();

	std::vector<typename associative_container<tkey, tvalue>::key_value_pair> result;



	if(it != end)
	{
		int cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, lower_bound);

		if(cmp < 0)
		{
			++it;
		}
		else if(cmp == 0)
		{
			if(lower_bound_inclusive)
			{
				result.emplace_back( (*it)->key, (*it)->value );
			}
			++it;
		}

		if(it != end)
		{
			cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, upper_bound);
		}

		while(it != end && cmp < 0)
		{
			result.emplace_back((*it)->key, (*it)->value);
			++it;
			cmp = search_tree<tkey, tvalue>::_keys_comparer((*it)->key, upper_bound);
		}


		if(it != end && cmp == 0 && upper_bound_inclusive)
		{
			result.emplace_back((*it)->key, (*it)->value);
		}

	}

	return result;

}

template<
    typename tkey,
    typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
	std::stack<node**> path = find_path_to_crnt_node(key);

	if(*path.top() == nullptr)
	{
		if(_dispose_nonexist_strategy == disposal_of_nonexistent_key_attempt_strategy::throw_an_exception)
		{
			throw disposal_of_nonexistent_key_attempt_exception(key);
		}
		else
		{
			if constexpr (std::is_default_constructible<tvalue>::value) // если конструктируемый по умолчанию -> возвращаем временный объект
			{
				return {};//tvalue()
			}
			else
			{
				throw disposal_of_nonexistent_key_attempt_exception(key);
			}
		}
	}

	return dispose_inside(path);

}

// endregion associative_containers contract implementations

// region destroy/insert node

template<
	typename tkey,
	typename tvalue>
std::stack<typename binary_search_tree<tkey, tvalue>::node**> binary_search_tree<tkey, tvalue>::find_path_to_crnt_node(const tkey & key)
{
	std::stack<node**> result;

	result.push(&(_root));

	auto current_node  = &(_root);

	while(*current_node != nullptr)
	{
		int cmp = search_tree<tkey, tvalue>::_keys_comparer(key, (*current_node)->key);

		if(cmp > 0)
		{
			current_node = &((*current_node)->right_subtree);
		}
		else if(cmp < 0)
		{
			current_node = &((*current_node)->left_subtree);
		}
		else
		{
			break;
		}

		result.push(current_node);
	}

	return result;
}

template<
	typename tkey,
	typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey &key, tvalue &&value, std::stack<node **> &path)
{
	(*path.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node)));
	try
	{
		allocator::construct(*path.top(), key, std::move(value)); // вызываем конструктор node
	}
	catch(...)
	{
		allocator_guardant::deallocate_with_guard(*path.top());
		throw;
	}
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::insert_inside(const tkey &key, const tvalue &value, std::stack<node **> &path)
{
	(*path.top()) =  static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node),1));
	try {
		allocator::construct(*path.top(), key, value);
	}
	catch(...)
	{
		allocator_guardant::deallocate_with_guard(*path.top());
		throw;
	}
}

template<
	typename tkey,
	typename tvalue>
tvalue const& binary_search_tree<tkey, tvalue>::obtain_inside(std::stack<node**> &path)
{
	return (*path.top())->value;
}

template<
	typename tkey,
	typename tvalue>
tvalue binary_search_tree<tkey, tvalue>::dispose_inside(std::stack<node**> &path)
{
	node* current_node = *path.top();

	if(current_node->left_subtree == nullptr && current_node->right_subtree == nullptr)
	{
		*path.top() = nullptr;
	}
	else if(current_node->right_subtree == nullptr || current_node->left_subtree == nullptr)
	{
		node* new_node = current_node->right_subtree != nullptr ? (current_node->right_subtree) : (current_node->left_subtree);

		*path.top() = new_node;
	}
	else
	{
		node** new_node = &((*path.top())->left_subtree);

		while((*new_node)->right_subtree != nullptr)
		{
			new_node = &((*new_node)->right_subtree);
		}

		node* previous_node = *path.top();

		*(path.top()) = *new_node;

		*new_node = (*new_node)->left_subtree;

		(*path.top())->left_subtree = previous_node->left_subtree == *(path.top()) ? *new_node : previous_node->left_subtree;
		(*path.top())->right_subtree = previous_node->right_subtree;

	}

	tvalue res = current_node->value;
	allocator::destruct(current_node);
	allocator_guardant::deallocate_with_guard(current_node);
	return res;
}

template<
	typename tkey,
	typename tvalue>
void binary_search_tree<tkey, tvalue>::copy_subtree(binary_search_tree::node **dest, binary_search_tree::node *src)
{
	if(src == nullptr)
		*dest = nullptr;


	*dest = static_cast<node*>(allocator_guardant::allocate_with_guard(sizeof(node)));

	try{
		allocator::construct(*dest, *src);
	}
	catch(std::bad_alloc & ex) //говно
	{
		allocator_guardant::deallocate_with_guard(*dest);
		*dest = nullptr;
		throw ex;
	}

	(*dest)->right_subtree = nullptr;
	(*dest)->left_subtree = nullptr;

	copy_subtree(&((*dest)->left_subtree), src->left_subtree);
	copy_subtree(&((*dest)->right_subtree), src->right_subtree);
}

template<
	typename tkey,
	typename tvalue>
void binary_search_tree<tkey, tvalue>::destroy_subtree(binary_search_tree::node *src) noexcept
{
	if(src != nullptr)
	{
		destroy_subtree(src->left_subtree);
		destroy_subtree(src->right_subtree);

		allocator::destruct(src);
		allocator_guardant::deallocate_with_guard(src);
	}
}

// endregion destroy/insert node


// region set_strategy

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_insertion_strategy(
    typename binary_search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_strategy insertion_strategy) noexcept
{
	_insert_exist_strategy = insertion_strategy;
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::set_removal_strategy(
    typename binary_search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_strategy disposal_strategy) noexcept
{
	_dispose_nonexist_strategy = disposal_strategy;
}

// endregion set_strategy

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::begin_prefix() const noexcept
{
	std::stack<node**> path;
	if(_root != nullptr)
	{
		path.push(&_root);
	}

	return prefix_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_iterator binary_search_tree<tkey, tvalue>::end_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_prefix() const noexcept
{
	std::stack<node**> path;
	if(_root != nullptr)
	{
		path.push(&_root);
	}

	return prefix_const_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_iterator binary_search_tree<tkey, tvalue>::cend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_prefix() const noexcept
{
	std::stack<node**> path;
	if(_root != nullptr)
	{
		path.push(&_root);
	}

	return prefix_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_prefix() const noexcept
{
	std::stack<node**> path;
	if(_root != nullptr)
	{
		path.push(&_root);
	}

	return prefix_const_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_prefix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::prefix_const_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::begin_infix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = &((*current_node)->left_subtree);
	}

	return infix_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_iterator binary_search_tree<tkey, tvalue>::end_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = &((*current_node)->left_subtree);
	}

	return infix_const_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_iterator binary_search_tree<tkey, tvalue>::cend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = &((*current_node)->right_subtree);
	}

	return infix_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = &((*current_node)->right_subtree);
	}

	return infix_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::infix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_infix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::infix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::begin_postfix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = (*current_node)->left_subtree != nullptr ? &((*current_node)->left_subtree) : &((*current_node)->right_subtree);
	}
	return postfix_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_iterator binary_search_tree<tkey, tvalue>::end_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cbegin_postfix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = (*current_node)->left_subtree != nullptr ? &((*current_node)->left_subtree) : &((*current_node)->right_subtree);
	}
	return postfix_const_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_iterator binary_search_tree<tkey, tvalue>::cend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_const_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rbegin_postfix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
	}
	return postfix_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_reverse_iterator binary_search_tree<tkey, tvalue>::rend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_reverse_iterator(std::stack<node**>(), nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crbegin_postfix() const noexcept
{
	std::stack<node**> path;
	node** current_node = &_root;
	while(*current_node != nullptr)
	{
		path.push(current_node);
		current_node = (*current_node)->right_subtree != nullptr ? &((*current_node)->right_subtree) : &((*current_node)->left_subtree);
	}
	return postfix_const_reverse_iterator(path, new iterator_data);
}

template<
    typename tkey,
    typename tvalue>
typename binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator binary_search_tree<tkey, tvalue>::crend_postfix() const noexcept
{
	return binary_search_tree<tkey, tvalue>::postfix_const_reverse_iterator(std::stack<node**>(), nullptr);
}


// endregion iterators request implementation

// region subtree rotations implementation

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::small_left_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr && subtree_root->right_subtree != nullptr)
	{
		node* ex_root = subtree_root;

		subtree_root = subtree_root->right_subtree;
		ex_root->right_subtree = subtree_root->left_subtree;
		subtree_root->left_subtree = ex_root;

	}
}

template<typename tkey, typename tvalue>
void binary_search_tree<tkey, tvalue>::small_right_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr  && subtree_root->left_subtree != nullptr)
	{
		node* ex_root = subtree_root;

		subtree_root = subtree_root->left_subtree;
		ex_root->left_subtree = subtree_root->right_subtree;
		subtree_root->right_subtree = ex_root;
	}
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_left_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr && subtree_root->right_subtree != nullptr && subtree_root->right_subtree->left_subtree != nullptr)
	{
		small_right_rotation(subtree_root->right_subtree);
		small_left_rotation(subtree_root);
	}
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::big_right_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr && subtree_root->left_subtree != nullptr && subtree_root->left_subtree->right_subtree != nullptr)
	{
		small_left_rotation(subtree_root->left_subtree);
		small_right_rotation(subtree_root);
	}
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_left_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr && subtree_root->right_subtree != nullptr && subtree_root->right_subtree->right_subtree != nullptr)
	{
		small_left_rotation(subtree_root);
		small_left_rotation(subtree_root);
	}
}

template<
    typename tkey,
    typename tvalue>
void binary_search_tree<tkey, tvalue>::double_right_rotation(
		typename binary_search_tree<tkey, tvalue>::node *&subtree_root) const
{
	if(subtree_root != nullptr && subtree_root->left_subtree != nullptr && subtree_root->left_subtree->left_subtree != nullptr)
	{
		small_right_rotation(subtree_root);
		small_right_rotation(subtree_root);
	}
}

// endregion subtree rotations implementation

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H