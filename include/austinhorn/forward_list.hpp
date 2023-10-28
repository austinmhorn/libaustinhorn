//
//  forward_list.hpp
//  austinhorn
//
//  Created by Austin Horn on 10/19/23.
//  Copyright © 2023 Austin Horn. All rights reserved.
//

#ifndef forward_list_hpp
#define forward_list_hpp

#include <iostream>
#include <cassert>
#include <austinhorn/iterator.hpp>

//////////////////////////////////////////////////////////
/// @brief Template specializations
/// @struct __list_node
/// @struct __list_node_base
/// @class __forward_list_imp
/// @class forward_list
/// @class __list_imp
/// @class list
/// @class __const_list_iterator
/// @class __reverse_list_iterator
/// @class __const_reverse_list_iterator
//////////////////////////////////////////////////////////
template <typename _Tp> struct __list_node;
template <typename _Tp> struct __list_node_base;
template <typename _Tp> class __forward_list_imp;
template <typename _Tp> class forward_list;
template <typename _Tp> class __list_imp;
template <typename _Tp> class list;
template <typename _Tp> class __const_list_iterator;
template <typename _Tp> class __reverse_list_iterator;
template <typename _Tp> class __const_reverse_list_iterator;

//////////////////////////////////////////////////////////
/// @struct __list_node_pointer_traits
/// @brief Pointer traits for list node
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
struct __list_node_pointer_traits
{
    typedef __list_node_base<_Tp> * __base_pointer;
    typedef __list_node<_Tp>      * __node_pointer;
};

//////////////////////////////////////////////////////////
/// @struct __list_node_base
/// @brief Base of list node
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
struct __list_node_base
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;
    typedef typename __node_traits::__base_pointer  __base_pointer;

    __node_pointer __prev_;
    __node_pointer __next_;
};

//////////////////////////////////////////////////////////
/// @struct __list_node
/// @brief List node
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp> 
struct __list_node 
    : public __list_node_base<_Tp> 
{
    typedef __list_node_base<_Tp>            __base;
    typedef typename __base::__node_pointer  __node_pointer;

    _Tp __value_;

    __list_node(const _Tp& __v, __node_pointer __p = nullptr, __node_pointer __n = nullptr) 
        { __value_ = __v; __base::__prev_ = __p; __base::__next_ = __n; }

    friend bool operator<(const __list_node<_Tp>& lhs, const __list_node<_Tp>& rhs) { return lhs.__value_ < rhs.__value_; }
}; 

//////////////////////////////////////////////////////////
/// @class __list_iterator
/// @brief Forward list node iterator
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __list_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer   __node_pointer;

    __node_pointer __pointer_;

    explicit __list_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class __forward_list_imp;
    template<typename> friend class forward_list;
    template<typename> friend class __list_imp;
    template<typename> friend class list;
    template<typename> friend class __const_list_iterator;
public:
    typedef __input_iterator_base<_Tp>  iterator_category;
    typedef _Tp                         value_type;
    typedef value_type&                 reference;
    typedef value_type*                 pointer;
    typedef std::ptrdiff_t              difference_type;

    __list_iterator() noexcept : __pointer_(nullptr) { }
    __list_iterator(const __const_list_iterator<_Tp>& __p) noexcept : __pointer_(__p.__pointer_) {}

    __node_pointer& __as_pointer() { return __pointer_; }

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __list_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }
    __list_iterator operator++(int) { __list_iterator __t(*this); ++(*this); return __t; }
    __list_iterator& operator--() { __pointer_ = __pointer_->__prev_; return *this; }
    __list_iterator operator--(int) { __list_iterator __t(*this); --(*this); return __t; }

    friend bool operator==(const __list_iterator& __x, const __list_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __list_iterator& __x, const __list_iterator& __y) { return !(__x == __y); }

};

//////////////////////////////////////////////////////////
/// @class __const_list_iterator
/// @brief Constant forward list node iterator
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __const_list_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;

    __node_pointer __pointer_;

    explicit __const_list_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}


    template<typename> friend class __forward_list_imp;
    template<typename> friend class forward_list;
    template<typename> friend class __list_imp;
    template<typename> friend class list;
    template<typename> friend class __list_iterator;
public:
    typedef __input_iterator_base<_Tp>   iterator_category;
    typedef _Tp                          value_type;
    typedef const value_type&            reference;
    typedef value_type*                  pointer;
    typedef std::ptrdiff_t               difference_type;

    __const_list_iterator() noexcept : __pointer_(nullptr) { }
    __const_list_iterator(const __list_iterator<_Tp>& __p) noexcept : __pointer_(__p.__pointer_) {}

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __const_list_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }
    __const_list_iterator operator++(int) { __const_list_iterator __t(*this); ++(*this); return __t; }
    __const_list_iterator& operator--() {  __pointer_ = __pointer_->__prev_; return *this; }
    __const_list_iterator operator--(int) { __const_list_iterator __t(*this); --(*this); return __t; }

    __node_pointer& __as_pointer() { return __pointer_; }

    friend bool operator==(const __const_list_iterator& __x, const __const_list_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __const_list_iterator& __x, const __const_list_iterator& __y) { return !(__x == __y); }
};


//////////////////////////////////////////////////////////
/// @class __reverse_list_iterator
/// @brief Reverse list node iterator
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __reverse_list_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer   __node_pointer;

    __node_pointer __pointer_;

    explicit __reverse_list_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class __forward_list_imp;
    template<typename> friend class forward_list;
    template<typename> friend class __list_imp;
    template<typename> friend class list;
    template<typename> friend class __const_reverse_list_iterator;
public:
    typedef __input_iterator_base<_Tp>  iterator_category;
    typedef _Tp                         value_type;
    typedef value_type&                 reference;
    typedef value_type*                 pointer;
    typedef std::ptrdiff_t              difference_type;

    __reverse_list_iterator() noexcept : __pointer_(nullptr) { }

    __node_pointer& __as_pointer() { return __pointer_; }

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __reverse_list_iterator& operator++() { __pointer_ = __pointer_->__prev_; return *this; }
    __reverse_list_iterator operator++(int) { __reverse_list_iterator __t(*this); ++(*this); return __t; }
    __reverse_list_iterator& operator--() { __pointer_ = __pointer_->__next_; return *this; }
    __reverse_list_iterator operator--(int) { __reverse_list_iterator __t(*this); --(*this); return __t; }

    friend bool operator==(const __reverse_list_iterator& __x, const __reverse_list_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __reverse_list_iterator& __x, const __reverse_list_iterator& __y) { return !(__x == __y); }

};

//////////////////////////////////////////////////////////
/// @class __const_reverse_list_iterator
/// @brief Constant reverse list node iterator
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __const_reverse_list_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer   __node_pointer;

    __node_pointer __pointer_;

    explicit __const_reverse_list_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class __forward_list_imp;
    template<typename> friend class forward_list;
    template<typename> friend class __list_imp;
    template<typename> friend class list;
public:
    typedef __input_iterator_base<_Tp>  iterator_category;
    typedef _Tp                         value_type;
    typedef value_type&                 reference;
    typedef value_type*                 pointer;
    typedef std::ptrdiff_t              difference_type;

    __const_reverse_list_iterator() noexcept : __pointer_(nullptr) { }

    __node_pointer& __as_pointer() { return __pointer_; }

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __const_reverse_list_iterator& operator++() { __pointer_ = __pointer_->__prev_; return *this; }
    __const_reverse_list_iterator operator++(int) { __const_reverse_list_iterator __t(*this); ++(*this); return __t; }
    __const_reverse_list_iterator& operator--() { __pointer_ = __pointer_->__next_; return *this; }
    __const_reverse_list_iterator operator--(int) { __const_reverse_list_iterator __t(*this); --(*this); return __t; }

    friend bool operator==(const __const_reverse_list_iterator& __x, const __const_reverse_list_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __const_reverse_list_iterator& __x, const __const_reverse_list_iterator& __y) { return !(__x == __y); }

};

//////////////////////////////////////////////////////////
/// @class __forward_list_imp
/// @brief Implementation of forward_list
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __forward_list_imp
{
    private:
        typedef __list_node<_Tp>  __node;
        typedef __node *          __node_pointer;
        typedef __node **         __env_pointer;
    protected:
        typedef _Tp                                 value_type;
        typedef value_type *                        pointer;
        typedef value_type &                        reference;
        typedef const value_type                    const_reference;
        typedef __list_iterator<value_type>         iterator;
        typedef __const_list_iterator<value_type>   const_iterator;

        __node_pointer __head_;
        __node_pointer __tail_;
        __env_pointer  __dptr_;
        std::size_t    __size_;

        __forward_list_imp() : __head_(nullptr), __tail_(nullptr), __dptr_(nullptr), __size_(0) {}
        virtual ~__forward_list_imp() 
        {
            clear();
        }

        __forward_list_imp(const __forward_list_imp&) = default;
        __forward_list_imp& operator=(const __forward_list_imp&) = default;

        __forward_list_imp(__forward_list_imp&&) noexcept = default;
        __forward_list_imp& operator=(__forward_list_imp&&) noexcept = default;

        void push_front(const value_type& __v)
        {
            __dptr_ = &__head_;
            __node_pointer p = new __node(__v, nullptr, {(empty()) ? nullptr : *__dptr_} );
            __head_ = p;

            if ( empty() )
                __tail_ = p;

            ++__size_;
        }
        void push_front(value_type&& __v)
        {
            __dptr_ = &__head_;
            __node_pointer p = new __node(std::move(__v), nullptr, {(empty()) ? nullptr : *__dptr_} );
            __head_ = p;

            if ( empty() )
                __tail_ = p;

            ++__size_;
        }
        void pop_front()
        {
            if ( !empty() )
            {
                __node_pointer p = __head_;
                __head_ = __head_->__next_;
                p->__next_ = nullptr;
                delete p;
                --__size_;
            }
        }
        bool empty() const noexcept 
        { 
            return (__size_ == 0);
        }
        std::size_t size() const noexcept 
        { 
            return __size_;
        }
        reference front() noexcept 
        { 
            assert((void("forward_list::front called on empty list"), !empty())); 
            return __head_->__value_; 
        }
        const_reference front() const noexcept 
        { 
            assert((void("forward_list::front called on empty list"), !empty()));
            return __head_->__value_; 
        }
        iterator begin() noexcept 
        { 
            return iterator(__head_); 
        }
        iterator end() noexcept 
        { 
            return iterator(__tail_->__next_); 
        }
        const_iterator begin() const noexcept 
        { 
            return const_iterator(__head_);  
        }
        const_iterator end() const noexcept 
        { 
            return const_iterator(__tail_->__next_); 
        }
        const_iterator erase(const_iterator pos)
        {
            __destroy_node(*pos);
            return pos;
        }
        const_iterator erase(const_iterator first, const_iterator last)
        {
            auto iter = first;
            auto fin = ++last;
            
            while ( *iter != *fin )
            {
                auto next = const_iterator( iter.__as_pointer()->__next_ );
                __destroy_node(*iter);
                iter = next;
            }

            return first;
        }
        iterator find(const value_type& __v)
        {
            iterator iter = begin();

            for (; iter != end(); ++iter)
                if ( *iter == __v )
                    break;

            return iter;
        }
        bool exists(const value_type& __v)
        {
            __dptr_ = &__head_;
            while( *__dptr_  &&  (*__dptr_)->__value_ != __v)
                __dptr_ = &(*__dptr_)->__next_;
            return *__dptr_  &&  (*__dptr_)->__value_ == __v;
        }
        void clear() noexcept
        {
            __recursive_destroy(__head_);
            __head_ = nullptr;
            __tail_ = nullptr;
            __dptr_ = nullptr;
            __size_ = 0;
        }
        void print()
        {
            __recursive_print(__head_);
            std::cout << std::endl;
        }


        void __destroy_node(const value_type& __v)
        {
            if ( exists(__v) )
            {
                __node_pointer q = *__dptr_;
                *__dptr_ = (*__dptr_)->__next_;
                delete q;
                --__size_;
            }
        }

        void __recursive_destroy(__node_pointer __p) noexcept
        {
            if ( __p )
            {
                __node_pointer q = __p->__next_; 
                __p->__next_ = nullptr;
                delete __p; 
                __recursive_destroy(q); 
            }
        }

        void __recursive_print(__node_pointer __p) noexcept
        {
            if ( __p )
            {
                std::cout << __p->__value_ << " ";
                __recursive_print(__p->__next_);
            }
        }

};

////////////////////////////////////////////////////////////////
/// @class forward_list
/// @brief Singly linked list
/// @tparam _Tp Value type list is storing
////////////////////////////////////////////////////////////////
template <typename _Tp>
class forward_list 
    : private __forward_list_imp<_Tp>
{
    private:
        typedef __forward_list_imp<_Tp> base;
    public:        
        typedef typename base::value_type      value_type;
        typedef typename base::pointer         pointer;
        typedef typename base::reference       reference;
        typedef typename base::const_reference const_reference;
        typedef typename base::iterator        iterator;
        typedef typename base::const_iterator  const_iterator;
    
        forward_list() : base::__forward_list_imp() {}
        virtual ~forward_list() = default;
        forward_list(const forward_list&) = default;
        forward_list& operator=(const forward_list&) = default;
        forward_list(forward_list&&) noexcept = default;
        forward_list& operator=(forward_list&&) noexcept = default;

        void push_front(const value_type& __v) { base::push_front(__v);}
        void push_front(value_type&& __v) { base::push_front(__v);}
        void pop_front() { base::pop_front();}
        void clear() { base::clear();}
        void print() { base::print();}

        std::size_t size() const noexcept {  return base::size(); }
        bool empty() const noexcept {  return base::empty(); }
        bool exists(const value_type& __v) { return base::exists(__v); }

        reference front() noexcept {  return base::front(); }
        const_reference front() const noexcept  {  return base::front(); }

        iterator begin() noexcept {  return base::begin(); }
        iterator end() noexcept {  return base::end(); }
        const_iterator begin() const noexcept {  return base::begin();  }
        const_iterator end() const noexcept {  return base::end(); }

        const_iterator erase(const_iterator pos) { return base::erase(pos); }
        const_iterator erase(const_iterator first, const_iterator last) { return base::erase(first, last); }
        iterator find(const value_type& __v) { return base::find(__v); }

};

#endif /* forward_list_hpp */