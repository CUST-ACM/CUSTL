/* file: list_demo.cpp
 * date: 2018/05/07
 * author: TanYz
 */

#include <iostream>

#include "list.h"

custl::list<int> List;
int test_num = 10;

void print()
{
    for (custl::list<int>::iterator it = List.begin(); it != List.end(); ++it) {
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
}

int main()
{
    for (int i = 1; i <= test_num; i++) List.push_back(i);
    for (int i = test_num; i > 0; i--) List.push_front(i);
    print();
    
	csutl::list<int>::iterator it = List.begin();
    it++;
    List.insert(it, 233); print();

    it = List.begin();
    it--;
    it--;
    List.insert(it, 2333);
    print();
    
    it = List.begin();
    it++;
    it++;
    List.erase(it);
    print();
    
    List.pop_front();
    print();
    
    List.pop_back();
    print();
    
    std::cout << List.size() << std::endl;
    List.clear();
    std::cout << List.size() << std::endl;
    
    if (List.empty())
        std::cout << "empty!" << std::endl;
    
    for (int i = 0; i < test_num; ++i) {
        List.push_back(233);
    }
    print();
    return 0;
}

