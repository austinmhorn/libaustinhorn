//
//  iterator.hpp
//  austinhorn
//
//  Created by Austin Horn on 10/19/23.
//  Copyright © 2023 Austin Horn. All rights reserved.
//

#ifndef iterator_hpp
#define iterator_hpp

#include <regex>

template <
    typename _Category,
    typename _Tp,
    typename _Distance  = std::ptrdiff_t,
    typename _Pointer   = _Tp*,
    typename _Reference = _Tp&
> struct __iterator_base {
    typedef _Category                       iterator_category;
    typedef _Tp                             value_type;
    typedef _Distance                       difference_type;
    typedef _Pointer                        pointer;
    typedef _Reference                      reference;
    typedef const pointer                   const_pointer;
    typedef const reference                 const_reference;
    typedef std::__wrap_iter<pointer>       iterator;
    typedef const std::__wrap_iter<pointer> const_iterator;
};

template <typename _Tp> struct __input_iterator_base : public __iterator_base<std::input_iterator_tag, _Tp>{};
template <typename _Tp> struct __output_iterator_base : public __iterator_base<std::output_iterator_tag, _Tp>{};
template <typename _Tp> struct __forward_iterator_base : public __iterator_base<std::forward_iterator_tag, _Tp>{};
template <typename _Tp> struct __bidirectional_iterator_base : public __iterator_base<std::bidirectional_iterator_tag, _Tp>{};
template <typename _Tp> struct __random_access_iterator_base : public __iterator_base<std::random_access_iterator_tag, _Tp>{};

template <typename _Tp>
class forward_iterator : public __forward_iterator_base<_Tp> { 

        typedef __forward_iterator_base<_Tp> base;
        typedef typename base::pointer       pointer;
        typedef typename base::reference     reference;

        forward_iterator(pointer __p) : __pointer_(__p) {}
        ~forward_iterator() {__pointer_ = nullptr;}

        reference operator*() const { return *__pointer_; }
        pointer operator->() { return __pointer_; }

        forward_iterator& operator++() { __pointer_++; return *this; }  
        forward_iterator operator++(int) { forward_iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator == (const forward_iterator& lhs, const forward_iterator& rhs) { return lhs.__pointer_ == rhs.__pointer_; };
        friend bool operator != (const forward_iterator& lhs, const forward_iterator& rhs) { return lhs.__pointer_ != rhs.__pointer_; };     
    private:
        pointer __pointer_;
};

template <long FROM, long TO>
class range_iterator: public __forward_iterator_base<long> {
    protected:
        long __value = FROM;
    public:
        explicit range_iterator(long first = 0) : __value(first) {}

        reference operator*() {return __value;}
        const_reference operator*() const {return __value;}
        
        range_iterator& operator++() {__value = TO >= FROM ? __value + 1 : __value - 1; return *this;}
        range_iterator operator++(int) {range_iterator retval = *this; ++(*this); return retval;}

        bool operator==(range_iterator other) const {return __value == other.__value;}
        bool operator!=(range_iterator other) const {return !(*this == other);}
};

#endif /* iterator_hpp */