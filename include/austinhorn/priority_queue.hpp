#include <iostream>
#include <austinhorn/iterator.hpp>

template <typename _Tp, typename _Compare> struct __queue_node;
template <typename _Tp, typename _Compare> struct __queue_node_base;

////////////////////////////////////////////////////
/// @struct __queue_node_base
/// @brief 
/// @tparam _Tp 
/// @tparam _Compare 
////////////////////////////////////////////////////
template <typename _Tp, typename _Compare> 
struct __queue_node_base 
{
    typedef __queue_node<_Tp, _Compare> * __node_pointer;
    __node_pointer __next_{nullptr};
};
////////////////////////////////////////////////////
/// @struct __queue_node
/// @brief 
/// @tparam _Tp 
/// @tparam _Compare 
////////////////////////////////////////////////////
template <typename _Tp, typename _Compare> 
struct __queue_node 
    : public __queue_node_base<_Tp, _Compare> 
{
    typedef __queue_node_base<_Tp, _Compare> __base;
    typedef typename __base::__node_pointer  __node_pointer;

    __queue_node(const _Tp& __v, __node_pointer __n = nullptr) 
        : __value_{__v} 
        {
            __base::__next_ = __n;
        }

    constexpr bool operator()( const __queue_node& lhs, const __queue_node& rhs ) const { return lhs.__value_ < rhs.__value_; }

    _Tp __value_;
}; 




////////////////////////////////////////////////////
/// @struct priority_queue
/// @brief 
/// @tparam _Tp 
/// @tparam _Compare 
////////////////////////////////////////////////////
template <typename _Tp, typename _Compare = std::less<>>
class priority_queue {
    private:
        typedef __queue_node<_Tp, _Compare> node_type;
        typedef node_type *                 node_pointer;
        typedef _Compare                    compare_func;

        node_pointer  __root_;
        node_pointer* __dp_;
        compare_func  __comp_; 
        std::size_t   __size_;

        void altCreate(node_pointer p);
        void altPrint(node_pointer p);
    public:
        priority_queue();
        ~priority_queue();
        priority_queue(const priority_queue& __pq);
        priority_queue& operator=(const priority_queue& __pq);
        
        priority_queue(priority_queue&&) noexcept = default;
        priority_queue& operator=(priority_queue&&) noexcept = default;

        void create();
        const _Tp& top() const;
        bool empty() const;
        std::size_t size() const;
        bool exists(_Tp &value);
        bool exists(const _Tp &value);
        void push(_Tp &value);
        void push(const _Tp &value);
        void pop();
        void remove(_Tp &value);
        void remove(const _Tp &value);
        void print();


};

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::altCreate(node_pointer p)
{
    if (p)
    {
        node_pointer q = p->__next_; // Pointer to node's next node (possibly null)
        p->__next_ = nullptr;        // Break link to next node
        delete p;                    // Destroy node
        altCreate(q);                // Recursively destroy next node
    }
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::altPrint(node_pointer p)
{
    if (p)
    {
        std::cout << p->__value_ << std::endl;
        altPrint(p->__next_);
    }
}

template <typename _Tp, typename _Compare>
inline priority_queue<_Tp, _Compare>::priority_queue()
{
    this->__root_ = nullptr;
    this->__dp_ = nullptr;
    this->__size_ = 0;
}

template <typename _Tp, typename _Compare>
inline priority_queue<_Tp, _Compare>::~priority_queue()
{
    altCreate(this->__root_); 
    this->__root_ = nullptr;
    this->__dp_ = nullptr;
    this->__size_ = 0;
}

template <typename _Tp, typename _Compare>
inline priority_queue<_Tp, _Compare>::priority_queue(const priority_queue &__pq)
{ 
    __root_ = __pq.__root_; 
    __dp_   = __pq.__dp_; 
    __comp_ = __pq.__comp_; 
    __size_ = __pq.__size_; 
}

template <typename _Tp, typename _Compare>
inline priority_queue<_Tp, _Compare> &priority_queue<_Tp, _Compare>::operator=(const priority_queue &__pq)
{
    if (this != &__pq) 
    {
        __root_ = __pq.__root_;
        __dp_   = __pq.__dp_;
        __comp_ = __pq.__comp_;
        __size_ = __pq.__size_;
    }
    return *this;
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::create()
{
    altCreate(this->__root_);
    this->__root_ = nullptr;
    this->__dp_   = nullptr;
    this->__size_ = 0;
}
template <typename _Tp, typename _Compare>
inline const _Tp& priority_queue<_Tp, _Compare>::top() const 
{ 
    return __root_->__value_; 
}
template <typename _Tp, typename _Compare>
inline bool priority_queue<_Tp, _Compare>::empty() const 
{ 
    return !__size_; 
}
template <typename _Tp, typename _Compare>
inline std::size_t priority_queue<_Tp, _Compare>::size() const 
{ 
    return __size_; 
}

template <typename _Tp, typename _Compare>
inline bool priority_queue<_Tp, _Compare>::exists(_Tp &value)
{
    this->__dp_ = &this->__root_;

    while( *this->__dp_  &&  __comp_((*this->__dp_)->__value_, value) )
        this->__dp_ = &(*this->__dp_)->__next_;

    return *this->__dp_ && (*this->__dp_)->__value_ == value;
}

template <typename _Tp, typename _Compare>
inline bool priority_queue<_Tp, _Compare>::exists(const _Tp &value)
{
    this->__dp_ = &this->__root_;

    while( *this->__dp_  &&  __comp_((*this->__dp_)->__value_, value) )
        this->__dp_ = &(*this->__dp_)->__next_;

    return *this->__dp_ && (*this->__dp_)->__value_ == value;
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::push(_Tp &value)
{    
    if ( !exists(value) )
    {
        node_pointer p = *this->__dp_;
        *this->__dp_ = new node_type(value, p);
        ++__size_;
    }
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::push(const _Tp &value)
{    
    if ( !exists(value) )
    {
        node_pointer p = *this->__dp_;
        *this->__dp_ = new node_type(value, p);
        ++__size_;
    }
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::pop()
{    
    remove(__root_->__value_);
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::remove(_Tp &value)
{
    if ( exists(value) )
    {
        node_pointer p = *this->__dp_;
        *this->__dp_ = (*this->__dp_)->__next_;
        
        p->__next_ = nullptr;
        delete p;
        --__size_;
    }
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::remove(const _Tp &value)
{
    if ( exists(value) )
    {
        node_pointer p = *this->__dp_;
        *this->__dp_ = (*this->__dp_)->__next_;
        
        p->__next_ = nullptr;
        delete p;
        --__size_;
    }
}

template <typename _Tp, typename _Compare>
inline void priority_queue<_Tp, _Compare>::print()
{
    if (__root_)
        altPrint(__root_);
}