/**
 * file: set_test.cpp
 * date: 2018-05-16
 * author: F_TD5X(jhx)
 **/

#include <assert.h>
#include <iostream>
#include <vector>

#include "set.h"

int main() {
    // test 1 default construct, insert and find
    custl::set<int> s1;
    assert(s1.empty());
    std::cout << s1.max_size() << std::endl;
    for (int i = 0; i < 10; i++) s1.insert(i);
    assert(*s1.begin() == 0);
    assert(s1.size() == 10);
    assert(s1.lower_bound(0) == s1.begin());
    assert(s1.lower_bound(10) == s1.end());
    assert(*s1.upper_bound(8) == 9);
    s1.insert(1);
    assert(s1.size() == 10);
    s1.clear();
    assert(s1.empty());

    // test 2 copy construct
    for (int i = 0; i < 10; i++) s1.insert(i);
    custl::set<int> s2(s1.begin(), s1.end());
    assert(s2.size() == 10);
    custl::set<int, std::greater<int>> s3(s1.begin(), s1.end(),
                                          std::greater<int>());
    assert(*s3.begin() == 9);
<<<<<<< HEAD
    // test swap
    swap(s1,s2);

    std::cout << "Set test passed" << std::endl;
}
=======

    std::cout << "Set test passed" << std::endl;
}
>>>>>>> 0906d1f40265f4782aa8f7e8533ab49125c99c2f
