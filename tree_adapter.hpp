#ifndef INC_201703_TREE_ADAPTER_HPP
#define INC_201703_TREE_ADAPTER_HPP

#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <queue>
#include "binary_tree.hpp"
#include "tree_parse.hpp"
#include "save_load.hpp"

namespace ds_exp
{
    inline namespace adapter
    {
        class null_value_tag;
        namespace detail
        {
            template <typename Key, typename Value>
            struct stored_t
            {
                Key key;
                Value value;
                friend std::ostream &operator<<(std::ostream &out, stored_t const &s)
                {
                    escape(out, s.key, ',', '\\') << ",";
                    escape(out, s.value, ',', '\\');
                    return out;
                }
            };
            template <typename T>
            auto const &get_key(T const &t)
            {
                return t;
            }
            template <typename Key, typename Value>
            auto const &get_key(stored_t<Key, Value> const &s)
            {
                return get_key(