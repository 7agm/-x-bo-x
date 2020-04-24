#include "test_tree_adapter.hpp"
#include "../tree_adapter.hpp"

void test_tree_adapter()
{
    using namespace ds_exp;
    using namespace std::literals;
    tree_adapter<std::string, int> adapter;
    std::string definition(R"~([(root,1), (left,2), (left left,3), null, null, null , (right,4), null, (right right, 5), null, null ])~"s);
    adapter.InitBiTree();
    adapter.DestroyBiTree();
    adapter.CreateBiTree(definition);
    adapter.ClearBiTree();
    assert(adapter.BiTreeEmpty());
    adapter.CreateBiTree(definition);
    assert(adapter.BiTreeDepth() == 3);
    assert(get_value(*adapter.Root()) == 1);
    auto value = adapter.Value("left left");
    ass