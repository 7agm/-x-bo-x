
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