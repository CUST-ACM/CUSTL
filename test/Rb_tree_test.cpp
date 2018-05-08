/**
 * Title: Rb Tree Test
 * Description: Rb Tree Unit Test
 * Author: F-TD5X(jhx)
 * Update: 2018-05-08 20:23
 **/

#include <assert.h>
#include <bits/stl_function.h>
#include <iostream>
#include "../src/tree.h"

int main() {
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>> tree;
    assert(tree.size() == 0);
    std::cout << "max_size: " << tree.max_size() << std::endl;

    // Insert unique test
    tree.insert_equal(1);
    assert(tree.size() == 1);
    for (int i = 2; i <= 10; i++) tree.insert_unique(i);
    assert(tree.size() == 10);
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>>::iterator
        it = tree.begin();
    for (int i = 1; it != tree.end(); ++it, ++i) assert(*it == i);

    // Cleaer test
    tree.clear();
    assert(tree.size() == 0);

    // Insert qeual test
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= i; j++) tree.insert_equal(i);
    assert(tree.size() == 55);
    for (int i = 1; i <= 10; i++) assert(tree.count(i) == i);

    tree.clear();

    // Find test
    for (int i = 1; i <= 10; i++) tree.insert_unique(i);

    for (int i = 1; i <= 10; i++){
        assert(custl::distance(tree.begin(), tree.lower_bound(i)) == i - 1);
        assert(custl::distance(tree.begin(),tree.upper_bound(i)) == i);
	}
    


    assert(tree.__verify());
}
