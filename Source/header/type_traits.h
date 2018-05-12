/* file: type_traits.h
 * date: 2018/04/30
 * author: axp (Xiao Peng)
 */

#ifndef _CUSTL_TYPE_TRAITS_H
#define _CUSTL_TYPE_TRAITS_H

namespace custl {

struct __true_type {};
struct __false_type {};

template <typename T>
struct type_traits {
    typedef __false_type      is_trivially_constructible;
};
template<>
struct type_traits<char> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<signed char> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<unsigned char> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<short> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<unsigned short> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<int> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<unsigned int> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<long long> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<unsigned long long> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<float> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<double> {
    typedef __true_type       is_trivially_constructible;
};
template<>
struct type_traits<long double> {
    typedef __true_type       is_trivially_constructible;
};
template <typename T>
struct type_traits<T*> {
    typedef __true_type       is_trivially_constructible;
};

} // namespace custl

#endif // _CUSTL_TYPE_TRAITS_H
