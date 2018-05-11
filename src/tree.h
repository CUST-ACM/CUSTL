/**
 * Title: Rb Tree
 * Description: CUSTL Red-Black Tree
 * Author: F-TD5X(jhx)
 * Update: 2018-05-08 20:11
 **/

#ifndef _CUSTL_TREE_H
#define _CUSTL_TREE_H

#include <iterator>
#include <utility>
#include "../allocator.h"

namespace custl {

enum _Rb_tree_color { red = false, black = true };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;

    _Rb_tree_color _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;

    static _Base_ptr _S_minimum(_Base_ptr __x) {
        while (__x->_M_left != nullptr) __x = __x->_M_left;
        return __x;
    }

    static _Base_ptr _S_maximum(_Base_ptr __x) {
        while (__x->_M_right != nullptr) __x = __x->_M_right;
        return __x;
    }
};

template <typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base {
    typedef _Rb_tree_node<_Val>* _Link_type;
    _Val _M_Val_field;
    _Val* _M_valptr() { return std::__addressof(_M_Val_field); }
    const _Val* _M_valptr() const { return std::__addressof(_M_Val_field); }
};

template <typename _Val>
struct _Rb_tree_iterator {
    typedef _Val value_type;
    typedef _Val& reference;
    typedef _Val* pointer;

    typedef _Rb_tree_iterator<_Val> _Self;
    typedef _Rb_tree_node<_Val>* _Link_type;

    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Base_ptr __x) : _M_node(__x) {}
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) : _M_node(__it._M_node) {}

    reference operator*() const {
        return *static_cast<_Link_type>(_M_node)->_M_valptr();
    }
    pointer operator->() const {
        return static_cast<_Link_type>(_M_node)->_M_valptr();
    }

    _Self operator++() {
        _M_increment();
        return *this;
    }
    _Self operator++(int) {
        _Self __tmp = *this;
        _M_increment();
        return __tmp;
    }
    _Self operator--() {
        _M_decrement();
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        _M_decrement();
        return __tmp;
    }

    void _M_increment() {
        if (_M_node->_M_right != nullptr) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != nullptr) _M_node = _M_node->_M_left;
        } else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y) _M_node = __y;
        }
    }

    void _M_decrement() {
        if (_M_node->_M_color == red && _M_node->_M_parent != nullptr &&
            _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_right != nullptr) {
            _Base_ptr __y = _M_node->_M_left;
            while (__y->_M_right != nullptr) __y = __y->_M_right;
            _M_node = __y;
        } else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
    }

    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_iterator& __x) const {
        return _M_node == __x._M_node;
    }
    bool operator!=(const _Rb_tree_iterator& __x) const {
        return _M_node != __x._M_node;
    }
};

template <typename _Val>
struct _Rb_tree_const_iterator {
    typedef _Val value_type;
    typedef const _Val& reference;
    typedef const _Val* pointer;

    typedef _Rb_tree_iterator<_Val> iterator;

    typedef _Rb_tree_const_iterator<_Val> _Self;
    typedef const _Rb_tree_node<_Val>* _Link_type;
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    _Rb_tree_const_iterator() {}
    _Rb_tree_const_iterator(const _Base_ptr __x) : _M_node(__x) {}
    _Rb_tree_const_iterator(const _Self& __it) : _M_node(__it._M_node) {}
    _Rb_tree_const_iterator(const iterator& __it) : _M_node(__it._M_node) {}

    iterator _M_const_cast() const {
        return iterator(const_cast<typename iterator::_Base_ptr>(_M_node));
    }

    reference operator*() const {
        return *static_cast<_Link_type>(_M_node)->_M_valptr();
    }
    pointer operator->() const {
        return static_cast<_Link_type>(_M_node)->_M_valptr();
    }
    _Self operator++() {
        _M_increment();
        return *this;
    }
    _Self operator++(int) {
        _Self __tmp = *this;
        _M_increment();
        return __tmp;
    }
    _Self operator--() {
        _M_decrement();
        return *this;
    }
    _Self operator--(int) {
        _Self __tmp = *this;
        _M_decrement();
        return __tmp;
    }

    void _M_increment() {
        if (_M_node->_M_right != nullptr) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != nullptr) _M_node = _M_node->_M_left;
        } else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y) _M_node = __y;
        }
    }

    void _M_decrement() {
        if (_M_node->_M_color == red &&
            _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_right != nullptr) {
            _Base_ptr __y = _M_node->_M_left;
            while (__y->_M_right != nullptr) __y = __y->_M_right;
            _M_node = __y;
        } else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
    }

    _Base_ptr _M_node;

    bool operator==(const _Rb_tree_const_iterator& __x) const {
        return _M_node == __x._M_node;
    }
    bool operator!=(const _Rb_tree_const_iterator& __x) const {
        return _M_node != __x._M_node;
    }
};

template <typename _T>
inline bool operator==(const _Rb_tree_iterator<_T>& __x,
                       const _Rb_tree_const_iterator<_T>& __y) {
    return __x._M_node == __y._M_node;
}

template <typename _T>
inline bool operator!=(const _Rb_tree_iterator<_T>& __x,
                       const _Rb_tree_const_iterator<_T>& __y) {
    return __x._M_node != __y._M_node;
}

inline void _Rb_tree_rotate_left(_Rb_tree_node_base* __x,
                                 _Rb_tree_node_base*& __root) {
    _Rb_tree_node_base* __y = __x->_M_right;
    __x->_M_right = __y->_M_left;
    if (__y->_M_left != 0) __y->_M_left->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;

    if (__x == __root)
        __root = __y;
    else if (__x == __x->_M_parent->_M_left)
        __x->_M_parent->_M_left = __y;
    else
        __x->_M_parent->_M_right = __y;
    __y->_M_left = __x;
    __x->_M_parent = __y;
}

inline void _Rb_tree_rotate_right(_Rb_tree_node_base* __x,
                                  _Rb_tree_node_base*& __root) {
    _Rb_tree_node_base* __y = __x->_M_left;
    __x->_M_left = __y->_M_right;
    if (__y->_M_right != 0) __y->_M_right->_M_parent = __x;
    __y->_M_parent = __x->_M_parent;

    if (__x == __root)
        __root = __y;
    else if (__x == __x->_M_parent->_M_right)
        __x->_M_parent->_M_right = __y;
    else
        __x->_M_parent->_M_left = __y;
    __y->_M_right = __x;
    __x->_M_parent = __y;
}

inline void _Rb_tree_rebalance(_Rb_tree_node_base* __x,
                               _Rb_tree_node_base*& __root) {
    __x->_M_color = red;
    while (__x != __root && __x->_M_parent->_M_color == red) {
        if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_right;
            if (__y && __y->_M_color == red) {
                __x->_M_parent->_M_color = black;
                __y->_M_color = black;
                __x->_M_parent->_M_parent->_M_color = red;
                __x = __x->_M_parent->_M_parent;
            } else {
                if (__x == __x->_M_parent->_M_right) {
                    __x = __x->_M_parent;
                    _Rb_tree_rotate_left(__x, __root);
                }
                __x->_M_parent->_M_color = black;
                __x->_M_parent->_M_parent->_M_color = red;
                _Rb_tree_rotate_right(__x->_M_parent->_M_parent, __root);
            }
        } else {
            _Rb_tree_node_base* __y = __x->_M_parent->_M_parent->_M_left;
            if (__y && __y->_M_color == red) {
                __x->_M_parent->_M_color = black;
                __y->_M_color = black;
                __x->_M_parent->_M_parent->_M_color = red;
                __x = __x->_M_parent->_M_parent;
            } else {
                if (__x == __x->_M_parent->_M_left) {
                    __x = __x->_M_parent;
                    _Rb_tree_rotate_right(__x, __root);
                }
                __x->_M_parent->_M_color = black;
                __x->_M_parent->_M_parent->_M_color = red;
                _Rb_tree_rotate_left(__x->_M_parent->_M_parent, __root);
            }
        }
    }
    __root->_M_color = black;
}

inline _Rb_tree_node_base* _Rb_tree_rebalance_for_erase(
    _Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
    _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost) {
    _Rb_tree_node_base* __y = __z;
    _Rb_tree_node_base* __x = 0;
    _Rb_tree_node_base* __x_parent = 0;
    if (__y->_M_left == 0)
        __x = __y->_M_right;
    else if (__y->_M_right == 0)
        __x = __y->_M_left;
    else {
        __y = __y->_M_right;
        while (__y->_M_left != 0) __y = __y->_M_left;
        __x = __y->_M_right;
    }
    if (__y != __z) {
        __z->_M_left->_M_parent = __y;
        __y->_M_left = __z->_M_left;
        if (__y != __z->_M_right) {
            __x_parent = __y->_M_parent;
            if (__x) __x->_M_parent = __y->_M_parent;
            __y->_M_parent->_M_left = __x;
            __y->_M_right = __z->_M_right;
            __z->_M_right->_M_parent = __y;
        } else
            __x_parent = __y;
        if (__root == __z)
            __root = __y;
        else if (__z->_M_parent->_M_left == __z)
            __z->_M_parent->_M_left = __y;
        else
            __z->_M_parent->_M_right = __y;
        __y->_M_parent = __z->_M_parent;
        std::swap(__y->_M_color, __z->_M_color);
        __y = __z;
    } else {
        __x_parent = __y->_M_parent;
        if (__x) __x->_M_parent = __y->_M_parent;
        if (__root == __z)
            __root = __x;
        else if (__z->_M_parent->_M_left == __z)
            __z->_M_parent->_M_left = __x;
        else
            __z->_M_parent->_M_right = __x;
        if (__leftmost == __z) {
            if (__z->_M_right == 0)
                __leftmost = __z->_M_parent;

            else
                __leftmost = _Rb_tree_node_base::_S_minimum(__x);
        }
        if (__rightmost == __z) {
            if (__z->_M_left == 0)
                __rightmost = __z->_M_parent;

            else
                __rightmost = _Rb_tree_node_base::_S_maximum(__x);
        }
    }
    if (__y->_M_color != red) {
        while (__x != __root && (__x == 0 || __x->_M_color == black))
            if (__x == __x_parent->_M_left) {
                _Rb_tree_node_base* __w = __x_parent->_M_right;
                if (__w->_M_color == red) {
                    __w->_M_color = black;
                    __x_parent->_M_color = red;
                    _Rb_tree_rotate_left(__x_parent, __root);
                    __w = __x_parent->_M_right;
                }
                if ((__w->_M_left == 0 || __w->_M_left->_M_color == black) &&
                    (__w->_M_right == 0 || __w->_M_right->_M_color == black)) {
                    __w->_M_color = red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_right == 0 ||
                        __w->_M_right->_M_color == black) {
                        if (__w->_M_left) __w->_M_left->_M_color = black;
                        __w->_M_color = red;
                        _Rb_tree_rotate_right(__w, __root);
                        __w = __x_parent->_M_right;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = black;
                    if (__w->_M_right) __w->_M_right->_M_color = black;
                    _Rb_tree_rotate_left(__x_parent, __root);
                    break;
                }
            } else {
                _Rb_tree_node_base* __w = __x_parent->_M_left;
                if (__w->_M_color == red) {
                    __w->_M_color = black;
                    __x_parent->_M_color = red;
                    _Rb_tree_rotate_right(__x_parent, __root);
                    __w = __x_parent->_M_left;
                }
                if ((__w->_M_right == 0 || __w->_M_right->_M_color == black) &&
                    (__w->_M_left == 0 || __w->_M_left->_M_color == black)) {
                    __w->_M_color = red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_left == 0 || __w->_M_left->_M_color == black) {
                        if (__w->_M_right) __w->_M_right->_M_color = black;
                        __w->_M_color = red;
                        _Rb_tree_rotate_left(__w, __root);
                        __w = __x_parent->_M_left;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = black;
                    if (__w->_M_left) __w->_M_left->_M_color = black;
                    _Rb_tree_rotate_right(__x_parent, __root);
                    break;
                }
            }
        if (__x) __x->_M_color = black;
    }
    return __y;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc = allocator<_Val>>
class _Rb_tree {
   protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_color _Color_type;
    typedef _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> _Self;

   public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type* const_pointer;
    typedef const value_type& const_reference;
    typedef _Rb_tree_node<_Val>* _Link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Alloc allocator_type;

    allocator_type get_allocator() const { return allocator_type(); }

   protected:
    _Link_type _M_header;
    _Link_type _M_get_node() {
        return allocator<_Rb_tree_node<_Val>>::allocate(1);
    }
    void _M_put_node(_Link_type __p) { delete __p; }
    _Link_type _M_create_node(const value_type& __x) {
        _Link_type __tmp = _M_get_node();
        try {
            construct(&__tmp->_M_Val_field, __x);
        } catch (const char*) {
            _M_put_node(__tmp);
        }
        return __tmp;
    }

    _Link_type _M_clone_node(_Link_type __x) {
        _Link_type __tmp = _M_create_node(__x->_M_Val_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = nullptr;
        __tmp->_M_right = nullptr;
        return __tmp;
    }

    void destory_node(_Link_type __p) {
        destory(&__p->_M_Val_field);
        _M_put_node(__p);
    }

   protected:
    size_type _M_node_count;
    _Compare _M_key_compare;

    _Link_type& _M_root() const { return (_Link_type&)_M_header->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header->_M_left; }
    _Link_type& _M_rightmost() const {
        return (_Link_type&)_M_header->_M_right;
    }

    static _Link_type& _S_left(_Link_type __x) {
        return (_Link_type&)(__x->_M_left);
    }
    static _Link_type& _S_right(_Link_type __x) {
        return (_Link_type&)(__x->_M_right);
    }
    static _Link_type& _S_parent(_Link_type __x) {
        return (_Link_type&)(__x->_M_parent);
    }
    static reference _S_Val(_Link_type __x) { return __x->_M_Val_field; }
    static const _Key& _S_key(_Link_type __x) {
        return _KeyOfValue()(_S_Val(__x));
    }
    static _Color_type& _S_color(_Link_type __x) {
        return (_Color_type&)(__x->_M_color);
    }

    static _Link_type& _S_left(_Base_ptr __x) {
        return (_Link_type&)(__x->_M_left);
    }
    static _Link_type& _S_right(_Base_ptr __x) {
        return (_Link_type&)(__x->_M_right);
    }
    static _Link_type& _S_parent(_Base_ptr __x) {
        return (_Link_type&)(__x->_M_parent);
    }
    static reference _S_Val(_Base_ptr __x) {
        return ((_Link_type)__x)->_M_Val_field;
    }
    static const _Key& _S_key(_Base_ptr __x) {
        return _KeyOfValue()(_S_Val(_Link_type(__x)));
    }
    static _Color_type& _S_color(_Base_ptr __x) {
        return (_Color_type&)(_Link_type(__x)->_M_color);
    }

    static _Link_type _S_minimum(_Link_type __x) {
        return (_Link_type)_Rb_tree_node_base::_S_minimum(__x);
    }

    static _Link_type _S_maximum(_Link_type __x) {
        return (_Link_type)_Rb_tree_node_base::_S_maximum(__x);
    }

   public:
    typedef _Rb_tree_iterator<_Val> iterator;
    typedef _Rb_tree_const_iterator<_Val> const_iterator;

   private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    void _M_erase(_Link_type __x);

   public:
    _Rb_tree() : _M_node_count(0), _M_key_compare() {
        _M_header = _M_get_node();
        _M_empty_initialize();
    }
    _Rb_tree(const _Compare& __comp)
        : _M_node_count(0), _M_key_compare(__comp) {
        _M_header = _M_get_node();
        _M_empty_initialize();
    }
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _M_node_count(0), _M_key_compare(__comp) {
        _M_header = _M_get_node();
        _M_empty_initialize();
    }
    _Rb_tree(const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x)
        : _M_node_count(0), _M_key_compare(__x._M_key_compare) {
            _M_header = _M_get_node();
        if (__x._M_root() == nullptr)
            _M_empty_initialize();
        else {
            _S_color(_M_header) = red;
            _M_root() = _M_copy(__x._M_root(), _M_header);
            _M_leftmost() = _S_minimum(_M_root());
            _M_rightmost() = _S_maximum(_M_root());
        }
        _M_node_count = __x._M_node_count;
    }
    ~_Rb_tree() { clear(); }
    _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& operator=(
        const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x);

   private:
    void _M_empty_initialize() {
        _S_color(_M_header) = red;
        _M_root() = nullptr;
        _M_rightmost() = _M_leftmost() = _M_header;
    }

   public:
    _Compare key_comp() const { return _M_key_compare; }
    iterator begin() { return _M_leftmost(); }
    iterator end() { return _M_header; }
    const_iterator begin() const { return _M_leftmost(); }
    const_iterator end() const { return _M_header; }

    bool empty() { return _M_node_count == 0; }
    size_type size() const { return _M_node_count; }
    size_type max_size() const { return size_type(-1); }

    void swap(_Self& __t) {
        swap(_M_header, __t._M_header);
        swap(_M_node_count, __t._M_node_count);
        swap(_M_key_compare, __t._M_key_compare);
    }

   public:
    std::pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_equal(const value_type& __x);
    iterator insert_unique(iterator __pos, const value_type& __x);
    iterator insert_equal(iterator __pos, const value_type& __x);
    void insert_unique(const_iterator __first, const_iterator __last);
    void insert_unique(const value_type* __first, const value_type* __last);
    void insert_equal(const_iterator __first, const_iterator __last);
    void insert_equal(const value_type* __first, const value_type* __last);

    void erase(iterator __pos);
    size_type erase(const key_type& __x);
    void erase(iterator __first, iterator __last);
    void erase(const key_type* __first, const key_type* __last);
    void clear() {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = _M_header;
            _M_root() = nullptr;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }

    iterator find(const key_type& __x);
    const_iterator find(const key_type& __x) const;
    size_type count(const key_type& __x) const;
    iterator lower_bound(const key_type& __x);
    const_iterator lower_bound(const key_type& __x) const;
    iterator upper_bound(const key_type& __x);
    const_iterator upper_bound(const key_type& __x) const;
    std::pair<iterator, iterator> equal_range(const key_type& __x);
    std::pair<const_iterator, const_iterator> equal_range(
        const key_type& __x) const;

    bool __verify() const;
};

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator==(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return __x.size() == __y.size() &&
           equal(__x.begin(), __x.end(), __y.begin());
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator<(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                   __y.end());
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator!=(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return !(__x == __y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator>(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return __y < __x;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator<=(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return !(__y < __x);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline bool operator>=(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    return !(__x < __y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline void swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x,
                 _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __y) {
    __x.swap(__y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>&
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::operator=(
    const _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __x) {
    if (this != &__x) {
        clear();
        _M_node_count = 0;
        _M_key_compare = __x._M_key_compare;
        if (__x._M_root() == 0) {
            _M_root() = 0;
            _M_leftmost() = _M_header;
            _M_rightmost() = _M_header;
        } else {
            _M_root() = _M_copy(__x._M_root(), _M_header);
            _M_leftmost() = _S_minimum(_M_root());
            _M_rightmost() = _S_maximum(_M_root());
            _M_node_count = __x._M_node_count;
        }
    }
    return *this;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert(
    _Base_ptr __x_, _Base_ptr __y_, const _Val& __v) {
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header || __x != 0 ||
        _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;
        if (__y == _M_header) {
            _M_root() = __z;
            _M_rightmost() = __z;
        } else if (__y == _M_leftmost())
            _M_leftmost() = __z;
    } else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost()) _M_rightmost() = __z;
    }
    _S_parent(__z) = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_tree_rebalance(__z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_equal(
    const _Val& __v) {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();
    while (__x != 0) {
        __y = __x;
        __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x)
                                                              : _S_right(__x);
    }
    return _M_insert(__x, __y, __v);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
std::pair<
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator,
    bool>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_unique(
    const _Val& __v) {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp) {
        if (__j == begin())
            return std::pair<iterator, bool>(_M_insert(__x, __y, __v), true);
        else
            --__j;
    }
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return std::pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return std::pair<iterator, bool>(__j, false);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_unique(
    iterator __position, const _Val& __v) {
    if (__position._M_node == _M_header->_M_left) {
        if (size() > 0 &&
            _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else
            return insert_unique(__v).first;
    } else if (__position._M_node == _M_header) {
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v);
        else
            return insert_unique(__v).first;
    } else {
        iterator __before = __position;
        --__before;
        if (_M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)) &&
            _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        } else
            return insert_unique(__v).first;
    }
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_equal(
    iterator __position, const _Val& __v) {
    if (__position._M_node == _M_header->_M_left) {
        if (size() > 0 &&
            !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else
            return insert_equal(__v);
    } else if (__position._M_node == _M_header) {
        if (!_M_key_compare(_KeyOfValue()(__v), _S_key(_M_rightmost())))
            return _M_insert(0, _M_rightmost(), __v);
        else
            return insert_equal(__v);
    } else {
        iterator __before = __position;
        --__before;
        if (!_M_key_compare(_KeyOfValue()(__v), _S_key(__before._M_node)) &&
            !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        } else
            return insert_equal(__v);
    }
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_equal(
    const _Val* __first, const _Val* __last) {
    for (; __first != __last; ++__first) insert_equal(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_equal(
    const_iterator __first, const_iterator __last) {
    for (; __first != __last; ++__first) insert_equal(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_unique(
    const _Val* __first, const _Val* __last) {
    for (; __first != __last; ++__first) insert_unique(*__first);
}

template <class _Key, class _Val, class _KoV, class _Cmp, class _Alloc>
void _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_unique(
    const_iterator __first, const_iterator __last) {
    for (; __first != __last; ++__first) insert_unique(*__first);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline void _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    iterator __position) {
    _Link_type __y = (_Link_type)_Rb_tree_rebalance_for_erase(
        __position._M_node, _M_header->_M_parent, _M_header->_M_left,
        _M_header->_M_right);
    destory_node(__y);
    --_M_node_count;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(const _Key& __x) {
    std::pair<iterator, iterator> __p = equal_range(__x);
    size_type __n = 0;
    distance(__p.first, __p.second, __n);
    erase(__p.first, __p.second);
    return __n;
}

template <class _Key, class _Val, class _KoV, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_M_copy(_Link_type __x,
                                                      _Link_type __p) {
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    try {
        if (__x->_M_right) __top->_M_right = _M_copy(_S_right(__x), __top);
        __p = __top;
        __x = _S_left(__x);

        while (__x != 0) {
            _Link_type __y = _M_clone_node(__x);
            __p->_M_left = __y;
            __y->_M_parent = __p;
            if (__x->_M_right) __y->_M_right = _M_copy(_S_right(__x), __y);
            __p = __y;
            __x = _S_left(__x);
        }
    } catch (const char*) {
        _M_erase(__top);
    }

    return __top;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
void _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_erase(
    _Link_type __x) {
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        destory_node(__x);
        __x = __y;
    }
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
void _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    iterator __first, iterator __last) {
    if (__first == begin() && __last == end())
        clear();
    else
        while (__first != __last) erase(__first++);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
void _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    const _Key* __first, const _Key* __last) {
    while (__first != __last) erase(*__first++);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::find(const _Key& __k) {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);

    iterator __j = iterator(__y);
    return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                      : __j;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::find(
    const _Key& __k) const {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0) {
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    }
    const_iterator __j = const_iterator(__y);
    return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                      : __j;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::count(
    const _Key& __k) const {
    std::pair<const_iterator, const_iterator> __p = equal_range(__k);
    size_type __n = 0;
    __n = distance(__p.first, __p.second);
    return __n;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::lower_bound(
    const _Key& __k) {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);

    return iterator(__y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::lower_bound(
    const _Key& __k) const {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);

    return const_iterator(__y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::upper_bound(
    const _Key& __k) {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);

    return iterator(__y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::upper_bound(
    const _Key& __k) const {
    _Link_type __y = _M_header;
    _Link_type __x = _M_root();

    while (__x != 0)
        if (_M_key_compare(__k, _S_key(__x)))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);

    return const_iterator(__y);
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
inline std::pair<
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator,
    typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::equal_range(
    const _Key& __k) {
    return std::pair<iterator, iterator>(lower_bound(__k), upper_bound(__k));
}

template <class _Key, class _Val, class _KoV, class _Compare, class _Alloc>
inline std::pair<
    typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::const_iterator,
    typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::const_iterator>
_Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::equal_range(
    const _Key& __k) const {
    return std::pair<const_iterator, const_iterator>(lower_bound(__k),
                                                     upper_bound(__k));
}

inline int __black_count(_Rb_tree_node_base* __node,
                         _Rb_tree_node_base* __root) {
    if (__node == 0)
        return 0;
    else {
        int __bc = __node->_M_color == black ? 1 : 0;
        if (__node == __root)
            return __bc;
        else
            return __bc + __black_count(__node->_M_parent, __root);
    }
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc>
bool _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::__verify() const {
    if (_M_node_count == 0 || begin() == end())
        return _M_node_count == 0 && begin() == end() &&
               _M_header->_M_left == _M_header &&
               _M_header->_M_right == _M_header;

    int __len = __black_count(_M_leftmost(), _M_root());
    for (const_iterator __it = begin(); __it != end(); ++__it) {
        _Link_type __x = (_Link_type)__it._M_node;
        _Link_type __L = _S_left(__x);
        _Link_type __R = _S_right(__x);

        if (__x->_M_color == red)
            if ((__L && __L->_M_color == red) || (__R && __R->_M_color == red))
                return false;

        if (__L && _M_key_compare(_S_key(__x), _S_key(__L))) return false;
        if (__R && _M_key_compare(_S_key(__R), _S_key(__x))) return false;

        if (!__L && !__R && __black_count(__x, _M_root()) != __len)
            return false;
    }

    if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root()))
        return false;
    if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root()))
        return false;

    return true;
}

template <class _Key, class _Val, class _KeyOfValue, class _Compare,
          class _Alloc = allocator<_Val>>
struct rb_tree : public _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> {
    typedef _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> _Base;
    typedef typename _Base::allocator_type allocator_type;

    rb_tree(const _Compare& __comp = _Compare(),
            const allocator_type& __a = allocator_type())
        : _Base(__comp, __a) {}

    ~rb_tree() {}
};

}  // namespace custl

#endif