#include "../src/tree.h"
#include <assert.h>
#include <iostream>
#include <bits/stl_function.h>

int main(){
    custl::_Rb_tree<int,int, std::_Identity<int>,std::less<int>> tree;
    assert(tree.size() == 0);
    std::cout << "max_size: " << tree.max_size() << std::endl;
    tree.insert_equal(1);
    assert(tree.size() == 1);
    tree.insert_unique(19);
    assert(tree.size() == 2);
    assert(tree.__verify());
}
