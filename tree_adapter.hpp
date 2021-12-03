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
                return get_value(s.value);
            }
            template <typename Key, typename Value>
            auto &get_value(stored_t<Key, Value> &s)
            {
                return get_value(s.value);
            }

            template <typename Key, typename Value>
            struct value_traits
            {
                using type = stored_t<Key, Value>;
                using key_type = Key;
                using value_type = Value;
            };
            template <typename Key>
            struct value_traits<Key, null_value_tag>
            {
                using type = Key;
                using key_type = Key;
                using value_type = Key;
            };
            template <typename Key, typename Value>
            void assign_element(std::string str, detail::stored_t<Key, Value> &v)
            {
                using ds_exp::assign_element;
                std::istringstream source(str);
                auto key_input = parse::detail::read_until(source, true, ',');
                parse::detail::force_read_char(source, ',');
                auto value_input = parse::detail::read_until(source, true);
                assign_element(std::move(key_input), v.key);
                assign_element(std::move(value_input), v.value);
            }
        }
        using detail::get_key;
        using detail::get_value;
        using detail::value_traits;

        using namespace std::literals;

        template <typename Key_t, typename Value_t = null_value_tag>
        class tree_adapter
        {
        public:
            using element_type = typename value_traits<Key_t, Value_t>::type;
            using tree_type = binary_tree<element_type>;
            using key_type = typename value_traits<Key_t, Value_t>::key_type;
            using value_type = typename value_traits<Key_t, Value_t>::value_type;

        private:
            std::optional<tree_type> tree;

            tree_adapter(tree_type &&tree)
                :tree(std::move(tree))
            {}
        public:
            struct tree_exists : std::logic_error
            {
                explicit tree_exists(std::string const &function)
                    : logic_error("The tree already exists in function "s + function + ".")
                {
                }
            };
            struct tree_not_exist : std::logic_error
            {
                explicit tree_not_exist(std::string const &function)
                    : logic_error("The tree doesn't exist in function "s + function + ".")
                {
                }
            };
            struct parse_failed : std::logic_error
            {
                parse_failed(std::string const &function)
                    : logic_error("Parse tree failed in function "s + function + ".")
                {
                }
            };
            struct precondition_failed_to_satisfy : std::logic_error
            {
                precondition_failed_to_satisfy(std::string const &function)
                    : logic_error("The precondition of funciton "s + function + " failed to satisfy.")
                {
                }
            };

            tree_adapter() = default;
            void InitBiTree()
            {
                if (tree)
                    throw tree_exists(__func__);
                tree = binary_tree<element_type>();
            }
            void DestroyBiTree()
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                tree.reset();
            }
            void CreateBiTree(std::i