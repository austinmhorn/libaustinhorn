//
//  list.cpp
//  List
//
//  Created by Austin Horn on 2/2/23.
//

#include <regex>
#include <iostream>
/*
template <typename _Tp, typename _Alloc = std::allocator<_Tp>>
class List
{
private:
    typedef _Tp               value_type;
    typedef _Alloc            allocator_type;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
        
    struct Node
    {
        value_type data = value_type{};
        Node* next = nullptr;
        Node* prev = nullptr;
    };
    
    typedef struct Node* node_ptr;
    
    void altClear(node_ptr);
    void altPrint(node_ptr);
    void altBackward(node_ptr);
    
    node_ptr root;
    node_ptr *envPtr;
    size_type _m__size;
public:
    List();
    ~List();
    void clear();
    bool isRoom();
    bool isThere(reference);
    void insert(reference);
    void remove(reference);
    void print();
    void backward();
};

template <typename _Tp, typename _Alloc>
List<_Tp, _Alloc>::List()
    : root(nullptr)
    , envPtr(nullptr)
    , _m__size(0)
{
}

template <typename _Tp, typename _Alloc>
List<_Tp, _Alloc>::~List()
{
    altClear(this->root);
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::altClear(node_ptr node)
{
    if (node)
    {
        // Pointer to node's next node (possibly null)
        Node *nextNode = node->next;
        
        // Break link to next node
        node->next = nullptr;
        
        // Destroy node
        delete node;
        altClear(nextNode);
    }
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::altPrint(node_ptr node)
{
    if (node)
    {
        std::cout << node->data << std::endl;
        altPrint(node->next);
    }
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::altBackward(node_ptr node)
{
    if (node)
    {
        std::cout << node->data << std::endl;
        altBackward(node->prev);
    }
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::clear()
{
    altClear(this->root);
    this->root = nullptr;
    this->envPtr = nullptr;
}

template <typename _Tp, typename _Alloc>
bool List<_Tp, _Alloc>::isRoom()
{
    // Note: There is no standard way to do this with 100% certainty.
    Node *p = new Node;
    // Allocation failure returns nullptr
    if ( p )
    {
        delete p;
        return true;
    }
    else return false;
}

template <typename _Tp, typename _Alloc>
bool List<_Tp, _Alloc>::isThere(_Tp &value)
{
    this->envPtr = &this->root;
    while( *this->envPtr  &&  (*this->envPtr)->data != value )
        this->envPtr = &(*this->envPtr)->next;

    return *this->envPtr && (*this->envPtr)->data == value;
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::insert(_Tp &value)
{
    if ( !isThere(value) )
    {
        Node *p = *this->envPtr;
        *this->envPtr = new Node;
        (*this->envPtr)->data = value;
        (*this->envPtr)->next = p;
    }
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::remove(_Tp &value)
{
    if ( isThere(value) )
    {
        Node *p = *this->envPtr;
        *this->envPtr = (*this->envPtr)->next;
        p->next = nullptr;
        delete p;
    }
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::print()
{
    if (root)
        altPrint(root);
    else
        std::cout << "List is empty." << std::endl;
}

template <typename _Tp, typename _Alloc>
void List<_Tp, _Alloc>::backward()
{
    if (root)
        altPrint(root);
    else
        std::cout << "List is empty." << std::endl;
}
*/