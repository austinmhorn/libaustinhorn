#ifndef tree_hpp
#define tree_hpp

#include <iostream>
#include <cmath>


template <typename _Tp> struct __tree_node;
template <typename _Tp> struct __tree_node_base;


template <typename _Tp> 
struct __tree_node_base 
{
    typedef __tree_node<_Tp> __node;
    typedef __node *         __node_pointer;

    __node_pointer __left_{nullptr};  
    __node_pointer __right_{nullptr}; 

    operator __node_pointer() { return *this; }
};




template <typename _Tp> 
struct __tree_node 
    : public __tree_node_base<_Tp> 
{
    typedef __tree_node_base<_Tp>           __base;
    typedef typename __base::__node_pointer __node_pointer;

    __tree_node(const _Tp& __v, __node_pointer __n = nullptr, __node_pointer __p = nullptr) 
        : __value_{__v} 
        {
            __base::__left_ = __n;
            __base::__right_ = __p;
        }

    _Tp __value_;
}; 





template <
    typename _Tp,
    typename _Alloc = std::allocator<_Tp>
> class tree {

    private:

        typedef _Tp                     value_type;
        typedef __tree_node<value_type> node_type;
        typedef node_type *             node_pointer;

        node_pointer  __root_;
        node_pointer* __dptr_;
        unsigned      __size_;
        unsigned      __lvls_;

    public:
    
        tree() : __root_(nullptr), __dptr_(nullptr), __size_(0), __lvls_(0) {}
        ~tree() { altDelete(__root_); __root_ = nullptr; __size_ = 0; __lvls_ = 0; }
        
        void clear() 
        { 
            altDelete(__root_); 
            __root_ = nullptr; 
            __size_ = 0; 
        }
        
        bool isThere(_Tp &value)
        {
            __dptr_ = &__root_;
            
            while( *__dptr_  &&  (*__dptr_)->__value_ != value )
                if((*__dptr_)->__value_ > value)
                    __dptr_=&(*__dptr_)->__left_;
                else
                    __dptr_=&(*__dptr_)->__right_;
            
            return *__dptr_;
        }
        void insert(_Tp value)
        {
            if(  !isThere(value) )
            {
                *__dptr_ = new __tree_node(value);
                (*__dptr_)->__value_=value;
                
                __size_++;
            }
        }
        void remove(_Tp value)
        {
            if ( isThere(value) )
            {
                /// Node does NOT have 2 children
                if ( !(*__dptr_)->__left_ || !(*__dptr_)->__right_ )
                {
                    if ( (*__dptr_)->__left_ )
                    {
                        node_pointer p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__left_;
                        delete p;
                    }
                    else
                    {
                        node_pointer p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__right_;
                        delete p;
                    }
                }
                /// Node DOES have 2 children
                else
                {
                    node_pointer *dp = __dptr_;
                    
                    dp = &(*__dptr_)->__left_;
                    
                    while ( (*dp)->__right_ )
                        dp = &(*dp)->__right_;
                    
                    node_pointer p = *__dptr_;
                    node_pointer q = *dp;
                    
                    *dp = q->__left_;
                    q->__left_ = p->__left_;
                    q->__right_ = p->__right_;
                    *__dptr_ = q;
                    
                    delete p;
                }
                
                __size_--;
            }
        }
        void remove(_Tp &value)
        {
            if ( isThere(value) ) /// Verify value exists in the tree
            {
                if ( !(*__dptr_)->__left_ || !(*__dptr_)->__right_ ) /// Node does NOT have 2 children
                {
                    if ( (*__dptr_)->__left_ )
                    {
                        node_pointer p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__left_;
                        delete p;
                    }
                    else
                    {
                        node_pointer p = *__dptr_;
                        p = *__dptr_;
                        *__dptr_ = (*__dptr_)->__right_;
                        delete p;
                    }
                }
                else /// Node DOES have 2 children
                {
                    node_pointer *dp = __dptr_;
                    
                    dp = &(*__dptr_)->__left_;
                    
                    while ( (*dp)->__right_ )
                        dp = &(*dp)->__right_;
                    
                    node_pointer p = *__dptr_;
                    node_pointer q = *dp;
                    
                    *dp = q->__left_;
                    q->__left_ = p->__left_;
                    q->__right_ = p->__right_;
                    *__dptr_ = q;
                    
                    delete p;
                    
                    __size_--;
                }
            }
        }
        void balance() // Executes balance routine
        {
            balanceTree(&__root_);
        }
        std::size_t size() const
        {
            return __size_;
        }
        void showTreeStats()
        {
            std::cout<<"*********************************"<<std::endl;
            std::cout<<"# of nodes in tree          = \t"<<__size_<<std::endl;
            std::cout<<"# of levels in PERFECT tree = \t"<<calcLvls()<<std::endl;
            std::cout<<"# of levels in  THIS  tree  = \t"<<cntLvls(__root_)<<std::endl;
            std::cout<<"*********************************"<<std::endl;
        }
        void showLevels()
        {
            altShowLevels(__root_, 0);
        }
        void inorder()
        {
            if (__root_)
                altInorder(__root_);
        }
        void preorder()
        {
            if (__root_)
                altPreorder(__root_);
        }
        void postorder()
        {
            if (__root_)
                altPostorder(__root_);
        }
        
    private:
    
        ////////////////////////////////////////////////////////////////////////////
        ///< @function altDelete
        ///< @param *p Pointer to node being deleted
        ///< @brief Recursive deletion procedure with a public wrapper: void tree::delete()
        ////////////////////////////////////////////////////////////////////////////
        void altDelete(node_pointer p)
        {
            if( p )
            {
                altDelete(p->__left_);
                altDelete(p->__right_);
                delete p;
            }
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @function altShowLevels
        ///< @param *p Pointer to node being printed
        ///< @param level Level position of node being printed
        ///< @brief Recursive printing procedure with a public wrapper: void tree::showLevels()
        ////////////////////////////////////////////////////////////////////////////
        void altShowLevels(node_pointer p, int level)
        {
            if (p)
            {
                std::cout<<"Level - "<<level<<'\t'<<p->__value_<<std::endl;
                altShowLevels(p->__right_, level+1);
                altShowLevels(p->__left_, level+1);
            }
        }
        void altInorder(node_pointer p)
        {
            if (!p)
                return;
            altInorder(p->__left_); ///< Recur on left subtree
            std::cout<<p->__value_<<std::endl; ///< Print node data
            altInorder(p->__right_); ///< Recur on right subtree
        }
        void altPreorder(node_pointer p)
        {
            if (!p)
                return;
            std::cout<<p->__value_<<std::endl; ///< Print node data
            altPreorder(p->__left_); ///< Recur on left subtree
            altPreorder(p->__right_); ///< Recur on right subtree
            
        }
        void altPostorder(node_pointer p)
        {
            if (!p)
                return;
            altPostorder(p->__left_);  ///< Recurse on left subtree
            altPostorder(p->__right_); ///< Recurse on right subtree
            std::cout<<p->__value_<<std::endl; ///< Print node data
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @function cntLevels
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @brief - Counts the number of levels in a tree or subtree
        ////////////////////////////////////////////////////////////////////////////
        int cntLvls(node_pointer p)
        {
            __lvls_ = 0;
            altCntLvls(p, 1);
            return __lvls_;
        }
        ////////////////////////////////////////////////////////////////////////////
        ///< @function altCntLevels
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @brief - Recursive, called by cntLvls(node)
        ///< @see cntLevels
        ////////////////////////////////////////////////////////////////////////////
        void altCntLvls(node_pointer p, int i)
        {
            if(!p)
                return;
            else
            {
                if (i > __lvls_)
                    __lvls_ = i; 

                altCntLvls(p->__left_, i+1);
                altCntLvls(p->__right_, i+1);
            }
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @function calcLvls
        ///< @category - Diagnostic routine for balance
        ///< @param *p
        ///< @brief - Calculates the number of levels in a perfect tree
        ////////////////////////////////////////////////////////////////////////////
        int calcLvls()
        {
            return std::ceil(std::log(__size_+1)/std::log(2));
        }


        ////////////////////////////////////////////////////////////////////////////
        ///< @function rotL
        ///< @brief Rotates a simple, three-node, two-level tree LEFT while preserving the logical structure of the subtrees
        ///< @param **dp Double pointer to node
        ////////////////////////////////////////////////////////////////////////////
        void rotL(node_pointer *dp)
        {
            node_pointer p = *dp;
            *dp = p->__right_;
            p->__right_ = (*dp)->__left_;
            (*dp)->__left_ = p;
        }

        ////////////////////////////////////////////////////////////////////////////
        ///< @function rotL
        ///< @brief Rotates a simple, three-node, two-level tree RIGHT while preserving the logical structure of the subtrees
        ///< @param **dp Double pointer to node
        ////////////////////////////////////////////////////////////////////////////
        void rotR(node_pointer *dp)
        {
            node_pointer p = *dp;
            *dp=p->__left_;
            p->__left_=(*dp)->__right_;
            (*dp)->__right_=p;
        }

        void balanceTree(node_pointer *dp)
        {
            if ( !(*dp) )
                return; // already balanced
            else
            {
                balanceTree( &(*dp)->__left_  );
                balanceTree( &(*dp)->__right_ );
                
                int l_count = cntLvls( (*dp)->__left_  );
                int r_count = cntLvls( (*dp)->__right_ );
                
                if ( l_count == r_count  ||  (l_count+1) == r_count  ||  (l_count-1) == r_count )
                    return; // done
                else
                {
                    
                    if ( (*dp)->__left_  &&  (*dp)->__left_->__right_ ) // Node has inside left grandchild
                        rotL( &(*dp)->__left_ );
                    if ( (*dp)->__right_  &&  (*dp)->__right_->__left_ ) // Node has inside right grandchild
                        rotR( &(*dp)->__right_ );
                    if ( l_count < r_count ) // Rotate tree to smaller side
                        rotL( dp );
                    else if ( l_count > r_count )
                        rotR( dp );

                    balanceTree( dp ); // Balance
                }
            }
        }
};

#endif
