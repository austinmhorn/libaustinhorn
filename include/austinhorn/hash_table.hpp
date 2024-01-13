//
//  hash_table.hpp
//  Solitaire
//
//  Created by Austin Horn on 11/30/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef hash_table_hpp
#define hash_table_hpp

#include <austinhorn/config.hpp>

#include <iostream>
#include <sstream>
#include <austinhorn/md5.h>
#include <austinhorn/io.hpp>
#include <cassert>

static const char BULLET_POINT[] = "\u2022";

template <typename _Tp, typename _Alloc> class __hash_table_node_base;
template <typename _Tp, typename _Alloc> class hash_table_node;

template <typename _Tp, typename _Alloc>
struct __hash_table_node_pointer_traits
{
    typedef __hash_table_node_base<_Tp, _Alloc> *  __base_pointer;
    typedef hash_table_node<_Tp, _Alloc> *         __node_pointer;
};

template <typename _Tp, typename _Alloc>
struct __hash_table_node_base
{
    typedef __hash_table_node_pointer_traits<_Tp, _Alloc>  __pointer_traits;
    typedef typename __pointer_traits::__base_pointer      __base_pointer;
    typedef typename __pointer_traits::__node_pointer      __node_pointer;
    
    __node_pointer __next_;
};

template <typename _Tp, typename _Alloc>
struct hash_table_node
    : public __hash_table_node_base<_Tp, _Alloc>
{
    typedef __hash_table_node_base<_Tp, _Alloc>  __base;
    typedef typename __base::__pointer_traits    __pointer_traits;
    typedef typename __base::__base_pointer      __base_pointer;
    typedef typename __base::__node_pointer      __node_pointer;

    _Tp __value_;

    hash_table_node(const _Tp& __v, __node_pointer __restrict__ __n = nullptr) 
        { __value_ = __v; __base::__next_ = __n; }
};









template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class hash_table 
{
        typedef hash_table_node<_Tp, _Alloc>          node_type;
        typedef typename node_type::__pointer_traits  __pointer_traits;
        typedef typename node_type::__base_pointer    __base_pointer;
        typedef typename node_type::__node_pointer    __node_pointer;
        typedef typename node_type::__node_pointer *  __env_pointer;

        static const int BUCKETS = 0x10; // 128 in hex
        
        __env_pointer __roots_;
        __env_pointer __dptr_;
        std::size_t   __size_;
        std::size_t   __buckets_;

        void __show_hash_bucket_message(int bucket);
        void __recursive_create_bucket(__node_pointer __restrict__ __p);
        void __recursive_destroy_bucket(__node_pointer __restrict__ __p);
        void __recursive_print_bucket(__node_pointer __restrict__ __p);
        
    public:

        typedef _Tp         value_type;
        typedef std::size_t size_type;
        
        hash_table() 
            noexcept(std::is_convertible<_Tp, std::string>::value);
        ~hash_table();
        
        void clear() noexcept;
        bool exists(const value_type &__v);
        void put(const value_type &__v);
        void remove(const value_type &__v);
        size_type size() const noexcept;

        // Bucket Interface
        size_type bucket_count() const noexcept
        {
            return __buckets_;
        }
        size_type max_bucket_count() const noexcept
        {
            return BUCKETS;
        }
        

        void print();
        void print_bucket(int bucket);
        
    protected:

        int getHashIndex(unsigned long k);
};

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
hash_table<_Tp, _Alloc>::hash_table() noexcept(std::is_convertible<_Tp, std::string>::value)
{
    assert((std::is_class<_Tp>::value));
    
    this->__roots_ = new node_type * [ BUCKETS ];
    clear();
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
hash_table<_Tp, _Alloc>::~hash_table()
{
    delete [] this->__roots_;
    this->__dptr_ = nullptr;
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::clear() noexcept
{
    for (int i = 0; i < BUCKETS; i++) 
    {
        //std::cout << "creating bucket " << i << std::endl;
        this->__dptr_ = &__roots_[ i ];
        __recursive_create_bucket(*this->__dptr_);
        this->__roots_[ i ] = nullptr;
    }
    this->__dptr_ = nullptr;
    this->__size_ = 0;
    this->__buckets_ = 0;
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
bool hash_table<_Tp, _Alloc>::exists(const value_type &__v)
{
    unsigned long h = __v.getHash();
    int i = getHashIndex(h);
    
    this->__dptr_ = &this->__roots_[ i ];
    while ( *this->__dptr_  &&  (*this->__dptr_)->__value_ != __v )
        this->__dptr_ = &(*this->__dptr_)->__next_;
    return *this->__dptr_ && (*this->__dptr_)->__value_ == __v;
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
int hash_table<_Tp, _Alloc>::getHashIndex(unsigned long k)
{
    return (k % BUCKETS);
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::put(const value_type &__v)
{
    if ( !exists(__v) )
    {
        __node_pointer __n = *this->__dptr_;
        *this->__dptr_ = new node_type(__v, __n);
        ++__size_;
        if ( *__dptr_  ==  __roots_[ getHashIndex(__v.getHash()) ] )
        {
            ++__buckets_;
        }
        //std::cout << "** Inserting value: " << (*this->__dptr_)->__value_ << std::endl;
    }
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::remove(const value_type &__v)
{
    if ( exists(__v) )
    {
        __node_pointer __n = *this->__dptr_;
        *this->__dptr_ = (*this->__dptr_)->__next_;
        delete __n;
        --__size_;
    }
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
std::size_t hash_table<_Tp, _Alloc>::size() const noexcept
{
    return __size_;
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::print()
{
    for (int i = 0; i < BUCKETS; i++)
    { 
        __show_hash_bucket_message(i);
        this->__dptr_ = &__roots_[ i ];
        if ( *this->__dptr_ )
            __recursive_print_bucket(*this->__dptr_);
        std::cout.flush() << std::endl;
    }
}


////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::__show_hash_bucket_message(int bucket)
{
    std::cout << '|' << spacing(10 + std::to_string(bucket).size()) << '|' << std::endl;
    std::cout << "| Bucket #" << bucket << " |" << std::endl;
    std::cout << '|' << repeat('_', 10 + std::to_string(bucket).size()) << "|\n" << std::endl;
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::__recursive_create_bucket(__node_pointer __restrict__ __p)
{
    if ( __p )
    {
        //std::cout << "nullify node" << std::endl;
        __node_pointer __q = __p->__next_;
        __p = nullptr;
        __recursive_create_bucket(__q);
    }
    else
    {
        //std::cout << "root is null" << std::endl;
    }
}

////////////////////////////////////////////////////////////
template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::__recursive_destroy_bucket(__node_pointer __restrict__ __p)
{
    if ( __p )
    {
        //std::cout << "destroy allocated node" << std::endl;
        __node_pointer __q = __p->__next_;
        delete __p;
        __recursive_create_bucket(__q);
    }
    else
    {
        //std::cout << "node is null" << std::endl;
    }
}

template <typename _Tp, typename _Alloc>
void hash_table<_Tp, _Alloc>::__recursive_print_bucket(__node_pointer __restrict__ __p)
{
    if ( __p )
    {
        std::cout.flush() << spacing(2) << BULLET_POINT << " " << __p->__value_ << std::endl;
        __recursive_print_bucket(__p->__next_);
    }
}

#endif /* hash_table_hpp */
