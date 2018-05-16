/* file: list.h
 * date: 2018/05/05
 * author: TanYz
 */

#ifndef _CUSTL_LIST_H
#define _CUSTL_LIST_H

#include "allocator.h"
#include "construct.h"
#include "iterator.h"

namespace custl {

template<typename T>
struct _list_node {
    _list_node<T>* _next;
    _list_node<T>* _prev;
    T _data;
};

template<typename T, typename ref, typename ptr>
struct _list_iterator {
    typedef _list_iterator<T, T&, T*>         _self;
    typedef _list_iterator<T, ref, ptr>       iterator;
    typedef ptrdiff_t                         difference_type;
    typedef T                                 value_type;
    typedef T*                                pointer;
    typedef T&                                reference;
    typedef bidirectional_iterator_tag        iterator_category;
    typedef _list_node<T>*                    link_type;

    link_type _node;
    
    _list_iterator() {}
    _list_iterator(link_type _x): _node(_x) {}
    _list_iterator(const iterator& _x): _node(_x._node) {}

    bool operator == (const _self& _rhs) const {
        return _node == _rhs._node;
    }   
    bool operator != (const _self& _rhs) const {
        return _node != _rhs._node;
    }
    reference operator *() const {
        return (*_node)._data;
    }
    pointer operator ->() const {
        return &(operator *());
    }
    _self& operator ++() {
        _node = (link_type) ((*_node)._next);
        return *this;
    }
    _self operator ++(int) {
        _self tmp = *this;
        ++(*this);
        return tmp;
    }
    _self& operator --() {
        _node = (link_type) ((*_node)._prev);
        return *this;
    }
    _self operator --(int) {
        _self tmp = *this;
        --(*this);
        return tmp;
    }
};

template<typename T, typename Alloc = allocator<T> >
class list {
public:
    typedef T                                         value_type;
    typedef T&                                        reference;
    typedef const T&                                  const_reference;
    typedef T*                                        pointer;
    typedef _list_iterator<T, T&, T*>                 iterator;
    typedef const _list_iterator<T, T&, T*>           const_iterator;
    typedef size_t                                    size_type;
    typedef ptrdiff_t                                 difference_type;
    typedef _list_node<T>*                            link_type;

protected:
    typedef allocator<_list_node<T> >                 _list_node_allocator;
    typedef _list_node<T>                             list_node;
    
    link_type _node;
    
    static link_type allocate() {
        return (link_type)Alloc::allocate(sizeof(list_node));
    }
    link_type _get_node() { return allocate(); }
    void _put_node(link_type _p) {
        _list_node_allocator::deallocate(_p, sizeof(_p));
    }
    link_type _create_node(const value_type& _x) {
        link_type _p = this->_get_node();
        construct(&_p->_data, _x);
        return _p;
    }
    void _destroy_node(link_type _p) {
        destroy(&_p->_data);
        _put_node(_p);
    }
    void _empty_initalize() {
        _node = _get_node();
        _node->_next = _node;
        _node->_prev = _node;
    }

public:
    list() { _empty_initalize(); }

    iterator begin() { return iterator(_node->_next); }
    iterator end() { return iterator(_node); }
    bool empty() const { return _node->_next == _node; }
    size_type size() {
        size_type res = 0;
        link_type cur = _node->_next;
        while (cur != _node) {
            ++res;
            cur = cur->_next;
        }
        return res;
    }
    reference front() { return *begin(); }
    reference back() { return *(--end()); }
    void clear() {
        link_type cur = _node->_next;
        while (cur != _node) {
            link_type tmp = cur;
            cur = cur->_next;
            _destroy_node(tmp);
        }
        _empty_initalize();
    }
    iterator insert(const_iterator pos, const value_type& x) {
        link_type prev_node = pos._node->_prev;
        link_type tmp = _create_node(x);
        pos._node->_prev = tmp;
        tmp->_next = pos._node;
        tmp->_prev = prev_node;
        prev_node->_next = tmp;
        return iterator(tmp);
    }
    void push_front(const value_type& x) { this->insert(begin(), x); }
    void push_back(const value_type& x) { this->insert(end(), x); }
    iterator erase(iterator pos) {
        link_type prev_node = pos._node->_prev;
        link_type next_node = pos._node->_next;
        prev_node->_next = next_node;
        next_node->_prev = prev_node;
        _destroy_node(pos._node);
        return iterator(next_node);
    }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }
};

} //namspace custl

#endif // _CUSTL_LIST_H
