
#ifndef INC_201703_SAVE_LOAD_HPP
#define INC_201703_SAVE_LOAD_HPP

#include "tree_parse.hpp"

namespace ds_exp
{
    inline namespace tree
    {
        template <typename T>
        std::istream &operator>>(std::istream &in, ds_exp::binary_tree<T> &tree)