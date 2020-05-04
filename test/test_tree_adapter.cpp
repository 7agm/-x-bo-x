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
    assert(value == 3);
    adapter.Assign("left", 5);
    assert(adapter.Value("left") == 5);
    adapter.Assign("left", 2);
    assert(adapter.Parent("left") == adapter.Root());
    assert(get_value(*adapter.Child("left", left_child)) == 3);
    assert(get_value(*adapter.Child("right", right_child)) == 5);
    assert(get_value(*adapter.Sibling("left", right_child)) == 4);
    assert(get_value(*adapter.Sibling("right", left_child)) == 2);
    auto right_node = adapter.Child("root", right_child);
    decltype(adapter) new_adapter;
    new_adapter.CreateBiTree(definition