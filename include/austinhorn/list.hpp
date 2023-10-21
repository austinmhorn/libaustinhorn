//
//  list.hpp
//  austinhorn
//
//  Created by Austin Horn on 10/19/23.
//  Copyright © 2023 Austin Horn. All rights reserved.
//

////////////////////////////////////////////////////////////
/// @struct __list_iterator
///
/// @brief List container iterator
///
/// @tparam _Tp Value type
////////////////////////////////////////////////////////////
/*
template <typename _Tp>
class __list_iterator : public __bidirectional_iterator_base<_Tp>
{
    public:
        typedef _Tp                                       value_type;
        typedef __bidirectional_iterator_base<value_type> iterator_base;
        typedef typename iterator_base::iterator_category iterator_category;
        typedef typename iterator_base::difference_type   difference_type;
        typedef typename iterator_base::pointer           pointer;
        typedef typename iterator_base::reference         reference;

        __list_iterator(pointer __p) : __pointer_(__p) {}
        ~__list_iterator() { __pointer_ = nullptr; }

        reference operator*() const { return *__pointer_; }
        pointer operator->() { return __pointer_; }
        reference operator[](difference_type n) const { return *(__pointer_ + n); }
        __list_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }  
        __list_iterator operator++(int) { __list_iterator __t(*this); ++(*this); return __t;}
        __list_iterator& operator--() { __pointer_ = __pointer_->__prev_; return *this; }
        __list_iterator operator--(int) { __list_iterator __t(*this); --__pointer_; return __t; }
        __list_iterator& operator+=(difference_type n) { __pointer_ += n; return *this; }
        __list_iterator& operator-=(difference_type n) { __pointer_ -= n; return *this; }

        friend bool operator==(const __list_iterator& lhs, const __list_iterator& rhs) { return lhs.__pointer_ == rhs.__pointer_; }
        friend bool operator!=(const __list_iterator& lhs, const __list_iterator& rhs) { return !(lhs == rhs); }
        friend bool operator<(const __list_iterator& lhs, const __list_iterator& rhs) { return lhs.__pointer_ < rhs.__pointer_; }
        friend bool operator>(const __list_iterator& lhs, const __list_iterator& rhs) { return rhs < lhs; }
        friend bool operator<=(const __list_iterator& lhs, const __list_iterator& rhs) { return !(rhs < lhs); }
        friend bool operator>=(const __list_iterator& lhs, const __list_iterator& rhs) { return !(lhs < rhs); }
        friend __list_iterator operator+(const __list_iterator& it, difference_type n) { __list_iterator temp = it; temp += n; return temp; }
        friend __list_iterator operator+(difference_type n, const __list_iterator& it) { return it + n; }
        friend __list_iterator operator-(const __list_iterator& it, difference_type n) { __list_iterator temp = it; temp -= n; return temp; }
        friend difference_type operator-(const __list_iterator& lhs, const __list_iterator& rhs) { return lhs.__pointer_ - rhs.__pointer_; }

    private:
        pointer __pointer_;
};
*/

#ifndef list_hpp
#define list_hpp

#include <iostream>
#include <austinhorn/iterator.hpp>

template <typename _Tp> struct __list_node;
template <typename _Tp> struct __list_node_base;
template <typename _Tp> class list;

///////////////////////////////////////////////////////////////
/// @struct __list_node_base
///
/// @brief Represents base class for a list node object 
///
/// @tparam _Tp Value type
///////////////////////////////////////////////////////////////
template <typename _Tp> 
struct __list_node_base 
{
    typedef __list_node<_Tp> __node;
    typedef __node *         __node_pointer;

    __node_pointer __next_{nullptr};  
    __node_pointer __prev_{nullptr}; 

    operator __node_pointer()
        { return *this; }
};

////////////////////////////////////////////////////////////
/// @struct __list_node
///
/// @brief Node object for list containers
///
/// @tparam _Tp Value type
////////////////////////////////////////////////////////////
template <typename _Tp> 
struct __list_node 
    : public __list_node_base<_Tp> 
{
    typedef _Tp value_type;
    typedef __list_node_base<_Tp>           __base;
    typedef typename __base::__node_pointer __node_pointer;

    __list_node(const _Tp& __v, __node_pointer __n = nullptr, __node_pointer __p = nullptr) 
        : __value_{__v} 
        {
            __base::__next_ = __n;
            __base::__prev_ = __p;
        }

    constexpr bool operator()( const __list_node& lhs, const __list_node& rhs ) const { return lhs.__value_ < rhs.__value_; }

    _Tp __value_;
}; 



template <typename _Tp>
class __list_iterator 
{
        typedef _Tp * pointer;
        typedef _Tp & reference;
        typedef std::ptrdiff_t difference_type;

        pointer __pointer_;
        template<class> friend class list;
    public:
        __list_iterator(pointer __p) : __pointer_(__p) {}
        ~__list_iterator() { __pointer_ = nullptr; }

        reference operator*() const { return *__pointer_; }
        pointer operator->() { return __pointer_; }
        reference operator[](difference_type n) const { return *(__pointer_ + n); }
        
        __list_iterator& operator++() { __pointer_ = __pointer_->__next_; return *this; }  
        __list_iterator operator++(int) { __list_iterator __t(*this); ++(*this); return __t;}
        __list_iterator& operator--() { __pointer_ = __pointer_->__prev_; return *this; }
        __list_iterator operator--(int) { __list_iterator __t(*this); --__pointer_; return __t; }
};

////////////////////////////////////////////////////////////
/// \class list
///
/// \ingroup Containers
///
/// \brief Uni-directional linked list
////////////////////////////////////////////////////////////
template <typename _Tp>
class list {
    private:

        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = _Tp;
        using pointer           = value_type*;  
        using reference         = value_type&;
        using list_node         = __list_node<value_type>;
        using node_pointer      = list_node*;
        using iterator = __list_iterator<node_pointer>;

        node_pointer  m_root;
        node_pointer* m_dp;
        std::size_t   m_size;

    public:


        iterator begin()
        {
            return iterator((&m_root));
        }
    
        ////////////////////////////////////////////////////////////
        /// \brief Default Constructor
        ////////////////////////////////////////////////////////////
        list() 
            : m_root{nullptr}
            , m_dp(nullptr)
            , m_size{0}
            { }

        ////////////////////////////////////////////////////////////
        /// \brief Destructor
        ////////////////////////////////////////////////////////////
        ~list()
        {
            altClear(this->m_root);
        }

        ////////////////////////////////////////////////////////////
        /// \function push_back
        ///
        /// \brief Create node and inserts at back of list
        ///
        /// \param data Reference to object being inserted
        ////////////////////////////////////////////////////////////
        void push_back(const value_type& data)
        {
            if ( pushingToBack() )
            {
                node_pointer p = *this->m_dp;
                *this->m_dp = new __list_node<_Tp>(data, p);
            }
            
            this->m_size++;
        }

        ////////////////////////////////////////////////////////////
        /// \function push_front
        ///
        /// \brief Create node and inserts at front of list
        ///
        /// \param data Reference to object being inserted
        ////////////////////////////////////////////////////////////
        void push_front(const value_type& data)
        {
            this->m_dp = &this->m_root;

            node_pointer p = new __list_node<_Tp>(data, *this->m_dp);
            this->m_root = p;

            this->m_size++;
        }

        ////////////////////////////////////////////////////////////
        /// \function remove
        ///
        /// \brief Remove node from list (if it exists)
        ///
        /// \param data Reference to object being removed
        ////////////////////////////////////////////////////////////
        void remove(const value_type& data)
        {
            node_pointer p = find(data);
            
            if( p )
            {
                node_pointer q = *this->m_dp;
                *this->m_dp = (*this->m_dp)->__next_;
                q->__next_ = nullptr;
                delete q;
                this->m_size--;
            }
        }

        ////////////////////////////////////////////////////////////
        /// \function find
        ///
        /// \brief Search list for node
        ///
        /// \param data Reference to node undergoing search
        ///
        /// \returns Pointer to node or nullptr if not found
        ////////////////////////////////////////////////////////////
        node_pointer find(const value_type& data)
        {
            this->m_dp = &this->m_root;
            
            while( *this->m_dp  &&  (*this->m_dp)->__value_ != data )
                this->m_dp = &(*this->m_dp)->__next_;
            
            return *this->m_dp;
        }

        ////////////////////////////////////////////////////////////
        /// \function size
        ///
        /// \brief Tells number of nodes stored in list
        ///
        /// \returns Number of nodes in list
        ////////////////////////////////////////////////////////////
        std::size_t size() const
        {
            return m_size;
        }

        ////////////////////////////////////////////////////////////
        /// \function clear
        ///
        /// \brief Public method for deleting entire list
        ////////////////////////////////////////////////////////////
        void clear()
        {
            altClear(this->m_root);
            this->m_root = nullptr;
            this->m_dp = nullptr;
            this->m_size = 0;
        }

        ////////////////////////////////////////////////////////////
        /// \function print
        ///
        /// \brief Print recursively starting at root
        ///
        /// \see altPrint
        ////////////////////////////////////////////////////////////
        void print()
        {
            altPrint(this->m_root);
        }

    private:

        ////////////////////////////////////////////////////////////
        /// \function altClear
        /// 
        /// \brief Private recursive clearing method
        ///
        /// \param p Pointer to node undergoing deletion
        ////////////////////////////////////////////////////////////
        void altClear(node_pointer p)
        {
            if ( p )
            {
                // Pointer to node's next node (possibly null)
                node_pointer q = p->__next_;
                
                // Break link to next node
                p->__next_ = nullptr;
                
                // Destroy node
                delete p;
                
                // Recursively delete next node in list
                altClear(q);
            }
        }

        ////////////////////////////////////////////////////////////
        /// \function altPrint
        ///
        /// \brief Private recursive printing method
        ///
        /// \param p Pointer to node undergoing print
        ////////////////////////////////////////////////////////////
        void altPrint(node_pointer p)
        {
            if ( p )
            {
                std::cout << p->__value_ << std::endl;
                altPrint(p->__next_);
            }
        }

        ////////////////////////////////////////////////////////////
        /// \function pushingToBack
        ///
        /// \brief Points to next position for node insertion
        ////////////////////////////////////////////////////////////
        bool pushingToBack()
        {
            this->m_dp = &this->m_root;
            
            while( *this->m_dp )
                this->m_dp = &(*this->m_dp)->__next_;

            return !(*this->m_dp);
        }        
};

#endif /* list_hpp */
