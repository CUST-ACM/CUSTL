/* file: vector.h
 * date: 2018/04/28
 * author: axp (Xiao Peng)
 */

#ifndef _CUSTL_VECTOR_H
#define _CUSTL_VECTOR_H

#include "allocator.h"

namespace custl {

template<typename T, typename Alloc = allocator<T> >
class vector {
public:
    typedef T                           value_type;
    typedef T&                          reference;
    typedef const T&                    const_reference;
    typedef T*                          pointer;
    typedef T*                          iterator;
    typedef const T*                    const_iterator;
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    
protected:
    typedef Alloc data_allocator;
    
    iterator start;
    iterator finish;
    iterator end_of_storage;

    void insert_aux(iterator position, const T& x);
    void deallocate() {
        if (start)
            data_allocator::deallocate(start, end_of_storage - start);
    }
    void fill_initialize(size_type n, const T& x) {
        start = allocate_and_fill(n, x);
        finish = start + n;
        end_of_storage = finish;
    }
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator result = data_allocator::allocate(n);
        uninitialized_fill_n(result, n, x);
        return result;
    }

public:
    vector() : start(0), finish(0), end_of_storage(0) {}
    ~vector(){
        destroy(start, finish);
        deallocate();
    }

    size_type size() const { return size_type(finish - start); }
    size_type capacity() const { return size_type(end_of_storage - start); }
    bool empty() const { return start == finish; }
    iterator begin() { return start; }
    iterator end() { return finish; }
    reference front() { return *start; }
    reference back() { return *(finish - 1); }
    reference operator[](size_type n) { return *(start + n); }

    void push_back(const value_type& x) {
        if (finish != end_of_storage) {
            construct(finish, x);
            ++finish;
        } else {
            insert_aux(finish, x);
        }
    }
    void pop_back() {
        --finish;
        destroy(finish);
    }
    iterator insert(const_iterator position, const value_type& x) {
        int n = position - begin();
        insert_aux(position, x);
        return begin() + n;
    }
};

//TODO(axp)
//insert和insert_aux先随便写个了，之后再更
template<typename T, typename Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x) {
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        for (iterator it = position + 1; it < finish; ++it)
            *it = *(it - 1);
        *position = x;
    } else {
        const size_type old_size = size();
        const size_type new_size = old_size == 0 ? 1 : 2 * old_size;

        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish = new_start;

        for (iterator it = begin(); it != end(); ++it)
        {
            if (it == position) {
                *new_finish = x;
                ++new_finish;
            }
            *new_finish = *it;
            ++new_finish;
        }
        if (position == end()) {
            *new_finish = x;
            ++new_finish;
        }

        destroy(start, finish);
        deallocate();

        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

}; // namespace custl

#endif // _CUSTL_VECTOR_H
