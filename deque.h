/*-----------------------------------------------
*   Filename: deque.h
*   Author: zhuyutian
*   Date: 2018.04.30
-----------------------------------------------*/

#ifndef _CUSTL_DEQUE_H
#define _CUSTL_DEQUE_H

#include "config.h"
#include "allocator.h"

namespace custl{

inline size_t __deque_buf_size(size_t n, size_t sz) { 
    return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1)); 
}

template <typename T, typename Ref, typename Ptr, size_t BufSize>
struct __deque_iterator {
    typedef __deque_iterator<T, T&, T*, BufSize>                iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSize>    const_terator;
    static size_t buffer_size() { return __deque_buf_size(BufSize,sizeof(T)); }

    typedef random_access_iterator_tag      iterator_category;
    typedef T                               value_type;
    typedef Ptr                             pointer;
    typedef Ref                             reference;
    typedef size_t                          size_type;
    typedef ptrdiff_t                       difference_type;
    typedef T**                             map_pointer;
    typedef __deque_iterator                self;

    pointer cur;
    pointer first;
    pointer last;
    map_pointer node;

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size());
    }

    reference operator*() const { return *cur; }
    pointer operator->() const { return &(operator*()); }
    difference_type operator-(const self& x) const {
        return difference_type(buffer_size()) * (node - x.node - 1) + 
            (cur - first) + (x.last - x.cur);
    }

    self& operator++() {
        ++cur;
        if(cur == last) {
             set_node(node + 1);
             cur = first;
        }
        return *this;
    }

    self operator++(int) { //后置式写法
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--() {
        if(cur == first){
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int) { 
        self tmp = *this;
        --*this;
        return tmp;
    }

    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if(offset >= 0 && offset < difference_type(buffer_size())) 
            cur += n;
        else {
            difference_type node_offset = 
                offset > 0 ? offset / difference_type(buffer_size()) 
                    : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }

    self operator+(difference_type n) const {
        self tmp = *this;
        return tmp += n;
    }

    self& operator-=(difference_type n) { return *this += -n; }
    
    self operator-(difference_type n) {
        self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(const self& x) const { return cur == x.cur; }
    bool operator!=(const self& x) const { return !(*this == x); }
    bool operator<(const self& x) const {
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }
};

template <typename T, size_t BufSize = 0>
class deque {
public:
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;  
    typedef size_t                          size_type;
    typedef ptrdiff_t                       difference_type;

    typedef __deque_iterator<T, T&, T*,BufSize> iterator;

protected:
    typedef pointer* map_pointer;

    static size_type buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

    iterator start;
    iterator finish;

    map_pointer map;
    size_type map_size;

protected:
    typedef allocator<value_type>   data_allocator;
    typedef allocator<pointer>      map_allocator;
    
    pointer allocate_node() {
        return data_allocator::allocate(buffer_size()); 
    }
    void deallocate_node(pointer buf_node) {
        data_allocator::deallocate(buf_node, buffer_size());
    }

    void create_map_and_nodes(size_type);
    void initialize_map(size_type);
    void reallocate_map(size_type);
    void reserve_map_at_back();
    void reserve_map_at_front();
    void push_back_aux(const value_type&);
    void push_front_aux(const value_type&);
    void pop_front_aux();
    void pop_back_aux();

public:
    deque() { initialize_map(0); }
    ~deque() {
        clear();
		deallocate_node(start.first);
	}

    iterator begin() { return start; }
    iterator end() { return finish; }

    reference operator[](size_type n) {
        return start[difference_type(n)];
    }

    reference front() { return *start; }
    reference back() {
        iterator tmp = finish;
        return *(--tmp);
    }

    size_type size() const { return finish - start; }
    size_type max_size() const { return size_type(-1); }
    bool empty() const { return start == finish; }

    void push_back(const value_type&);
    void push_front(const value_type&);
    void pop_back();
    void pop_front();
    void clear();
};

//TODO(zhuyutian) 初版
template <typename T, size_t BufSize>
void deque<T, BufSize>::create_map_and_nodes(size_type num_elememts) {
    
    const size_type num_nodes = num_elememts / buffer_size() + 1;
    
    map_size = num_nodes + 2;
    map = map_allocator::allocate(map_size);
    
    map_pointer nstart = map + 1;
    map_pointer nfinish = nstart + num_nodes - 1;
    
    try {
        for(map_pointer cur = nstart; cur <= nfinish; ++cur)
            *cur = allocate_node();
    } catch(map_pointer mp) {
        //...
    }

    start.set_node(nstart);
    finish.set_node(nfinish);
    start.cur = start.first;
    finish.cur = finish.first + num_elememts % buffer_size();
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::initialize_map(size_type num_elememts) {
    create_map_and_nodes(num_elememts);
}

//TODO(zhuyutian) 简化版策略
//中间的copy操作手动实现了，等日后空间分配器实现该函数
//可以直接调用
template <typename T, size_t BufSize>
void deque<T, BufSize>::reallocate_map(size_type nodes_to_add) {

    size_type old_num_nodes = finish.node - start.node + 1;
    size_type new_map_size = map_size + nodes_to_add;

    map_pointer new_map = map_allocator::allocate(new_map_size);
    map_pointer new_nstart = new_map + 1;
    map_pointer cur = start.node;
    map_pointer new_cur = new_nstart;   

    while(cur <= finish.node) {
        *new_cur = *cur;
        ++cur;
        ++new_cur;
    }

    map_allocator::deallocate(map, map_size);

    map = new_map;
    map_size = new_map_size;

    start.set_node(new_nstart);
    finish.set_node(new_nstart + old_num_nodes - 1);
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::reserve_map_at_back() {
    size_type nodes_to_add = 1;
    if(nodes_to_add + 1 > map_size - (finish.node - map))
        reallocate_map(nodes_to_add);
} 

template <typename T, size_t BufSize>
void deque<T, BufSize>::reserve_map_at_front() {
    size_type nodes_to_add = 1;
    if(nodes_to_add > size_type(start.node - map))
        reallocate_map(nodes_to_add);
}

//TODO(zhuyutian) 简化版策略
template <typename T, size_t BufSize>
void deque<T, BufSize>::push_back_aux(const value_type& t) {
    
    reserve_map_at_back();
    
    *(finish.node + 1) = allocate_node();
    
    construct(finish.cur, t);
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::push_back(const value_type& t) {
    if(finish.cur != finish.last - 1) {
        construct(finish.cur,t);
        ++finish.cur;
    } 
    else
        push_back_aux(t);
}

//TODO(zhuyutian) 简化版策略
template <typename T, size_t BufSize>
void deque<T, BufSize>::push_front_aux(const value_type& t) {
    
    reserve_map_at_front();

    *(start.node - 1) = allocate_node();
    
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    construct(start.cur, t);
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::push_front(const value_type& t) {
    if(start.cur != start.first) {
        construct(start.cur - 1,t);
        --start.cur;
    }
    else
        push_front_aux(t);
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::pop_front_aux() {
    destroy(start.cur);
    deallocate_node(start.first);
    start.set_node(start.node + 1);
    start.cur = start.first;
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::pop_front() {
    if(start.cur != start.last) {
        destroy(start.cur);
        ++start.cur;
    }
    else
        pop_front_aux();
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::pop_back_aux() {
    deallocate_node(finish.first);
    finish.set_node(finish.node - 1);
    finish.cur = finish.last;
    destroy(finish.cur);
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::pop_back() {
    if(finish.cur != finish.first) {
        --finish.cur;
        destroy(finish.cur);
    }
    else
        pop_back_aux();
}

template <typename T, size_t BufSize>
void deque<T, BufSize>::clear() {
    for(map_pointer cur = start.node + 1; cur < finish.node; ++cur) {
        destroy(*cur, *cur + buffer_size());
        deallocate_node(*cur);
    }
    
    if(start.node != finish.node) {
        destroy(start.cur, start.last);
        destroy(finish.first, finish.cur);
        deallocate_node(finish.first);
    }
    else
        destroy(start.cur, finish.cur);
    finish = start;
}

} //namespace custl

#endif








