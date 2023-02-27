
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
        {
            expect_failed(std::string const &s)
                : domain_error("expect '"s + s + "' but got something else.")
            {
            }
        };
        struct unexpected_end : std::domain_error
        {
            unexpected_end()
                : domain_error("unexpected end of string when parsing.")
            {
            }
        };
        namespace detail
        {
            template <typename U>
            struct final_call
            {
                final_call(U c)
                    : call(c)
                {
                }
                ~final_call()
                {
                    call();
                }
                U call;
            };
        }

        template <typename value>
        void assign_element(std::string str, value &v)
        {
            std::istringstream stream(str);
            stream >> v;
            if (!stream)
                throw parse_failed();
        }
        template <>
        inline void assign_element(std::string str, std::string &v)
        {