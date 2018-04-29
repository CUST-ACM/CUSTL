/* file: construct.h
 * date: 2018/04/29
 * author: axp (Xiao Peng)
 */

#ifndef _CUSTL_CONSTRUCT_H
#define _CUSTL_CONSTRUCT_H

#include "iterator.h"

namespace custl {
template <typename T1, typename T2>
inline void construct(T1* p, const T2& value) {
    new (p) T1(value);
}
template <typename T>
inline void destory(T* p) {
    p->~T();
}
template <typename ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last) {
    for (; first < last; ++first)
        destory(&*first);
}
//TODO(axp):
//需要增加type_traits判断trival_type，现在默认为non-trival_type
template <typename ForwardIterator, typename T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*) {
    __destory_aux(first, last);
}
template <typename ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
    __destory(first, last, value_type(first));
}
}; // namespace custl

#endif // _CUSTL_CONSTRUCT_H
