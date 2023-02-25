
#ifndef INC_201703_TREE_PARSE_HPP
#define INC_201703_TREE_PARSE_HPP

#include <optional>
#include <string_view>
#include <sstream>
#include <stdexcept>
#include <functional>
#include "binary_tree.hpp"

namespace ds_exp
{
    inline namespace parse
    {
        using namespace std::literals;
        struct parse_failed : std::domain_error
        {
            parse_failed()
                : domain_error("parse failed in tree_parse.")
            {
            }
        };
        struct expect_failed : std::domain_error