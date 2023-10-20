//
//  list.hpp
//  austinhorn
//
//  Created by Austin Horn on 10/19/23.
//  Copyright © 2023 Austin Horn. All rights reserved.
//

#ifndef list_hpp
#define list_hpp

#include <iostream>

template <typename _Tp> struct __list_node;

////////////////////////////////////////////////////////////
/// \struct __list_node
///
/// \brief
////////////////////////////////////////////////////////////
template <typename _Tp> 
struct __list_node {
    __list_node(_Tp __value, __list_node<_Tp>* __next = nullptr) 
        : __value_{__value}
        , __next_{__next} {}

    typedef __list_node<_Tp>* node_pointer;
    
    _Tp          __value_; ///< Contains object data
    node_pointer __next_;  ///< Points to next Node in list
};

////////////////////////////////////////////////////////////
/// \template
/// \class list
///
/// \ingroup LinkedLists
///
/// \brief Uni-directional linked list
////////////////////////////////////////////////////////////
template <typename _Tp>
class list {
    protected:

        typedef typename ::__list_node<_Tp>::node_pointer node_pointer;
        typedef _Tp value_type;

    public:
    
        ////////////////////////////////////////////////////////////
        /// \brief Default Constructor
        ////////////////////////////////////////////////////////////
        list() 
            : m_root{nullptr}
            , m_envPtr(nullptr)
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
                __list_node<_Tp> *p = *this->m_envPtr;
                *this->m_envPtr = new __list_node<_Tp>(data, p);
            }
            
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
            __list_node<_Tp> *p = find(data);
            
            if( p )
            {
                __list_node<_Tp> *q = *this->m_envPtr;
                *this->m_envPtr = (*this->m_envPtr)->__next_;
                q->__next_ = nullptr;
                delete q;
            }
            
            this->m_size--;
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
            this->m_envPtr = &this->m_root;
            
            while( *this->m_envPtr  &&  (*this->m_envPtr)->__value_ != data )
                this->m_envPtr = &(*this->m_envPtr)->__next_;
            
            return *this->m_envPtr;
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
            this->m_envPtr = nullptr;
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
                __list_node<_Tp> *q = p->__next_;
                
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
            this->m_envPtr = &this->m_root;
            
            while( *this->m_envPtr )
                this->m_envPtr = &(*this->m_envPtr)->__next_;

            return !(*this->m_envPtr);
        }
    
    private:
    
        node_pointer  m_root;
        node_pointer* m_envPtr;
        std::size_t   m_size;
        
};



#endif /* list_hpp */
