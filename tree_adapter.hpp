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
                return get_key(s.key);
            }
            template <typename Key1, typename Value1, typename Key2, typename Value2>
            bool operator<(stored_t<Key1,Value1> const &lhs, stored_t<Key2, Value2> const &rhs)
            {
                return get_key(lhs) < get_key(rhs);
            }
            template <typename Key1, typename Value1, typename Key2>
            bool operator<(stored_t<Key1,Value1> const &lhs, Key2 const &rhs)
            {
                return get_key(lhs) < rhs;
            }
            template <typename Key1, typename Key2, typename Value2>
            bool operator<(Key1 const &lhs, stored_t<Key2, Value2> const &rhs)
            {
                return lhs < get_key(rhs);
            }
            template <typename t1, typename t2>
            struct support_equality
            {
                template <typename r1, typename r2, typename Ret = decltype(get_key(std::declval<r1>()) == get_key(std::declval<r2>()))>
                static Ret helper(int)
                {}
                template <typename , typename, typename T = void>
                static T helper(double)
                {}
                constexpr static bool value = std::is_convertible_v<decltype(helper<t1,t2>(0)), bool>;
            };
            template <typename t1, typename t2, std::enable_if_t<support_equality<t1, t2>::value, int> = 0>
            bool operator==(t1 const &lhs, t2 const &rhs)
            {
                return get_key(lhs) == get_key(rhs);
            }
            template <typename t1, typename t2, std::enable_if_t<!support_equality<t1, t2>::value, int> = 0>
            bool operator==(t1 const &lhs, t2 const &rhs)
            {
                return !(get_key(lhs) < get_key(rhs)) && !(get_key(rhs) < get_key(rhs));
            }
            template <typename T1, typename T2>
            bool operator!=(T1 const &lhs, T2 const &rhs)
            {
                return !(lhs == rhs);
            }
            template <typename T>
            auto &get_value(T &t)
            {
                return t;
            }
            template <typename T>
            auto &get_value(T const &t)
            {
                return t;
            }
            template <typename Key, typename Value>
            auto &get_value(stored_t<Key, Value> const &s)
            {
  