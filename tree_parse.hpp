
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
            v = std::move(str);
        }

        namespace detail
        {

            inline void eat_space(std::istream &in)
            {
                while (in && std::isspace(in.peek()))
                    in.get();
            }
            inline bool read_char(std::istream &in, char c)
            {
                eat_space(in);
                if (in && in.peek() == c)
                    return in.get(), true;
                else
                    return false;
            }
            inline void force_read_char(std::istream &in, char c)
            {
                if (!read_char(in, c))
                    throw expect_failed(std::string() + c);
            }
            template <typename ...Escaped>
            void unescape(std::istream &in, Escaped ...escaped)
            {
                if (in.peek() == '\\')
                {
                    auto pos = in.tellg();
                    in.get();
                    if (((in.peek() == escaped) || ...))
                        return;
                    in.seekg(pos);
                }
            }
            template <typename ...Stops>
            std::string read_until(std::istream &in, bool skip_backslash, Stops ...stop)
            {
                std::string str;
                while (in && ((in.peek() != stop) && ...))
                {
                    if (skip_backslash)
                        unescape(in, '\\', stop...);
                    else
                        unescape(in, stop...);
                    char c = in.get();
                    if (in)
                        str.push_back(c);
                }
                return str;
            }
            inline bool read_word(std::istream &in, std::string_view word)