
#ifndef INC_201703_SAVE_LOAD_HPP
#define INC_201703_SAVE_LOAD_HPP

#include "tree_parse.hpp"

namespace ds_exp
{
    inline namespace tree
    {
        template <typename T>
        std::istream &operator>>(std::istream &in, ds_exp::binary_tree<T> &tree)
        {
            tree = ds_exp::tree_parse<ds_exp::left_first_t, T>(in).get_binary_tree().value();
            return in;
        }

        template <typename T, typename ...Escaped>
        std::ostream &escape(std::ostream &out, T const &t, Escaped ...escaped)
        {
            std::ostringstream stream;
            stream << t;
            auto output = stream.str();
            for (auto iter = output.begin(); iter != output.end(); ++iter)
            {
                if (((*iter == escaped) || ...))
                {
                    iter = output.insert(iter, '\\');
                    ++iter;
                }
            }
            return out << output;

        }
        inline void print_null(std::ostream &out)
        {
            out << "null";
        }
        template <typename Iter>
        void print_node(std::ostream &out, Iter iter)
        {
            if (!iter)
            {
                print_null(out);
                return;
            }
            out << "(";
            escape(out, *iter, ')') << ")";
            print_node(out << ",", iter.first_child());
            print_node(out << ",", iter.second_child());
        }
        template <typename T>
        std::ostream &operator<<(std::ostream &out, binary_tree<T> const &tree)
        {
            out << "[";;
            auto iter = tree.begin(preorder);
            print_node(out, iter);
            out << "]";
            return out;
        }
    }
}

#endif //INC_201703_SAVE_LOAD_HPP