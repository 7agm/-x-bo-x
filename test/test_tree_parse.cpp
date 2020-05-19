
#include <string>
#include <sstream>
#include "test_tree_parse.hpp"
#include "../tree_parse.hpp"
#include "../tree_adapter.hpp"

void test_tree_parse()
{
    using namespace ds_exp;
    using namespace std::literals;
    std::istringstream istream(R"~( [ (\,, 1), ([, 2), (\\,3), null, null, null, (\),4), null, null] )~"s);
    tree_parse<left_first_t, adapter::detail::stored_t<std::string, int>> parse(istream);
    auto tree = parse.get_binary_tree().value();
    auto root = tree.root<preorder_t>();
    auto iter = root;
    assert(root->key == ",");
    assert(root->value == 1);
    ++iter;
    assert(iter->key == "[");
    assert(iter->value == 2);
    ++iter;
    assert(iter->key == "\\");
    assert(iter->value == 3);
    ++iter;
    assert(iter->key == ")");