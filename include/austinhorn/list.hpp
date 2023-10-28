#ifndef __list_hpp
#define __list_hpp

#include <regex>
#include <iostream>
#include <cassert>
#include <austinhorn/forward_list.hpp>

#define assertm(exp, msg) assert(((void)msg, exp))

//////////////////////////////////////////////////////////
/// @class __list_imp
/// @brief Implementation of list
/// @tparam _Tp 
//////////////////////////////////////////////////////////
template <typename _Tp>
class __list_imp
{
    private:
        typedef __list_node<_Tp>  __node;
        typedef __node *          __node_pointer;
        typedef __node **         __env_pointer;
            
        template <typename> friend class list;
    protected:
        typedef std::size_t                                size_type;
        typedef std::ptrdiff_t                             difference_type;
        typedef _Tp                                        value_type;
        typedef value_type *                               pointer;
        typedef value_type &                               reference;
        typedef const value_type                           const_reference;
        typedef __list_iterator<value_type>                iterator;
        typedef __const_list_iterator<value_type>          const_iterator;
        typedef __reverse_list_iterator<value_type>        reverse_iterator;
        typedef __const_reverse_list_iterator<value_type>  const_reverse_iterator;
        
        __node_pointer __head_;
        __node_pointer __tail_;
        __env_pointer  __dptr_;
        size_type      __size_;

        __list_imp() 
            : __head_(nullptr), __tail_(nullptr), __dptr_(nullptr), __size_(0) {}
        virtual ~__list_imp() 
            {  clear(); }

        __list_imp(const __list_imp&) = default;
        __list_imp& operator=(const __list_imp&) = default;

        __list_imp(__list_imp&&) noexcept = default;
        __list_imp& operator=(__list_imp&&) noexcept = default;

        void push_front(const value_type& __v)
        {
            try
            {
                if ( __size_ + 1 > max_size() ) { throw std::runtime_error("list reached maximum size"); }
                
                __node_pointer __p = new __node(__v);

                if ( empty() )
                {
                    __head_ = __p;
                    __tail_ = __p;
                }
                else
                {
                    __head_->__prev_ = __p;
                    __p->__next_ = __head_;
                    __head_ = __p;
                }
                ++__size_;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        void push_front(value_type&& __v)
        {
            try
            {
                if ( __size_ + 1 > max_size() ) { throw std::runtime_error("list reached maximum size"); }

                __node_pointer __p = new __node(std::move(__v));

                if ( empty() )
                {
                    __head_ = __p;
                    __tail_ = __p;
                }
                else
                {
                    __head_->__prev_ = __p;
                    __p->__next_ = __head_;
                    __head_ = __p;
                }
                ++__size_;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        void push_back(const value_type& __v)
        {
            try
            {
                if ( __size_ + 1 > max_size() ) { throw std::runtime_error("list reached maximum size"); }
                    
                __node_pointer __p = new __node(__v);

                if ( empty() )
                {
                    __head_ = __p;
                    __tail_ = __p;
                }
                else
                {
                    __tail_->__next_ = __p;
                    __p->__prev_ = __tail_;
                    __tail_ = __p;
                }
                ++__size_;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        void push_back(value_type&& __v)
        {
            try
            {
                if ( __size_ + 1 > max_size() )
                    throw std::runtime_error("list reached maximum size");

                __node_pointer __p = new __node(std::move(__v));

                if ( empty() )
                {
                    __head_ = __p;
                    __tail_ = __p;
                }
                else
                {
                    __tail_->__next_ = __p;
                    __p->__prev_ = __tail_;
                    __tail_ = __p;
                }
                ++__size_;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            
        }
        void pop_front()
        {
            if ( !empty() )
            {
                __node_pointer p = __head_;
                __head_ = __head_->__next_;
                __head_->__prev_ = nullptr;
                p->__next_ = nullptr;
                delete p;
                --__size_;
            }
        }
        void pop_back()
        {
            if ( !empty() )
            {
                __node_pointer p = __tail_;
                __tail_ = __tail_->__prev_;
                __tail_->__next_ = nullptr;
                p->__prev_ = nullptr;
                delete p;
                --__size_;
            }
        }
        void clear() noexcept
        {
            __recursive_destroy(__head_);
            __head_ = nullptr;
            __tail_ = nullptr;
            __dptr_ = nullptr;
            __size_ = 0;
        }

        size_type size() const noexcept 
        { 
            return __size_;
        }
        size_type max_size() const noexcept
        {
            return std::numeric_limits<difference_type>::max();
        }
        bool empty() const noexcept 
        { 
            return (__size_ == 0);
        }
        bool exists(const value_type& __v)
        {
            __dptr_ = &__head_;
            while( *__dptr_  &&  (*__dptr_)->__value_ != __v)
                __dptr_ = &(*__dptr_)->__next_;
            return *__dptr_  &&  (*__dptr_)->__value_ == __v;
        }
        const_iterator remove(const value_type& __v)
        {
            auto iter = begin();

            while ( iter != end() )
            {
                if ( *iter == __v )
                    { __destroy_iter(iter); break; }
                else
                    ++iter;
            }

            return iter;
        }
        size_type remove_all(const value_type& __v)
        {
            size_type removed = 0;
            auto iter = begin();

            while ( iter != end() )
            {
                if ( *iter == __v )
                    { __destroy_iter(iter++); ++removed; }   
                else
                    ++iter;
            }

            return removed;
        }
        const_iterator erase_before(const_iterator pos)
        {
            pos = ( pos == const_iterator(end().__as_pointer()) ) ? const_iterator(__tail_) : --pos;
            __destroy_iter(pos);
            return pos;
        }
        const_iterator erase_after(const_iterator pos)
        {
            __destroy_iter(++pos);
            return pos;
        }
        const_iterator erase(const_iterator pos)
        {
            __destroy_iter(pos);
            return pos;
        }
        size_type erase(const_iterator first, const_iterator last)
        {
            auto pos = first;
            auto fin = ( const_iterator(__tail_) == last ) ? const_iterator(__tail_) : ++last;
            size_type erased = 0;

            while ( pos != fin )
                { __destroy_iter(pos++); ++erased; }

            if ( fin.__as_pointer() == __head_  ||  fin.__as_pointer() == __tail_ )
                { __destroy_iter(pos); ++erased; }

            return erased;
        }
        template <typename UnaryPredicate>
        size_type erase_if(iterator first, iterator last, UnaryPredicate p)
        {
            assertm(std::is_object_v<UnaryPredicate>, "list::erase_if : UnaryPredicate type is not an object"); 
            auto pos = first;
            size_type removed = 0;

            while ( pos != end() )
            {
                if ( !p(*pos) )
                    { __destroy_iter(pos++); ++removed; }
                else
                    ++pos;
            }

            return removed;
        }

        iterator insert(const_iterator pos, const value_type& __v)
        {
            if ( pos == const_iterator(__head_) )
                { push_front(__v); return pos; }
            else if ( pos == const_iterator(__tail_) )
                { push_back(__v); return pos; }

            __node_pointer p = pos.__as_pointer();
            __node_pointer q = pos.__as_pointer()->__next_;
            __node_pointer n = new __node(__v, p, q);

            p->__next_ = n;
            q->__prev_ = n;
            ++__size_;

            return pos;
        }

        iterator find(const value_type& __v, int occ = 1)
        {
            iterator iter = begin();
            int found = 0;
            
            for (; iter != end(); ++iter)
                if ( *iter == __v )
                    { found++; break; }
  
            while ( occ != found  &&  iter != end() )
            {    
                ++iter;
                if ( *iter == __v )
                    found++;
            }

            return iter;
        }
        iterator find_first_of(const value_type& __v)
        {
            iterator iter = begin();

            for (; iter != end(); ++iter)
                if ( *iter == __v )
                    break;

            return iter;
        }
        iterator find_last_of(const value_type& __v)
        {
            iterator iter = end();

            for (auto it = begin(); it != end(); ++it)
                if ( *it == __v )
                    iter = it;

            return iter;
        }
        reference front() noexcept 
        { 
            assertm(!empty(), "list::front called on empty list"); 
            return __head_->__value_; 
        }
        const_reference front() const noexcept 
        { 
            assertm(!empty(), "list::front called on empty list"); 
            return __head_->__value_; 
        }
        reference back() noexcept 
        { 
            assertm(!empty(), "list::front called on empty list"); 
            return __tail_->__value_; 
        }
        const_reference back() const noexcept 
        { 
            assertm(!empty(), "list::front called on empty list"); 
            return __tail_->__value_; 
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
        reverse_iterator rbegin() noexcept 
        { 
            return reverse_iterator(__tail_); 
        }
        reverse_iterator rend() noexcept 
        { 
            return reverse_iterator(__head_->__prev_); 
        }
        const_reverse_iterator rbegin() const noexcept 
        { 
            return const_reverse_iterator(__tail_);  
        }
        const_reverse_iterator rend() const noexcept 
        { 
            return const_reverse_iterator(__head_->__prev_); 
        }

        template <typename _Compare = std::less<>>
        void sort()
        {
             _Compare compare;
            __node_pointer p = __head_;
            __node_pointer q = __head_->__next_;

            while ( p && q )
            {
                while ( q )
                {
                    if ( compare(q->__value_, p->__value_) )
                        std::swap( p->__value_, q->__value_ );
                    q = q->__next_;
                }

                p = p->__next_;
                q = p->__next_;
            }
        }

        void unique()
        {
            __node_pointer p = __head_;
            __node_pointer q = __head_->__next_;

            while ( p && q )
            {
                if ( p->__value_ == q->__value_ )
                {
                    __dptr_ = &q;
                    __node_pointer r = *__dptr_;
                    *__dptr_ = (*__dptr_)->__next_;

                    delete r;
                    --__size_;

                    p->__next_ = *__dptr_;

                    __node_pointer s = *__dptr_;
                    if ( s )
                        s->__prev_ = p;

                    q = p;
                }

                p = q;
                q = p->__next_;
            }

            __tail_ = p;
        }

        void reverse()
        {
            __node_pointer __hc = __head_;
            __node_pointer __hp = nullptr;
            __node_pointer __hn = nullptr;
            __node_pointer __tc = __tail_;
            __node_pointer __tp = nullptr;
            __node_pointer __tn = nullptr;
    
            while ( __hc && __tc ) 
            {
                // Store current node's next node
                __hn = __hc->__next_;
                __tn = __tc->__prev_;
                // Reverse current node's pointer
                __hc->__next_ = __hp;
                __tc->__prev_ = __tp;
                // Advance pointers one position ahead
                __hp = __hc;
                __hc = __hn;
                __tp = __tc;
                __tc = __tn;
            }
            __head_ = __hp;
            __tail_ = __tp;
        }

        void merge( const list<_Tp>& src, const list<_Tp>& other )
        {
            if ( &src != &other )
            {
                for ( auto i : other )
                {
                    const_iterator pos = find(i);
                    pos = ( pos == const_iterator(end().__as_pointer()) ) ? const_iterator(__tail_) : pos;
                    insert(pos, i);
                }
            }
        }
        template <typename _Compare = std::less<>>
        void merge( const list<_Tp>& other )
        {
            _Compare comp;
            
            for ( auto i : other )
            {
                auto iter = begin();
                while ( iter != end() )
                {
                    if ( comp(*iter, i) )
                        insert(iter, i);
                    ++iter;
                }
            }
        }

        void print_fwd()
        {
            __recursive_fwd_print(__head_);
            std::cout << std::endl;
        }
        void print_bwd()
        {
            __recursive_bwd_print(__tail_);
            std::cout << std::endl;
        }

        const_iterator __destroy_iter(const_iterator pos)
        {
            //std::cout << "destroy iter: " << *pos << std::endl;
            if ( pos == const_iterator(__head_) )
                { pop_front(); return pos; }
            else if ( pos == const_iterator(__tail_) )
                { pop_back(); return pos; }

            __node_pointer p = pos.__as_pointer();
            __node_pointer q = pos.__as_pointer()->__prev_;
            __node_pointer r = pos.__as_pointer()->__next_;

            q->__next_ = r;
            r->__prev_ = q;
            p->__prev_ = nullptr;
            p->__next_ = nullptr;
            delete p;
            --__size_;
            return pos;
        }

        void __recursive_destroy(__node_pointer __p) noexcept
        {
            if ( __p )
            {
                __node_pointer q = __p->__next_; 
                __p->__prev_ = nullptr;
                __p->__next_ = nullptr;
                delete __p; 
                __recursive_destroy(q); 
            }
        }

        void __recursive_fwd_print(__node_pointer __p) noexcept
        {
            if ( __p )
            {
                std::cout << __p->__value_ << " ";
                __recursive_fwd_print(__p->__next_);
            }
        }
        void __recursive_bwd_print(__node_pointer __p) noexcept
        {
            if ( __p )
            {
                std::cout << __p->__value_ << " ";
                __recursive_bwd_print(__p->__prev_);
            }
        }
};

////////////////////////////////////////////////////////////////
/// @class list
/// @brief Doubly linked list
/// @tparam _Tp Value type the list is storing
////////////////////////////////////////////////////////////////
template <typename _Tp>
class list 
    : private __list_imp<_Tp>
{
    private:
        typedef __list_imp<_Tp> base;
    public:        
        typedef typename base::size_type               size_type;
        typedef typename base::value_type              value_type;
        typedef typename base::pointer                 pointer;
        typedef typename base::reference               reference;
        typedef typename base::const_reference         const_reference;
        typedef typename base::iterator                iterator;
        typedef typename base::const_iterator          const_iterator;
        typedef typename base::reverse_iterator        reverse_iterator;
        typedef typename base::const_reverse_iterator  const_reverse_iterator;
    
        list() : base::__list_imp() {}
        virtual ~list() = default;
        list(const list&) = default;
        list& operator=(const list&) = default;
        list(list&&) noexcept = default;
        list& operator=(list&&) noexcept = default;

        void push_front(const value_type& __v) { base::push_front(__v); }
        void push_front(value_type&& __v) { base::push_front(__v); }
        void push_back(const value_type& __v) { base::push_back(__v); }
        void push_back(value_type&& __v) { base::push_back(__v); }
        void pop_front() { base::pop_front(); }
        void pop_back() { base::pop_back(); }
        void clear() { base::clear(); }

        size_type size() const noexcept { return base::size(); }
        size_type max_size() const noexcept { return base::max_size(); }

        bool empty() const noexcept { return base::empty(); }
        bool exists(const value_type& __v) { return base::exists(__v); }
        const_iterator remove(const value_type& __v) { return base::remove(__v); }
        size_type remove_all(const value_type& __v) { return base::remove_all(__v); }
        const_iterator erase_before(const_iterator pos) { return base::erase_before(pos); }
        const_iterator erase_after(const_iterator pos) { return base::erase_after(pos); }
        const_iterator erase(const_iterator pos) { return base::erase(pos); }
        size_type erase(const_iterator first, const_iterator last) { return base::erase(first, last); }
        template <typename UnaryPredicate>
        size_type erase_if(iterator first, iterator last, UnaryPredicate p) { return base::template erase_if<UnaryPredicate>(first, last, p); }
        iterator insert(const_iterator pos, const value_type& __v) { return base::insert(pos, __v); }
        iterator find(const value_type& __v, int occ = 1) { return base::find(__v, occ); }
        iterator find_first_of(const value_type& __v) { return base::find_first_of(__v); }
        iterator find_last_of(const value_type& __v) { return base::find_last_of(__v); }

        reference front() noexcept { return base::front(); }
        const_reference front() const noexcept { return base::front(); }
        reference back() noexcept { return base::back(); }
        const_reference back() const noexcept { return base::back(); }

        iterator begin() noexcept { return base::begin(); }
        iterator end() noexcept { return base::end(); }
        const_iterator begin() const noexcept { return base::begin(); }
        const_iterator end() const noexcept { return base::end(); }
        reverse_iterator rbegin() noexcept { return base::rbegin(); }
        reverse_iterator rend() noexcept { return base::rend(); }
        const_reverse_iterator rbegin() const noexcept { return base::rbegin(); }
        const_reverse_iterator rend() const noexcept { return base::rend(); }
        
        template <typename _Compare = std::less<>>
        void sort() { base:: template sort<_Compare>(); }
        void unique() { base::unique(); }
        void reverse() { base::reverse(); }
        
        void merge( const list<_Tp>& other ) { base::merge( *this, other ); }
        template <typename _Compare = std::less<>>
        void merge( const list<_Tp>& other ) { base::template merge<_Compare>(other); }

        void print_fwd() { base::print_fwd(); } // Debug forward print function
        void print_bwd() { base::print_bwd(); } // Debug backward print function

};

#endif /* __list_hpp */