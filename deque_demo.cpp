
#include "deque.h"
#include <iostream>

int main(){
    custl::deque<int> Q;
	Q.clear();
    std::cout << Q.size() << '\n';
    for(int i = 0; i < 1001; i++)
        Q.push_back(1);
    std::cout << *Q.begin() << '\n';
    std::cout << Q.back() << '\n';
    std::cout << Q.front() << '\n';
    std::cout << Q.size() << '\n';
    Q.pop_back();
    std::cout << Q.size() << '\n';
    Q.pop_front();
    std::cout << Q.size() << '\n';
    Q.clear();
    std::cout<<Q.size()<<'\n';
    for(int i = 0; i < 10; i++) {
        if(i > 5) Q.push_back(i);
        else Q.push_front(i);
    }
    for(custl::deque<int>::iterator it = Q.begin(); it != Q.end(); it++)
        std::cout << *it << ' ';
    std::cout << '\n';
    return 0;
}




