
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