
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
        assert(inorder_iter == left_left);
        assert(++inorder_iter == left);
        assert(++inorder_iter == left_right);
        assert(++inorder_iter == root);
        assert(++inorder_iter == right);
        assert(++inorder_iter == tree.end());
        assert(--inorder_iter == right);
        assert(--inorder_iter == root);
        assert(--inorder_iter == left_right);
        assert(--inorder_iter == left);
        assert(--inorder_iter == left_left);
    }
    {
        auto preorder_iter = tree.begin(preorder);
        assert(preorder_iter == root);
        assert(++preorder_iter == left);
        assert(++preorder_iter == left_left);
        assert(++preorder_iter == left_right);
        assert(++preorder_iter == right);
        assert(++preorder_iter == tree.end());
        assert(--preorder_iter == right);
        assert(--preorder_iter == left_right);
        assert(--preorder_iter == left_left);
        assert(--preorder_iter == left);
        assert(--preorder_iter == root);
    }
    {
        auto postorder_iter = tree.begin(postorder);
        assert(postorder_iter == left_left);
        assert(++postorder_iter == left_right);
        assert(++postorder_iter == left);
        assert(++postorder_iter == right);
        assert(++postorder_iter == root);
        assert(++postorder_iter == tree.end());
        assert(--postorder_iter == root);
        assert(--postorder_iter == right);
        assert(--postorder_iter == left);
        assert(--postorder_iter == left_right);
        assert(--postorder_iter == left_left);
    }
    {
        auto tree2 = tree;
        assert(tree2 == tree);
    }
}