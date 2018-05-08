/* file: construct.h
 * date: 2018/04/29
 * author: axp (Xiao Peng)
 */

#ifndef _CUSTL_CONSTRUCT_H
#define _CUSTL_CONSTRUCT_H

#include <new>

#include "iterator.h"
#include "type_traits.h"

namespace custl {
// By axp:
// placement new依赖于全局的void* operator new(sizetype, void*);
// 且此函数不能定义于namespace中，如果手写此函数会导致之后包含stl标准库时
// 因重复定义此函数而报错，故此处引入了stl的new文件。
// 如果之后custl足够完善，可保证不引入标准库文件的情况下也不影响库的开发和
// 使用，可在全局补充该函数。
template <typename T1, typename T2>
inline void construct(T1* p, const T2& value) {
    new(p) T1(value);
}
template <typename T>
inline void destroy(T* p) {
    p->~T();
}
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
    for (; first < last; ++first)
        destroy(&*first);
}
template <typename ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type) {}
template <typename ForwardIterator, typename T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
    __destroy_aux(first, last, typename type_traits<T>::is_trivially_constructible());
}
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}
}; // namespace custl

#endif // _CUSTL_CONSTRUCT_H
