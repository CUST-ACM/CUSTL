/* file: allocator.h
 * date: 2018/04/26
 * author: axp (Xiao Peng)
 */

#ifndef _CUSTL_ALLOCATOR_H
#define _CUSTL_ALLOCATOR_H

#include "config.h"
#include "construct.h"

namespace custl {

template <typename T>
class allocator {
public:
    typedef T           value_type;
    typedef size_t      size_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef ptrdiff_t   difference_type;

    allocator(){}
    ~allocator(){}

    static pointer allocate(size_type n, const void* = 0) {
        return static_cast<pointer>(operator new (n * sizeof(T)));
    }
    static void deallocate(pointer p, size_type) {
        delete p;
    }
    static pointer address(reference x) {
        return static_cast<pointer>(&x);
    }
};

template<>
class allocator<void> {
public:
    typedef void* pointer;
};

} // namespace custl

#endif // _CUSTL_ALLOCATOR_H
