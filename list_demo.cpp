/*-----------------------------------------------
					Filename: list_demo.cpp
					Author: TanYz
					Date: 2018.05.07
-----------------------------------------------*/

#include <iostream>

#include "list.h"

custl::list<int> l;
int n = 10;

void print()
{
	for(custl::list<int>::iterator it = l.begin(); it != l.end(); it++) {
		std::cout << (*it) << " ";
	}
	std::cout << std::endl;
}
int main()
{
	for(int i = 1; i <= n; i++) l.push_back(i);
	for(int i = n; i > 0; i--) l.push_front(i);
	print();
	
	custl::list<int>::iterator it = l.begin();
	it++;
	l.insert(it, 233); print();

	it = l.begin(); it--; it--;
	l.insert(it, 2333); print();
	
	it = l.begin();
	it++; it++;
	l.erase(it); print();
	
	l.pop_front(); print();
	l.pop_back(); print();
	std::cout << l.size() << std::endl;
	l.clear();
	std::cout << l.size() << std::endl;
	
	for(int i = 0; i < n; i++) {
		l.push_back(233);
	}
	print();
	return 0;
}

