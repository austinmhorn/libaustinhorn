#ifndef binary_tree_hpp
#define binary_tree_hpp

#include <iostream>
#include <cmath>


template <typename _Tp> struct __binary_tree_node;
template <typename _Tp> struct __binary_tree_node_base;


template <typename _Tp> 
struct __binary_tree_node_base 
{
    typedef __binary_tree_node<_Tp> __node;
    typedef __node *         __node_pointer  ;

    __node_pointer   __left_{nullptr};  
    __node_pointer   __right_{nullptr}; 

};




template <typename _Tp> 
struct __binary_tree_node 
    : public __binary_tree_node_base<_Tp> 
{
    typedef __binary_tree_node_base<_Tp>      __base;
    typedef typename __base::__node_pointer   __node_pointer;

    __binary_tree_node(const _Tp& __v, __node_pointer __n = nullptr, __node_pointer __p = nullptr) 
        { __value_ = __v; __base::__left_ = __n; __base::__right_ = __p; }

    _Tp __value_;
}; 





template <typename _Tp> 
class __binary_tree_imp 
{
        typedef __binary_tree_node<_Tp>  __node;
        typedef __node *                 __node_pointer;
        typedef __node **                __env_pointer;
    protected:
        typedef std::size_t                                size_type;
        typedef std::ptrdiff_t                             difference_type;
        typedef _Tp                                        value_type;
        typedef value_type *                               pointer;
        typedef value_type &                               reference;
        typedef const value_type                           const_reference;

        __node_pointer __root_;
        __env_pointer  __dptr_;
        unsigned       __size_;
        unsigned       __lvls_;
    
        __binary_tree_imp() 
            : __root_(nullptr), __dptr_(nullptr), __size_(0), __lvls_(0) {}
        virtual ~__binary_tree_imp() 
            { clear(); }

        __binary_tree_imp(const __binary_tree_imp&) = default;
        __binary_tree_imp& operator=(const __binary_tree_imp&) = default;

        __binary_tree_imp(__binary_tree_imp&&) noexcept = default;
        __binary_tree_imp& operator=(__binary_tree_imp&&) noexcept = default;
        
        void clear() 
        { 
            __recursive_destroy(__root_); 
            __root_ = nullptr; 
            __size_ = 0; 
            __lvls_ = 0;
        }
        
        bool exists(const value_type &__v)
        {
            __dptr_ = &__root_;
            
            while( *__dptr_  &&  (*__dptr_)->__value_ != __v )
                if((*__dptr_)->__value_ > __v)
                    __dptr_=&(*__dptr_)->__left_;
                else
                    __dptr_=&(*__dptr_)->__right_;
            
            return *__dptr_;
        }
        void insert(const value_type &__v)
        {
            if( !exists(__v) )
            {
                *__dptr_ = new __binary_tree_node(__v);
                (*__dptr_)->__value_ = __v;
                __size_++;
            }
        }
        void insert(value_type &&__v)
        {
            if( !exists(__v) )
            {
                *__dptr_ = new __binary_tree_node(__v);
                (*__dptr_)->__value_ = __v;
                __size_++;
            }
        }
        void remove(const value_type &__v)
        {
            if ( exists(__v) )
            {
                /// Node does NOT have 2 children
                if ( !(*__dptr_)->__left_ || !(*__dptr_)->__right_ )
                {
                    if ( (*__dptr_)->__left_ )
                    {
                        __node_pointer  p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__left_;
                        delete p;
                    }
                    else
                    {
                        __node_pointer  p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__right_;
                        delete p;
                    }
                }
                /// Node DOES have 2 children
                else
                {
                    __node_pointer  *dp = __dptr_;
                    
                    dp = &(*__dptr_)->__left_;
                    
                    while ( (*dp)->__right_ )
                        dp = &(*dp)->__right_;
                    
                    __node_pointer  p = *__dptr_;
                    __node_pointer  q = *dp;
                    
                    *dp = q->__left_;
                    q->__left_ = p->__left_;
                    q->__right_ = p->__right_;
                    *__dptr_ = q;
                    
                    delete p;
                }
                
                --__size_;
            }
        }
        void remove(value_type &&__v)
        {
            if ( exists(__v) ) /// Verify value exists in the __binary_tree_imp
            {
                if ( !(*__dptr_)->__left_ || !(*__dptr_)->__right_ ) /// Node does NOT have 2 children
                {
                    if ( (*__dptr_)->__left_ )
                    {
                        __node_pointer  p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__left_;
                        delete p;
                    }
                    else
                    {
                        __node_pointer  p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__right_;
                        delete p;
                    }
                }
                else /// Node DOES have 2 children
                {
                    __node_pointer  *dp = __dptr_;
                    
                    dp = &(*__dptr_)->__left_;
                    
                    while ( (*dp)->__right_ )
                        dp = &(*dp)->__right_;
                    
                    __node_pointer  p = *__dptr_;
                    __node_pointer  q = *dp;
                    
                    *dp = q->__left_;
                    q->__left_ = p->__left_;
                    q->__right_ = p->__right_;
                    *__dptr_ = q;
                    
                    delete p;
                }

                --__size_;
            }
        }
        void balance() // Executes balance routine
        {
            __recursive_balance(&__root_);
        }
        std::size_t size() const
        {
            return __size_;
        }
        void showTreeStats()
        {
            std::cout<<"*********************************"<<std::endl;
            std::cout<<"# of nodes in __binary_tree_imp          = \t"<<__size_<<std::endl;
            std::cout<<"# of levels in PERFECT __binary_tree_imp = \t"<<__calculate_perfect()<<std::endl;
            std::cout<<"# of levels in  THIS  __binary_tree_imp  = \t"<<__count_lvls(__root_)<<std::endl;
            std::cout<<"*********************************"<<std::endl;
        }
        void showLevels()
        {
            __recursive_show_lvls(__root_, 0);
        }
        void inorder()
        {
            if (__root_)
                __recursive_inorder(__root_);
        }
        void preorder()
        {
            if (__root_)
                __recursive_preorder(__root_);
        }
        void postorder()
        {
            if (__root_)
                __recursive_postorder(__root_);
        }
        
    private:
    
        ////////////////////////////////////////////////////////////////////////////
        ///< @param *p Pointer to node being deleted
        ///< @brief Recursive deletion procedure with a public wrapper: void __binary_tree_imp::delete()
        ////////////////////////////////////////////////////////////////////////////
        void __recursive_destroy(__node_pointer  p)
        {
            if( p )
            {
                __recursive_destroy(p->__left_);
                __recursive_destroy(p->__right_);
                delete p;
            }
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @param *p Pointer to node being printed
        ///< @param level Level position of node being printed
        ///< @brief Recursive printing procedure with a public wrapper: void __binary_tree_imp::showLevels()
        ////////////////////////////////////////////////////////////////////////////
        void __recursive_show_lvls(__node_pointer  p, int level)
        {
            if (p)
            {
                std::cout<<"Level - "<<level<<'\t'<<p->__value_<<std::endl;
                __recursive_show_lvls(p->__right_, level+1);
                __recursive_show_lvls(p->__left_, level+1);
            }
        }
        void __recursive_inorder(__node_pointer p)
        {
            if (!p)
                return;
            __recursive_inorder(p->__left_); ///< Recur on left subtree
            std::cout<<p->__value_<<std::endl; ///< Print node data
            __recursive_inorder(p->__right_); ///< Recur on right subtree
        }
        void __recursive_preorder(__node_pointer p)
        {
            if (!p)
                return;
            std::cout<<p->__value_<<std::endl; ///< Print node data
            __recursive_preorder(p->__left_); ///< Recur on left subtree
            __recursive_preorder(p->__right_); ///< Recur on right subtree
            
        }
        void __recursive_postorder(__node_pointer p)
        {
            if (!p)
                return;
            __recursive_postorder(p->__left_);  ///< Recurse on left subtree
            __recursive_postorder(p->__right_); ///< Recurse on right subtree
            std::cout<<p->__value_<<std::endl; ///< Print node data
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @brief - Counts the number of levels in a __binary_tree_imp or subtree
        ////////////////////////////////////////////////////////////////////////////
        int __count_lvls(__node_pointer p)
        {
            __lvls_ = 0;
            __recursive_count_lvls(p, 1);
            return __lvls_;
        }
        ////////////////////////////////////////////////////////////////////////////
        ///< @brief - Recursive, called by __count_lvls(node)
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @param i 
        ///< @see cntLevels
        ////////////////////////////////////////////////////////////////////////////
        void __recursive_count_lvls(__node_pointer p, int i)
        {
            if(!p)
                return;
            else
            {
                if (i > __lvls_)
                    __lvls_ = i; 

                __recursive_count_lvls(p->__left_, i+1);
                __recursive_count_lvls(p->__right_, i+1);
            }
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @function __calculate_perfect
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @brief - Calculates the number of levels in a perfect __binary_tree_imp
        ////////////////////////////////////////////////////////////////////////////
        int __calculate_perfect()
        {
            return std::ceil(std::log(__size_+1)/std::log(2));
        }


        ////////////////////////////////////////////////////////////////////////////
        ///< @category - Diagnostic routine for balance
        ///< @brief Rotates a simple, three-node, two-level __binary_tree_imp LEFT while preserving the logical structure of the subtrees
        ///< @param **dp Double pointer to node
        ////////////////////////////////////////////////////////////////////////////
        void __rotate_left(__node_pointer *dp)
        {
            __node_pointer  p = *dp;
            *dp = p->__right_;
            p->__right_ = (*dp)->__left_;
            (*dp)->__left_ = p;
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @category - Diagnostic routine for balance
        ///< @brief Rotates a simple, three-node, two-level __binary_tree_imp RIGHT while preserving the logical structure of the subtrees
        ///< @param **dp Double pointer to node
        ////////////////////////////////////////////////////////////////////////////
        void __rotate_right(__node_pointer *dp)
        {
            __node_pointer  p = *dp;
            *dp=p->__left_;
            p->__left_=(*dp)->__right_;
            (*dp)->__right_=p;
        }

        void __recursive_balance(__node_pointer *dp)
        {
            if ( !(*dp) )
                return; // already balanced
            else
            {
                __recursive_balance( &(*dp)->__left_  );
                __recursive_balance( &(*dp)->__right_ );
                
                int l_count = __count_lvls( (*dp)->__left_  );
                int r_count = __count_lvls( (*dp)->__right_ );
                
                if ( l_count == r_count  ||  (l_count+1) == r_count  ||  (l_count-1) == r_count )
                    return; // done
                else
                {
                    if ( (*dp)->__left_  &&  (*dp)->__left_->__right_ ) // Node has inside left grandchild
                        __rotate_left( &(*dp)->__left_ );
                    if ( (*dp)->__right_  &&  (*dp)->__right_->__left_ ) // Node has inside right grandchild
                        __rotate_right( &(*dp)->__right_ );
                    if ( l_count < r_count ) // Rotate __binary_tree_imp to smaller side
                        __rotate_left( dp );
                    else if ( l_count > r_count )
                        __rotate_right( dp );

                    __recursive_balance( dp ); // Balance
                }
            }
        }
};



template <typename _Tp>
class binary_tree
    : private __binary_tree_imp<_Tp>
{
        typedef __binary_tree_imp<_Tp> base;
    public:        
        typedef typename base::size_type               size_type;
        typedef typename base::value_type              value_type;
        typedef typename base::pointer                 pointer;
        typedef typename base::reference               reference;
        typedef typename base::const_reference         const_reference;

        binary_tree() : base::__binary_tree_imp() {}
        virtual ~binary_tree() = default;

        binary_tree(const binary_tree&) = default;
        binary_tree& operator=(const binary_tree&) = default;

        binary_tree(binary_tree&&) noexcept = default;
        binary_tree& operator=(binary_tree&&) noexcept = default;

        void clear() { base::clear(); } 
        bool exists(const value_type &__v) { return base::exists(__v); }
        void insert(const value_type &__v) { base::insert(__v); }
        void insert(value_type &&__v) const noexcept { base::insert(__v); }
        void remove(const value_type &__v) { base::remove(__v); }
        void remove(value_type &&__v) const noexcept { base::remove(); }
        void balance() { base::balance(); }
        std::size_t size() const { return base::size(); }
        void showTreeStats() { base::showTreeStats(); }
        void showLevels() { base::showLevels(); }
        void inorder() { base::inorder(); }
        void preorder() { base::preorder(); }
        void postorder()  { base::postorder(); }
};


#endif /* binary_tree_hpp */
