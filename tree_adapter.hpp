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
            void CreateBiTree(std::istream &definition)
            {
                auto generated_tree = tree_parse<left_first_t, element_type>(definition).get_binary_tree();
                if (!generated_tree)
                    throw parse_failed(__func__);
                tree = generated_tree;
            }
            void CreateBiTree(std::string const &string)
            {
                std::istringstream stream(string);
                CreateBiTree(stream);
            }
            void ClearBiTree()
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                tree->clear();
            }
            auto BiTreeEmpty() const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                return tree->empty();
            }
            auto BiTreeDepth() const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                return tree->depth();
            }
            auto Root() const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                return tree->root();
            }
            template <typename order_t = preorder_t, typename dir_t = left_first_t>
            auto &Value(key_type const &key, order_t order = order_t{}, dir_t dir = dir_t{}) const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                for (auto &element : tree_iterate(*tree, order, dir))
                    if(element == key)
                        return get_value(element);
                throw precondition_failed_to_satisfy(__func__);
            }
            template <typename U, typename order_t = preorder_t, typename dir_t = left_first_t>
            void Assign(key_type const &key, U &&value, order_t order = order_t{}, dir_t dir = dir_t{})
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                if (auto iter = std::find(tree->begin(order, dir), tree->end(order, dir), key))
                    get_value(*iter) = std::forward<U>(value);
                else
                    throw precondition_failed_to_satisfy(__func__);
            }

            template <typename order_t = preorder_t, typename dir_t = left_first_t>
            auto Parent(key_type const &key, order_t order = order_t{}, dir_t dir = dir_t{}) const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                if (auto iter = std::find(tree->begin(order, dir), tree->end(order, dir), key))
                    return iter.parent();
                throw precondition_failed_to_satisfy(__func__);
            }
            template <typename child_t, typename order_t = preorder_t, typename dir_t = left_first_t>
            auto Child(key_type const &key, child_t child = child_t{}, order_t order = order_t{}, dir_t dir = dir_t{}) const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                if (auto iter = std::find(tree->begin(order, dir), tree->end(order, dir), key))
                    return iter.first_child(child);
                throw precondition_failed_to_satisfy(__func__);
            }
            template <typename child_t, typename order_t = preorder_t, typename dir_t = left_first_t>
            auto Sibling(key_type const &key, child_t child = child_t{}, order_t order = order_t{}, dir_t dir = dir_t{}) const
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                if (auto iter = std::find(tree->begin(order, dir), tree->end(order, dir), key))
                {
                    auto desired_child = iter.parent().first_child(child);
                    if (desired_child == iter)
                        return tree->end(order, dir);
                    return desired_child;
                }
                throw precondition_failed_to_satisfy(__func__);
            }
            template <typename child_t, typename iter, typename dir_t = right_t>
            void InsertChild(iter pos, tree_adapter inserted, child_t child = child_t{}, dir_t dir = dir_t{})
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                auto replaced = tree->replace_child(pos, std::move(inserted.tree.value()), child);
                auto farest = tree->begin(inorder, dir);
                auto empty = tree->replace_child(farest, std::move(replaced), dir);
                assert(empty.empty());
            }
            template <typename child_t, typename iter>
            auto DeleteChild(iter pos, child_t child = child_t{})
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                return tree_adapter(tree->replace_child(pos, tree_type{}, child));
            }
            template <typename Callable, typename order_t, typename dir_t = left_first_t>
            void Traverse(Callable callable, order_t order, dir_t dir = dir_t{})
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                for(auto &element : tree_iterate(*tree, order, dir))
                    callable(element);
            }
            template <typename Callable, typename dir_t = left_first_t>
            void LevelOrderTraverse(Callable callable, dir_t dir = dir_t{})
            {
                if (!tree)
                    throw tree_not_exist(__func__);
                auto iter = tree->root(preorder, dir);
                if(!iter)
                    return;
                std::queue<decltype(iter)> queue;
                queue.push(iter);
                while (!queue.empty())
                {
                    iter = queue.front(), queue.pop();
                    if (iter.first_child(dir))
                        queue.push(iter.first_child(dir));
                    if (iter.second_child(dir))
                        queue.push(iter.second_child(dir));
                    callable(*iter);
                }
            }

            template <typename order_t = preorder_t, typename dir_t = left_first_t>
            auto get_iterator(key_type const &key, order_t order = order_t{}, dir_t dir = dir_t{})
            {
                if(auto iter = std::find(tree->begin(order, dir), tree->end(order, dir), key))
                    return iter;
                throw precondition_failed_to_satisfy(__func__);
            }
            template <typename order_t = preorder_t, typename dir_t = left_first_t>
            auto get_end_iterator(order_t order = order_t{}, dir_t dir = dir_t{})
            {
                if(!tree)
                    throw precondition_failed_to_satisfy(__func__);
                return tree->end(order, dir);
            }

            friend bool operator==(tree_adapter const &lhs, tree_adapter const &rhs)
            {
                return *lhs.tree == *rhs.tree;
            }

            friend std::ostream &operator<<(std::ostream &out, tree_adapter const&tree)
            {
                if(!tree.tree)
                    out << 0 << " ";
                else
                {
                    out << 1 << " " << tree.tree.value();
                }
                return out;
            }
            friend std::istream &operator>>(std::istream &in, tree_adapter &tree)
            {
                int has_tree = 0;
                in >> has_tree;
                if(!has_tree)
                    tree.tree.reset();
                else
                {
                    tree.tree = tree_type{};
                    in >> tree.tree.value();
                }
                return in;
            }
        };
    }
}

#endif //INC_201703_TREE_ADAPTER_HPP
