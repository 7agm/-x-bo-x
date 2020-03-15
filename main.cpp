

#include "test/test_binary_tree.hpp"
#include "test/test_tree_parse.hpp"
#include "test/test_tree_adapter.hpp"
#include "console_ui.hpp"

int main()
{
    std::cout<<std::boolalpha;
    test_binary_tree();
    test_tree_parse();
    test_tree_adapter();
    ds_exp::console_ui<std::string, std::string> ui;
    ui.execute();
    return 0;
}