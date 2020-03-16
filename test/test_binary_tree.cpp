
#include <string>
#include "test_binary_tree.hpp"
#include "../binary_tree.hpp"

void test_binary_tree()
{
    using namespace ds_exp;
    binary_tree<std::string> tree;
    tree.set_root("root");
    auto root = tree.begin();
    assert(*root == "root");
    assert(++root == tree.end());
    assert(--root == tree.begin());
    auto left = tree.new_child(root, "left child", left_child );
    assert(*left == "left child");
    assert(*root.first_child() == "left child");
    auto right = tree.new_child(root, "right child", right_child );
    assert(*right == "right child");
    assert(*root.second_child() == "right child");
    auto left_left = tree.new_child(left, "left left", left_child);
    assert(*left_left == "left left");
    assert(*left.first_child() == "left left");
    auto left_right = tree.new_child(left, "left right", right_child);
    assert(*left_right == "left right");
    assert(*left.second_child() == "left right");
    {
        auto inorder_iter = tree.begin(inorder);