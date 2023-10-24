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

template <typename _Tp> struct __list_node;
template <typename _Tp> struct __list_node_base;

template <typename _Tp>
struct __list_node_pointer_traits
{
    typedef __list_node_base<_Tp> * __base_pointer;
    typedef __list_node<_Tp>      * __node_pointer;
};





template <typename _Tp>
struct __list_node_base
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;
    typedef typename __node_traits::__base_pointer  __base_pointer;

    __node_pointer __next_;
};




template <typename _Tp> 
struct __list_node 
    : public __list_node_base<_Tp> 
{
    typedef __list_node_base<_Tp>           __base;
    typedef typename __base::__node_pointer __node_pointer;

    _Tp __value_;

    __list_node(const _Tp& __v, __node_pointer __n = nullptr) 
        { __value_ = __v; __base::__next_ = __n; }
}; 


template <typename _Tp> class list;
template <typename _Tp> class __list_imp;
template <typename _Tp> class __list_const_iterator;






template <typename _Tp>
class __list_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer   __node_pointer;

    __node_pointer __pointer_;

    explicit __list_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class list;
    template<typename> friend class __list_imp;
    template<typename> friend class __list_const_iterator;
public:
    typedef __input_iterator_base<_Tp>  iterator_category;
    typedef _Tp                         value_type;
    typedef value_type&                 reference;
    typedef value_type*                 pointer;
    typedef std::ptrdiff_t              difference_type;

    __list_iterator() noexcept : __pointer_(nullptr) { }
    __list_iterator(const __list_const_iterator<_Tp>& __p) noexcept : __pointer_(__p.__pointer_) {}

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

///////////////////////////////////////////////////
/// @class __list_const_iterator
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
class __list_const_iterator
{
    typedef __list_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;

    __node_pointer __pointer_;

    explicit __list_const_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class list;
    template<typename> friend class __list_imp;
    template<typename> friend class __list_iterator;
public:
    typedef __input_iterator_base<_Tp>   iterator_category;
    typedef _Tp                          value_type;
    typedef const value_type&            reference;
    typedef value_type*                  pointer;
    typedef std::ptrdiff_t               difference_type;

    __list_const_iterator() noexcept : __pointer_(nullptr) { }
    __list_const_iterator(const __list_iterator<_Tp>& __p) noexcept : __pointer_(__p.__pointer_) {}

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __list_const_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }
    __list_const_iterator operator++(int) { __list_const_iterator __t(*this); ++(*this); return __t; }
    __list_const_iterator& operator--() {  __pointer_ = __pointer_->__prev_; return *this; }
    __list_const_iterator operator--(int) { __list_const_iterator __t(*this); --(*this); return __t; }

    __node_pointer& __as_pointer() { return __pointer_; }

    friend bool operator==(const __list_const_iterator& __x, const __list_const_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __list_const_iterator& __x, const __list_const_iterator& __y) { return !(__x == __y); }
};






template <typename _Tp>
class __list_imp
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
        typedef __list_const_iterator<value_type>   const_iterator;

        __node_pointer __head_;
        __node_pointer __tail_;
        __env_pointer  __dptr_;
        std::size_t    __size_;

        __list_imp() : __head_(nullptr), __tail_(nullptr), __dptr_(nullptr), __size_(0) {}
        ~__list_imp() = default;

        __list_imp(const __list_imp&) = default;
        __list_imp& operator=(const __list_imp&) = default;

        __list_imp(__list_imp&&) noexcept = default;
        __list_imp& operator=(__list_imp&&) noexcept = default;

        void push_front(const value_type& __v)
        {
            __dptr_ = &__head_;
            __node_pointer p = new __node(__v, {(empty()) ? nullptr : *__dptr_} );
            __head_ = p;

            if ( empty() )
                __tail_ = p;

            ++__size_;
        }
        void push_front(value_type&& __v)
        {
            __dptr_ = &__head_;
            __node_pointer p = new __node(std::move(__v), {(empty()) ? nullptr : *__dptr_}  );
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
            __tail_ = nullptr;
            __dptr_ = nullptr;
            __size_ = 0;
        }
        void print()
        {
            __recursive_print(__head_);
            std::cout << std::endl;
        }

        template <typename _ForwardIter>
        _ForwardIter unique(_ForwardIter first, _ForwardIter last)
        {
            if (first == last)
                return last;
                
            _ForwardIter result = first;
            while (++first != last)
                if (!(*result == *first) && ++result != first)
                    *result = std::move(*first);
            return ++result;
        }

        template <typename _ForwardIter>
        _ForwardIter remove(_ForwardIter first, _ForwardIter last, const value_type& value)
        {
            first = std::find(first, last, value);
            if (first != last)
                for (_ForwardIter i = first; ++i != last;)
                    if (!(*i == value))
                        *first++ = std::move(*i);
            return first;
        }

        template <typename __ForwardIter, typename _UnaryPred>
        __ForwardIter remove_if(__ForwardIter first, __ForwardIter last, _UnaryPred pred)
        {
            first = std::find_if(first, last, pred);
            if (first != last)
                for (__ForwardIter i = first; ++i != last;)
                    if (!pred(*i))
                        *first++ = std::move(*i);
            return first;
        }

        /// TODO: reverse - reverses the order of the elements 

        template <typename _Compare>
        void sort(_Compare comp)
        {
            
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
/// @brief 
/// @tparam _Tp 
////////////////////////////////////////////////////////////////
template <typename _Tp>
class forward_list 
    : private __list_imp<_Tp>
{
    private:
        typedef __list_imp<_Tp> base;
    public:        
        typedef typename base::value_type      value_type;
        typedef typename base::pointer         pointer;
        typedef typename base::reference       reference;
        typedef typename base::const_reference const_reference;
        typedef typename base::iterator        iterator;
        typedef typename base::const_iterator  const_iterator;
    
        forward_list() : base::__list_imp() {}
        ~forward_list() { base::clear(); }
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

        /*
        iterator erase_after(const_iterator pos) { return base::erase_after(pos); }
        iterator erase_after(const_iterator first, const_iterator last) { return base::erase_after(first, last); }
        
        template <typename _ForwardIter>
        _ForwardIter unique(_ForwardIter first, _ForwardIter last)
            { return base::unique(begin(), end()); }
        template <typename _ForwardIter>
        _ForwardIter remove(_ForwardIter first, _ForwardIter last, const value_type& value)
            { return base::remove(first, last, value); }
        template <typename __ForwardIterer, typename _UnaryPred>
        __ForwardIterer remove_if(__ForwardIterer first, __ForwardIterer last, _UnaryPred pred)
            { return base::remove_if(first, last, pred); }
        template <typename _Compare>
        void sort(_Compare comp)
            { base::sort(comp); }
        */

};

#endif /* forward_list_hpp */