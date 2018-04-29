/* file: test_vector.h
 * date: 2018/04/29
 * author: axp (Xiao Peng)
 */

#include <iostream>

#include "vector.h"

struct st {
    int x;
    st(){}
    st(int t):x(t){}
};

int main() {
    custl::vector<st> ve;
    ve.push_back(st(100));
    ve.push_back(st(50));
    std::cout << ve[0].x << std::endl;
    std::cout << ve[1].x << std::endl;
    std::cout << ve.size() << std::endl;
    ve.pop_back();
    std::cout << ve[0].x << std::endl;
    std::cout << ve.size() << std::endl;
    return 0;
}
