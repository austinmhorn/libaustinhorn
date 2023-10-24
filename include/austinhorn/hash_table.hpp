//
//  hash_table.hpp
//  Solitaire
//
//  Created by Austin Horn on 11/30/22.
//  Copyright © 2022 Austin Horn. All rights reserved.
//

#ifndef hash_table_hpp
#define hash_table_hpp

#include <iostream>



template <typename _Tp>
class hash_table {
private:
    
    static const int HASH_SPACE = 0x10; // 16 in hex
    struct hash_node
    {
        _Tp payload;
        hash_node* next;
        hash_node() : next(nullptr) {}
    };
    hash_node **roots;
    hash_node **envPtr;
    
public:
    
    ////////////////////////////////////////////////////////////
    /// \brief Default Constructor
    ////////////////////////////////////////////////////////////
    hash_table();
    
    ////////////////////////////////////////////////////////////
    /// \function create
    ///
    /// \brief
    ////////////////////////////////////////////////////////////
    void create();
    
    ////////////////////////////////////////////////////////////
    /// \function isThere
    ///
    /// \brief
    ///
    /// \returns
    ////////////////////////////////////////////////////////////
    bool isThere(_Tp &);
    
    ////////////////////////////////////////////////////////////
    /// \function getHashIndex
    ///
    /// \brief
    ///
    /// \returns
    ////////////////////////////////////////////////////////////
    int getHashIndex(unsigned long);
    
    ////////////////////////////////////////////////////////////
    /// \function put
    ///
    /// \brief
    ////////////////////////////////////////////////////////////
    void put(_Tp &);
    
    ////////////////////////////////////////////////////////////
    /// \function remove
    ///
    /// \brief
    ////////////////////////////////////////////////////////////
    void remove(_Tp &);
    
    ////////////////////////////////////////////////////////////
    /// \function print
    ///
    /// \brief
    ////////////////////////////////////////////////////////////
    void print(std::string);
};

////////////////////////////////////////////////////////////
template <typename _Tp>
hash_table<_Tp>::hash_table()
{
    this->roots = nullptr;
    this->envPtr = nullptr;
    create();
}

////////////////////////////////////////////////////////////
template <typename _Tp>
void hash_table<_Tp>::create()
{
    this->roots = new hash_node*[HASH_SPACE];

    for(int i = 0; i < HASH_SPACE; i++)
        this->roots[ i ] = nullptr;

    this->envPtr = nullptr;
    
    
}

////////////////////////////////////////////////////////////
template <typename _Tp>
bool hash_table<_Tp>::isThere(_Tp &unit)
{
    unsigned long h = unit.getHash();
    
    int i = getHashIndex(h);
    
    this->envPtr = &this->roots[ i ];
    while( *this->envPtr  &&  (*this->envPtr)->payload != unit )
        this->envPtr = &(*this->envPtr)->next;
    
    std::cout << "Identifier: " << unit.identifier << "\tIndex: " << i << std::endl;
        
    return *this->envPtr && (*this->envPtr)->payload == unit;
}

////////////////////////////////////////////////////////////
template <typename _Tp>
int hash_table<_Tp>::getHashIndex(unsigned long k)
{
    return (k % HASH_SPACE);
}

////////////////////////////////////////////////////////////
template <typename _Tp>
void hash_table<_Tp>::put(_Tp &unit)
{
    if ( !isThere(unit) )
    {
        hash_node *n1 = *this->envPtr;
        *this->envPtr = new hash_node;
        (*this->envPtr)->payload = unit;
        (*this->envPtr)->next = n1;
    }
}

////////////////////////////////////////////////////////////
template <typename _Tp>
void hash_table<_Tp>::remove(_Tp &unit)
{
    if ( isThere(unit) )
    {
        hash_node *n1;
        n1 = *this->envPtr;
        *this->envPtr = (*this->envPtr)->next;
        
        n1->next = nullptr;
        delete n1;
    }
}

////////////////////////////////////////////////////////////
template <typename _Tp>
void hash_table<_Tp>::print(std::string str)
{
    std::cout << str << std::endl;
    std::cout << "\n\n" << std::endl;
    std::cout << "***********************" << std::endl;
    for (int i = 0; i < HASH_SPACE; i++)
    {
        std::cout << "Bucket: " << i << std::endl;
        
        this->envPtr = &roots[ i ];
        
        while ( *this->envPtr )
        {
            std::cout << (*this->envPtr)->payload << std::endl;
            this->envPtr = &(*this->envPtr)->next;
        }
        std::cout << std::endl;
    }
    std::cout << "***********************" << std::endl;
}

#endif /* hash_table_hpp */