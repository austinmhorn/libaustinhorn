#ifndef stack_hpp
#define stack_hpp

#include <memory>
#include <type_traits>
#include <iostream>

template <typename _Tp> struct __stack_node;
template <typename _Tp> struct __stack_node_base;

///////////////////////////////////////////////////
/// @struct __stack_node_pointer_traits
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
struct __stack_node_pointer_traits 
{
    typedef __stack_node_base<_Tp> * __base_pointer;
    typedef __stack_node<_Tp>      * __node_pointer;
};

///////////////////////////////////////////////////
/// @struct __stack_node_base
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
struct __stack_node_base
{
    typedef __stack_node_pointer_traits<_Tp>        __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;
    typedef typename __node_traits::__base_pointer  __base_pointer;

    __node_pointer __prev_;
    __node_pointer __next_;
};




///////////////////////////////////////////////////
/// @struct __stack_node
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
struct __stack_node 
    : public __stack_node_base<_Tp>
{
    typedef __stack_node_base<_Tp>                __node_base;
    typedef typename __node_base::__node_pointer  __node_pointer;

    _Tp __value_;

    __stack_node(const _Tp& __v, __node_pointer __p = nullptr, __node_pointer __n = nullptr) 
        { __value_ = __v; __node_base::__prev_ = __p; __node_base::__next_ = __n; }
};




template <typename _Tp> class stack;
template <typename _Tp> class __stack_imp;
template <typename _Tp> class __stack_const_iterator;

///////////////////////////////////////////////////
/// @class __stack_iterator
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
class __stack_iterator
{
    typedef __stack_node_pointer_traits<_Tp>         __node_traits;
    typedef typename __node_traits::__node_pointer   __node_pointer;

    __node_pointer __pointer_;

    explicit __stack_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class stack;
    template<typename> friend class __stack_imp;
    template<typename> friend class __stack_const_iterator;
public:
    typedef std::bidirectional_iterator_tag       iterator_category;
    typedef _Tp                                   value_type;
    typedef value_type&                           reference;
    typedef value_type*                           pointer;
    typedef std::ptrdiff_t                        difference_type;

    reference operator*() const { return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __stack_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }
    __stack_iterator operator++(int) { __stack_iterator __t(*this); ++(*this); return __t; }
    __stack_iterator& operator--() { __pointer_ = __pointer_->__prev_; return *this; }
    __stack_iterator operator--(int) { __stack_iterator __t(*this); --(*this); return __t; }

    friend bool operator==(const __stack_iterator& __x, const __stack_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __stack_iterator& __x, const __stack_iterator& __y) { return !(__x == __y); }

};

///////////////////////////////////////////////////
/// @class __stack_const_iterator
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
class __stack_const_iterator
{
    typedef __stack_node_pointer_traits<_Tp>        __node_traits;
    typedef typename __node_traits::__node_pointer  __node_pointer;

    __node_pointer __pointer_;

    explicit __stack_const_iterator(__node_pointer __p) noexcept : __pointer_(__p) {}

    template<typename> friend class stack;
    template<typename> friend class __stack_imp;
    template<typename> friend class __stack_imp;
public:
    typedef std::bidirectional_iterator_tag  iterator_category;
    typedef _Tp                              value_type;
    typedef const value_type&                reference;
    typedef value_type*                      pointer;
    typedef std::ptrdiff_t                   difference_type;

    __stack_const_iterator() noexcept : __pointer_(nullptr) { }
    __stack_const_iterator(const __stack_iterator<_Tp>& __p) noexcept : __pointer_(__p.__pointer_) {}

    reference operator*() const {  return __pointer_->__value_; }
    pointer operator->() const { return &__pointer_->__value_; }

    __stack_const_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }
    __stack_const_iterator operator++(int) { __stack_const_iterator __t(*this); ++(*this); return __t; }
    __stack_const_iterator& operator--() {  __pointer_ = __pointer_->__prev_; return *this; }
    __stack_const_iterator operator--(int) { __stack_const_iterator __t(*this); --(*this); return __t; }

    friend bool operator==(const __stack_const_iterator& __x, const __stack_const_iterator& __y) { return __x.__pointer_ == __y.__pointer_; }
    friend bool operator!=(const __stack_const_iterator& __x, const __stack_const_iterator& __y) { return !(__x == __y); }
};






///////////////////////////////////////////////////
/// @class __stack_imp
/// @brief
/// @tparam _Tp
///////////////////////////////////////////////////
template <typename _Tp>
class __stack_imp
{
    private:
        typedef __stack_node<_Tp>  __node;
        typedef __node *           __node_pointer;
        typedef __node_pointer *   __env_pointer;
    protected:
        typedef _Tp                                 value_type;
        typedef value_type *                        pointer;
        typedef value_type &                        reference;
        typedef const value_type                    const_reference;
        typedef __stack_iterator<value_type>        iterator;
        typedef __stack_const_iterator<value_type>  const_iterator;

        __node_pointer __head_;
        __node_pointer __tail_;
        __env_pointer  __dptr_;
        unsigned long  __size_;

        __stack_imp() 
            : __head_(nullptr), __tail_(nullptr), __dptr_(nullptr), __size_(0) {}
        ~__stack_imp() 
            { __destroy_stack(); }

        __stack_imp(const __stack_imp&) = default;
        __stack_imp& operator=(const __stack_imp&) = default;
        __stack_imp(__stack_imp&&) noexcept = default;
        __stack_imp& operator=(__stack_imp&&) noexcept = default;

        void push(const value_type& __v)
        {
            if ( empty() )
            {
                __dptr_ = &__head_;
                __node_pointer __p = *__dptr_;
                __p = new __node(__v, nullptr, nullptr);
                __head_ = __p;
                __tail_ = __p;
            }
            else 
            {
                if ( __size_ == 1 )
                    __head_->__next_ = __tail_;
                __dptr_ = &__tail_;
                __node_pointer __p = *__dptr_;
                __p = new __node(__v, __tail_, nullptr);
                __tail_->__next_ = __p;
                __tail_ = __p;
            }
            ++__size_;
        }

        void push(value_type&& __v)
        {
            if ( empty() )
            {
                __dptr_ = &__head_;
                __node_pointer __p = *__dptr_;
                __p = new __node(std::move(__v), nullptr, nullptr);
                __head_ = __p;
                __tail_ = __p;
            }
            else 
            {
                if ( __size_ == 1 )
                    __head_->__next_ = __tail_;
                __dptr_ = &__tail_;
                __node_pointer __p = *__dptr_;
                __p = new __node(std::move(__v), __tail_, nullptr);
                __tail_->__next_ = __p;
                __tail_ = __p;
            }
            ++__size_;
        }

        void pop()
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

        bool empty() noexcept { return (__size_ == 0); }

        reference top() noexcept { assert((void("stack::top called on empty list"), !empty())); return __tail_->__value_; }
        const_reference top() const noexcept { assert((void("stack::top called on empty list"), !empty())); return __tail_->__value_; }
        //reference front()
        //    noexcept { assert((void("stack::front called on empty list"), !empty())); return __head_->__value_; }
        //reference back()
        //    noexcept { assert((void("stack::back called on empty list"), !empty())); return __tail_->__value_; }

        iterator begin() noexcept  { return iterator(__head_); }
        iterator end() noexcept { return iterator(__tail_->__next_); }
    
        const_iterator begin() const noexcept { return const_iterator(__head_);  }
        const_iterator end() const noexcept { return const_iterator(__tail_->__next_); }


        void print()
        {
            __recursive_print(__head_);
        }

        void __recursive_print(__node_pointer __p)
        {
            if ( __p )
            {
                std::cout << __p->__value_ << std::endl;
                __recursive_print(__p->__next_);
            }
        }   

        void __destroy_stack() noexcept
        {
            __node_pointer __p = __head_;
            while ( __p ) 
            {
                __head_ = __p->__next_;
                delete __p;
                __p = __head_;
            }
            __tail_ = nullptr;
            __dptr_ = nullptr;
            __size_ = 0;
        }
};







template <typename _Tp>
class stack
    : private __stack_imp<_Tp>
{
        typedef __stack_imp<_Tp> base;
    public:        
        typedef typename base::value_type      value_type;
        typedef typename base::pointer         pointer;
        typedef typename base::reference       reference;
        typedef typename base::const_reference const_reference;
        typedef typename base::iterator        iterator;
        typedef typename base::const_iterator  const_iterator;

        stack() 
        {

        }

        ~stack()
        {
            base::__destroy_stack();
        }

        void clear() noexcept
        {
            base::__destroy_stack();
        }

        void push(const value_type& __v)
        {
            base::push(__v);
        }

        void pop()
        {
            base::pop();
        }

        reference top() noexcept
        {
            return base::top();
        }

        const_reference top() const noexcept 
        {
            return base::top();
        }

        reference front() noexcept
        {
            return base::front();   
        }

        reference back() noexcept
        {
            return base::back();
        }


        iterator begin() noexcept
        {
            return base::begin();
        }

        iterator end() noexcept
        {
            return base::end();
        }

        const_iterator begin() const noexcept
        {
            return base::cbegin();
        }

        const_iterator end() const noexcept
        {
            return base::cend();
        }

        void print()
        {
            base::print();
        }

    protected:


};



#endif /* stack_hpp */