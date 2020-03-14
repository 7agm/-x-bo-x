
#ifndef INC_201703_BINARY_TREE_HPP
#define INC_201703_BINARY_TREE_HPP

#include <cstdlib>
#include <memory>
#include <cassert>

namespace ds_exp
{
    inline namespace tree
    {
        struct right_first_t;
        struct left_first_t
        {
            constexpr left_first_t() = default;
            using inverse = right_first_t;
        };
        struct right_first_t
        {
            constexpr right_first_t() = default;
            using inverse = left_first_t;
        };
        using left_t = left_first_t;
        using right_t = right_first_t;
        constexpr left_first_t left_first;
        constexpr left_t left_child;
        constexpr right_first_t right_first;
        constexpr right_t right_child;

        template <typename T>
        struct node
        {
            using value_type = T;

            template <typename U>
            explicit node(U &&value, node *parent, std::unique_ptr<node> left,
                          std::unique_ptr<node> right)
                :value(std::forward<U>(value)), left_child(std::move(left)), right_child(std::move(right)), parent(parent)
            {
            }

            value_type value;
            std::unique_ptr<node> left_child;
            std::unique_ptr<node> right_child;
            node *parent = nullptr;
        };

        template <typename direction_tag>
        struct iterate_direction;

        template <>
        struct iterate_direction<left_first_t>
        {
            ~iterate_direction() = default;
            template <typename T>
            static auto &first_child(node<T> *p)
            {
                return p->left_child;
            }
            template <typename T>
            static auto &second_child(node<T> *p)
            {
                return p->right_child;
            }
        };

        template <>
        struct iterate_direction<right_first_t>
        {
            ~iterate_direction() = delete;
            template <typename T>
            static auto &first_child(node<T> *p)
            {
                return p->right_child;
            }
            template <typename T>
            static auto &second_child(node<T> *p)
            {
                return p->left_child;
            }
        };

        struct inorder_t
        {
            constexpr inorder_t() = default;
            using inverse = inorder_t;
        };
        struct preorder_t;
        struct postorder_t
        {
            constexpr postorder_t() = default;
            using inverse = preorder_t;
        };
        struct preorder_t
        {
            constexpr preorder_t() = default;
            using inverse = postorder_t;
        };
        constexpr inorder_t inorder;
        constexpr preorder_t preorder;
        constexpr postorder_t postorder;

        template <typename T, typename order, typename dir>
        struct order_template;
        template <typename T, typename dir>
        struct order_template<T, inorder_t, dir>
        {
            using node_type = node<T>;
            using order_type = inorder_t;
            using direction = iterate_direction<dir>;
            using inverse_order = order_template<T, order_type::inverse, typename dir::inverse>;
            static node_type *begin(node_type *root)
            {
                assert(root);
                auto current = root;
                while (direction::first_child(current))
                    current = direction::first_child(current).get();
                return current;
            }
            static node_type *next(node_type *current)
            {
                assert(current);
                if (direction::second_child(current))
                    return begin(direction::second_child(current).get());
                else
                    return backtrack(current);
            }
            static node_type *backtrack(node_type *current)
            {
                while (current->parent != nullptr && direction::second_child(current->parent).get() == current)
                    current = current->parent;
                if (current->parent == nullptr)
                    return nullptr;
                assert(direction::first_child(current->parent).get() == current);
                return current->parent;
            }
        };

        template <typename T, typename dir>
        struct order_template<T, postorder_t, dir>;
        template <typename T, typename dir>
        struct order_template<T, preorder_t, dir>
        {
            using node_type = node<T>;
            using order_type = preorder_t;
            using direction = iterate_direction<dir>;
            using inverse_order = order_template<T, order_type::inverse, typename dir::inverse>;
            static node_type *begin(node_type *root)
            {
                assert(root);
                return root;
            }
            static node_type *next(node_type *current)
            {
                assert(current);
                if (direction::first_child(current))
                    return direction::first_child(current).get();
                else if (direction::second_child(current))
                    return direction::second_child(current).get();
                else
                    return backtrack(current);
            }
            static node_type *backtrack(node_type *current)
            {
                while (current->parent != nullptr &&
                       (direction::second_child(current->parent).get() == current ||
                        !direction::second_child(current->parent)))
                    current = current->parent;
                if (current->parent == nullptr)
                    return nullptr;
                assert(direction::first_child(current->parent).get() == current &&
                       direction::second_child(current->parent) != nullptr);
                return direction::second_child(current->parent).get();
            }
        };

        template <typename T, typename dir>
        struct order_template<T, postorder_t, dir>
        {
            using node_type = node<T>;
            using order_type = postorder_t;
            using direction = iterate_direction<dir>;
            using inverse_order = order_template<T, order_type::inverse, typename dir::inverse>;
            static node_type *begin(node_type *root)
            {
                assert(root);
                auto current = root;
                while (direction::first_child(current))
                    current = direction::first_child(current).get();
                while (direction::second_child(current))
                {
                    current = direction::second_child(current).get();
                    while (direction::first_child(current))
                        current = direction::first_child(current).get();
                }
                return current;
            }
            static node_type *next(node_type *current)
            {
                assert(current);
                if (current->parent == nullptr)
                    return nullptr;
                else if (direction::first_child(current->parent).get() == current &&
                         direction::second_child(current->parent) != nullptr)
                    return begin(direction::second_child(current->parent).get());
                else
                {
                    assert(direction::second_child(current->parent).get() == current ||
                           direction::second_child(current->parent) == nullptr);
                    return current->parent;
                }
            }
        };

        template <typename T>
        class binary_tree
        {
            using default_order = preorder_t;
            using default_direction = left_first_t;
        public:
            using value_type = T;
            using node_type = node<value_type>;
            using handler_type = std::unique_ptr<node_type>;
            using size_type = std::size_t;

        private:
            struct base_iter
            {
                using difference_type = std::ptrdiff_t;
                using value_type = binary_tree::value_type;
                using pointer = value_type *;
                using reference = value_type &;
                using iterator_category = std::bidirectional_iterator_tag;

                template <typename iter1, typename iter2>
                friend bool operator==(iter1 const &lhs, iter2 const &rhs)
                {
                    return lhs.node == rhs.node;
                }
                template <typename iter1, typename iter2>
                friend bool operator!=(iter1 const &lhs, iter2 const &rhs)
                {
                    return !(lhs == rhs);
                }
            };
            explicit binary_tree(handler_type root)
                : root_(std::move(root))
            {
                if (root_)
                    root_->parent = nullptr;
            }
        public:

            template <typename, typename>
            class iterator;

            template <typename default_order, typename default_direction>
            class const_iterator : public base_iter
            {
                template <typename, typename>
                friend
                class iterator;
                friend class binary_tree;

                const_iterator(const binary_tree *tree, node_type *p)
                    : tree(tree), node(p)
                {
                }

                const binary_tree *tree;
                node_type *node;
            public:

                template <typename order, typename direction>
                const_iterator(const_iterator<order, direction> const &src)
                    :tree(src.tree), node(src.node)
                {
                }
                explicit operator bool() const
                {
                    return node != nullptr;
                }
                value_type const &operator*() const
                {
                    return node->value;
                }
                value_type const *operator->() const
                {
                    return &node->value;
                }
                auto &operator++()
                {
                    return next(), *this;
                }
                auto operator++(int)
                {
                    auto iter = *this;
                    return this->next(), iter;
                }
                auto &operator--()
                {
                    return previous(), *this;
                }
                auto operator--(int)
                {
                    auto iter = *this;
                    return this->previous(), iter;
                }
                template <typename order = default_order, typename direction = default_direction>
                void next(order = order{}, direction = direction{})
                {
                    assert(node);
                    node = order_template<value_type, order, direction>::next(node);
                }
                template <typename order = default_order, typename direction = default_direction>
                void previous(order = order{}, direction = direction{})
                {
                    if (node == nullptr)
                        node = order_template<value_type, order, direction>::inverse_order::begin(tree->root_.get());
                    else
                    {
                        auto pre = order_template<value_type, order, direction>::inverse_order::next(node);
                        if (pre != nullptr)
                            node = pre;
                    }
                }
                template <typename direction = default_direction>
                const_iterator first_child(direction = direction{}) const
                {
                    assert(node);
                    return const_iterator(tree, iterate_direction<direction>::first_child(node).get());
                }
                template <typename direction = default_direction>
                const_iterator second_child(direction = direction{}) const
                {
                    assert(node);
                    return const_iterator(tree, iterate_direction<direction>::second_child(node).get());
                }
                const_iterator parent() const
                {
                    assert(node &&node->parent);
                    return const_iterator(tree, node->parent);
                }
                template <typename order = default_order, typename direction = default_direction>
                auto change(order = order{}, direction = direction{}) const
                {
                    return const_iterator<order, direction>(*this);
                }
                template <typename iter1, typename iter2>
                friend bool operator==(iter1 const &, iter2 const &);
            };

            template <typename default_order, typename default_direction>
            class iterator : public base_iter
            {
                friend class binary_tree;

                iterator(binary_tree *tree, node_type *p)
                    : tree(tree), node(p)
                {
                }

                binary_tree *tree;
                node_type *node;
            public:
                template <typename order = default_order, typename direction = default_direction>
                iterator(iterator<order, direction> const &src)
                    :tree(src.tree), node(src.node)
                {
                }
                template <typename order, typename direction>
                operator const_iterator<order, direction>()
                {
                    return const_iterator<order, direction>(tree, node);
                }
                explicit operator bool() const
                {
                    return node != nullptr;
                }
                value_type &operator*() const
                {
                    return node->value;
                }
                value_type *operator->() const
                {
                    return &node->value;
                }
                auto &operator++()
                {
                    return next(), *this;
                }
                auto operator++(int)
                {
                    auto iter = *this;
                    return this->next(), iter;
                }
                auto &operator--()
                {
                    return previous(), *this;
                }
                auto operator--(int)
                {
                    auto iter = *this;
                    return this->previous(), iter;
                }
                template <typename order = default_order, typename direction = default_direction>
                void next(order = order{}, direction = direction{})
                {
                    assert(node);
                    node = order_template<value_type, order, direction>::next(node);
                }
                template <typename order = default_order, typename direction = default_direction>
                void previous(order = order{}, direction = direction{})
                {
                    if (node == nullptr)
                        node = order_template<value_type, order, direction>::inverse_order::begin(tree->root_.get());
                    else
                    {
                        auto pre = order_template<value_type, order, direction>::inverse_order::next(node);
                        if (pre != nullptr)
                            node = pre;
                    }
                }
                template <typename direction = default_direction>
                iterator first_child(direction = direction{}) const
                {
                    assert(node);
                    return iterator(tree, iterate_direction<direction>::first_child(node).get());
                }
                template <typename direction = default_direction>
                iterator second_child(direction = direction{}) const
                {
                    assert(node);
                    return iterator(tree, iterate_direction<direction>::second_child(node).get());
                }
                iterator parent() const
                {
                    assert(node &&node->parent);
                    return iterator(tree, node->parent);
                }
                template <typename order = default_order, typename direction = default_direction>
                auto change(order = order{}, direction = direction{}) const
                {
                    return iterator<order, direction>(*this);
                }

                template <typename iter1, typename iter2>
                friend bool operator==(iter1 const &, iter2 const &);
            };

            binary_tree() = default;
            binary_tree(binary_tree &&src) = default;
            binary_tree(binary_tree const &src)
            {
                set_root(*src.root());
                for (auto src_iter = src.begin(preorder), dest_iter = cbegin(preorder); src_iter != src.end(); ++src_iter, ++dest_iter)
                {
                    if (src_iter.first_child())
                        new_child(dest_iter, *src_iter.first_child(), default_direction{});
                    if (src_iter.second_child())
                        new_child(dest_iter, *src_iter.second_child(), default_direction::inverse{});
                }
            }
            binary_tree &operator=(binary_tree &&) = default;
            binary_tree &operator=(binary_tree const &src)
            {
                *this = binary_tree(src);
                return *this;
            }

            template <typename order_t = default_order, typename direction_t = default_direction>
            auto begin(order_t order = order_t{}, direction_t direction = direction_t{})
            {
                if (!root_)
                    return end(order, direction);
                return get_iter<order_t, direction_t>(order_template<value_type, order_t, direction_t>::begin(root_.get()));
            }

            template <typename order_t = default_order, typename direction_t = default_direction>
            auto begin(order_t order = order_t{}, direction_t direction = direction_t{}) const
            {
                if (!root_)
                    return end(order, direction);
                return get_const_iter<order_t, direction_t>(order_template<value_type, order_t, direction_t>::begin(root_.get()));
            }
            template <typename order_t = default_order, typename direction_t = default_direction>
            auto cbegin(order_t order = order_t{}, direction_t direction = direction_t{}) const
            {
                if (!root_)
                    return end(order, direction);
                return begin(order, direction);
            }

            template <typename order_t = default_order, typename direction_t = default_direction>
            auto end(order_t = order_t{}, direction_t = direction_t{})
            {
                return get_iter<order_t, direction_t>(nullptr);
            }
            template <typename order_t = default_order, typename direction_t = default_direction>
            auto end(order_t = order_t{}, direction_t = direction_t{}) const
            {
                return get_const_iter<order_t, direction_t>(nullptr);
            }
            template <typename order_t = default_order, typename direction_t = default_direction>
            auto cend(order_t = order_t{}, direction_t = direction_t{}) const
            {
                return get_const_iter<order_t, direction_t>(nullptr);
            }

            template <typename order_t = default_order, typename direction_t = default_direction>
            auto root(order_t = order_t{}, direction_t = direction_t{})
            {
                return get_iter<order_t, direction_t>(root_.get());
            }
            template <typename order_t = default_order, typename direction_t = default_direction>
            auto root(order_t = order_t{}, direction_t = direction_t{}) const
            {
                return get_const_iter<order_t, direction_t>(root_.get());
            }

            void clear()
            {
                root_.reset();
            }
            bool empty() const
            {
                return root() == end();
            }
            std::size_t depth() const
            {
                return subtree_depth(root());
            }
            template <typename iter>
            std::size_t subtree_depth(iter subtree_root) const
            {
                if (subtree_root == end())
                    return 0;
                auto i = std::max(subtree_depth(subtree_root.first_child()), subtree_depth(subtree_root.second_child())) + 1;
                return i;
            }

            template <typename iter>
            binary_tree replace(iter replaced, binary_tree &&new_tree)
            {
                handler_type *handler = nullptr;
                if (replaced == root())
                    handler = &root_;
                else
                    handler = &get_handler(replaced.node);
                auto parent = (*handler)->parent;
                auto returned = std::move(*handler);
                *handler = std::move(new_tree.root_);
                if (*handler)
                    (*handler)->parent = parent;
                return binary_tree(returned);
            }

            template <typename iter>
            binary_tree remove(iter subtree)
            {
                return replace(subtree, binary_tree{});
            }

            template <typename U>
            void set_root(U &&u)
            {
                root_ = make_handler(std::forward<U>(u), nullptr);
            }
            template <typename direction, typename iter, typename U>
            iter new_child(iter parent, U &&u, direction = direction{})
            {
                auto &child = iterate_direction<direction>::first_child(parent.node);
                child = make_handler(std::forward<U>(u), parent.node);
                return iter(this, child.get());
            }
            template <typename iter, typename direction_t>
            binary_tree replace_child(iter parent, binary_tree &&tree, direction_t = direction_t{})
            {
                auto &child = iterate_direction<direction_t>::first_child(parent.node);
                auto replaced = std::move(child);
                child = std::move(tree.root_);
                if (child)
                    child->parent = parent.node;
                return binary_tree(std::move(replaced));
            }

            friend bool operator==(binary_tree const &lhs, binary_tree const &rhs)
            {
                auto left_iter = lhs.begin(preorder), right_iter = rhs.begin(preorder);
                for (; left_iter != lhs.end() && right_iter != rhs.end(); ++left_iter, ++right_iter)
                {
                    if (*left_iter != *right_iter)
                        return false;
                }
                return left_iter == right_iter;
            }
        private:

            template <typename default_order, typename default_direction>
            auto get_iter(node_type *p)
            {
                return iterator<default_order, default_direction>{this, p};
            }
            template <typename default_order, typename default_direction>
            auto get_const_iter(node_type *p) const
            {
                return const_iterator<default_order, default_direction>{this, p};
            }
            auto &get_handler(node_type *p)
            {
                if (p->parent->left_child.get() == p)
                    return p->parent->left_child;
                else
                    return p->parent->right_child;
            }
            template <typename U>
            auto make_handler(U &&u, node_type *parent = nullptr, handler_type left = nullptr, handler_type right = nullptr)
            {
                return std::make_unique<node_type>(std::forward<U>(u), parent, std::move(left), std::move(right));
            }
            handler_type root_;
        };

        template <typename tree_t, typename order_t, typename dir_t>
        class iterate_adapter
        {
        public:
            explicit iterate_adapter(tree_t &&tree)
                :tree(tree)
            {
            }

            auto begin()
            {
                return tree.begin(order_t{}, dir_t{});
            }
            auto cbegin()
            {
                return tree.cbegin(order_t{}, dir_t{});
            }
            auto end()
            {
                return tree.end(order_t{}, dir_t{});
            }
            auto cend()
            {
                return tree.cend(order_t{}, dir_t{});
            }
            tree_t &&tree;