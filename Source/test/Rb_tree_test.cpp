/**
 * file: Rb_tree_test.h
 * date: 2018-05-08 20:11
 * author: F-TD5X(jhx)
 **/

#include <assert.h>
#include <iostream>
#include <utility>
#include "tree.h"

struct Node {
    int a;
    Node() { a = 0; }
    Node(int a) : a(a) {}
    bool operator<(const Node &t) const { return a < t.a; }
};

struct cmp{
    bool operator()(Node a,Node b){return a.a < b.a;}
};

int main() {
    // Init
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>> tree;
    assert(tree.size() == 0);
    std::cout << "max_size: " << tree.max_size() << std::endl;

    tree.insert_unique(1);
    tree.clear();

    // Insert unique test
    tree.insert_equal(1);
    assert(tree.size() == 1);
    for (int i = 2; i <= 10; i++) tree.insert_unique(i);
    assert(tree.size() == 10);
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>>::iterator
        it = tree.begin();
    for (int i = 1; it != tree.end(); ++it, ++i) assert(*it == i);
    assert(*(++tree.lower_bound(2)) == 3);
    assert(*tree.upper_bound(2) == 3);

    // Cleaer test
    tree.clear();
    assert(tree.size() == 0);

    // Insert qeual test
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= i; j++) tree.insert_equal(i);
    assert(tree.size() == 55);
    for (int i = 1; i <= 10; i++) assert(tree.count(i) == (unsigned int)i);

    tree.clear();

    // Find test
    for (int i = 1; i <= 10; i++) tree.insert_unique(i);

    for (int i = 1; i <= 10; i++) {
        assert(custl::distance(tree.begin(), tree.lower_bound(i)) == i - 1);
        assert(custl::distance(tree.begin(), tree.upper_bound(i)) == i);
    }

    // erase test
    tree.erase(tree.find(1));
    assert(tree.find(1) == tree.end());
    assert(*tree.begin() == 2);

    // erase range test
    tree.erase(tree.lower_bound(1), tree.lower_bound(5));
    assert(tree.size() == 6);
    {
        int t = 5;
        for (auto it = tree.begin(); it != tree.end(); ++it, ++t)
            assert(*it == t);
    }

    // Structure verify
    assert(tree.__verify());
    tree.clear();

    // copy test
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>> t1;
    for (int i = 1; i <= 10; i++) t1.insert_unique(i);
    custl::_Rb_tree<int, int, std::_Identity<int>, std::less<int>> t2(t1);
    {
        int t = 1;
        for (auto it = t2.begin(); it != t2.end(); it++, t++) assert(*it == t);
    }

    // structure

    custl::rb_tree<Node, Node, std::_Identity<Node>, std::less<Node>> t3;
    for (int i = 1; i <= 10; i++) t3.insert_unique(Node(i));
    assert((*(++t3.begin())).a == 2);

    // Compare Function
    custl::_Rb_tree<Node,Node,std::_Identity<Node>,cmp> t4;
    for(int i=1;i<=10;i++)
        t4.insert_unique(Node(i));
    assert((*(++t4.begin())).a == 2);

    //swap test
    t1.swap(tree);

    std::cout << "Rb tree Unit test passed" << std::endl;
}
