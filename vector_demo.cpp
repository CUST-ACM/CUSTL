/* file: test_vector.h
 * date: 2018/04/29
 * author: axp (Xiao Peng)
 */

#include <iostream>

#include "vector.h"

struct st {
    static int cal;
    int x;
    st(){ ++cal; }
    st(int t):x(t){ ++cal; }
};

int st::cal = 0;

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
    std::cout << st::cal << std::endl;

    custl::vector<int> vint;
    vint.push_back(1);
    vint.push_back(2);
    vint.push_back(3);
    vint.push_back(4);
    vint.push_back(5);
    vint.push_back(6);
    vint.push_back(7);
    vint.pop_back();
    for(custl::vector<int>::iterator it = vint.begin(); it != vint.end(); ++it) {
        std::cout<<(*it)<<std::endl;
    }
    return 0;
}
