#include "test_tree_adapter.hpp"
#include "../tree_adapter.hpp"

void test_tree_adapter()
{
    using namespace ds_exp;
    using namespace std::literals;
    tree_adapter<std::string, int> adapter;
    std::string definition