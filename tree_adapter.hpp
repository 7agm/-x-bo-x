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
                friend std::ostream &operator