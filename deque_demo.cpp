/* file: deque_demo.cpp
 * date: 2018/05/08
 * author: zhuyutian
 */

#include <iostream>

#include "deque.h"

struct Node {
    int x;
    Node(int x):x(x){}
};

int main(){
    custl::deque<Node> dqnode;
    dqnode.push_back(Node(1));
    dqnode.push_front(Node(2));
    dqnode.push_back(Node(3));
    dqnode.push_front(Node(4));
    std::cout << dqnode[0].x << '\n';
    std::cout << dqnode.front().x << '\n';
    std::cout << dqnode.back().x << '\n';
    std::cout << dqnode.size() << '\n';
    dqnode.pop_back();
    std::cout << dqnode.front().x << '\n';
    std::cout << dqnode.back().x << '\n';
    std::cout << dqnode.size() << '\n'; 

    custl::deque<int> dqint;
    dqint.clear();
    std::cout << dqint.size() << '\n';
    for(int i = 0; i < 1001; i++)
        dqint.push_back(i);
    std::cout << dqint[0] << '\n';
    std::cout << dqint[1] << '\n';
    std::cout << *dqint.begin() << '\n';
    std::cout << dqint.back() << '\n';
    std::cout << dqint.front() << '\n';
    std::cout << dqint.size() << '\n';
    dqint.pop_back();
    std::cout << dqint.size() << '\n';
    dqint.pop_front();
    std::cout << dqint.size() << '\n';
    dqint.clear();
    std::cout << dqint.size() << '\n';
    
    for(int i = 0; i < 10; i++) {
        if(i > 5) dqint.push_back(i);
        else dqint.push_front(i);
    }
    for(custl::deque<int>::iterator it = dqint.begin(); it != dqint.end(); it++)
        std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}




