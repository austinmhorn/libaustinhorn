//#include <austinhorn/graph.hpp>

#include <iostream>
#include <vector>
#include <map>

#define MAX_GRAPH_NODES 1000

struct edge 
{ 
    std::size_t src, dest, weight; 

    edge() = default;
    edge(std::size_t __s, std::size_t __d, std::size_t __w) 
        : src(__s), dest(__d), weight(__w) { }
};


typedef std::pair<std::size_t, std::size_t> pair;



template <typename _Tp> class __graph_node;





template <typename _Tp>
struct __grape_node_base
{
    typedef __graph_node<_Tp>  node_type;
    typedef node_type *        node_pointer;

    std::size_t  __id_;
};




template <typename _Tp> 
struct __graph_node
    : public __grape_node_base<_Tp>
{
    typedef __grape_node_base<_Tp>  base;

    _Tp  __value_;
    __graph_node() = default;
    __graph_node(std::size_t __id, const _Tp& __v)
    {
        base::__id_ = __id;
        __value_ = __v;
    }
};








template <typename _Tp>
class __graph_imp
{
    private:
        typedef __graph_node<_Tp>  node;
    protected:
        typedef std::size_t   size_type;
        typedef _Tp           value_type;
        typedef value_type *  pointer;
        typedef value_type &  reference;

        std::vector< node >               __nodes_;
        std::vector< std::vector<pair> >  __adj_;
        std::vector< edge >               __edges_ ;
        size_type                         __size_;

        __graph_imp()
            : __size_( 0 )
        {
            __nodes_.resize( MAX_GRAPH_NODES );
            __adj_.resize( MAX_GRAPH_NODES * 3 );
        }
        virtual ~__graph_imp()
        {
           clear();
        }

        void push_back(const value_type& __v)
        {
            node n = node(__size_, __v);
            
            if ( empty() )
            {
                __adj_[n.__id_].push_back( std::make_pair(0, 1) );
            }
            if ( size() )
            { 
                __adj_[n.__id_].push_back( std::make_pair( ( size() == 1 ) ? 0 : __size_-1 , 1) );
            }


            __nodes_[n.__id_] = n;
            edge e;
            e.src = n.__id_;
            e.dest = empty() ? 0 : __size_-1;
            e.weight = 1;
            insert_edge(e);
            ++__size_;
        }
        void insert_edge(const edge& __e)
        {
            __edges_.push_back(__e);
        }
        void connect_forward_traveral(std::size_t src, std::size_t dest)
        {
            const edge e = edge(src, dest, 1);

            if ( e.src >= __size_  ||  e.dest > __size_ )
            {
                std::cout << "out of range" << std::endl;
            }
            else
            {
                __adj_[e.src].push_back( std::make_pair(e.dest, e.weight) );
                insert_edge(e);
            }
        }

        void clear() noexcept
        {
            __nodes_.clear();
            __adj_.clear();
            __edges_.clear();
            __size_ = 0;
        }

        bool empty()
        {
            return (__size_ == 0);
        }

        size_type size() 
        {
            return __size_;
        }

        const std::vector< edge >& edges() const 
        {
            return __edges_;
        }

        void print()
        {
            for (auto &e : __edges_)
            {
                std::cout << "Node " << e.src << " contains value " << __nodes_.at(e.src).__value_ << " leads to " << e.dest << std::endl;
            }
        }
};





template <typename _Tp>
class graph
    : private __graph_imp<_Tp>
{
    private:
        typedef __graph_imp<_Tp>  base;
    public:        
        typedef typename base::size_type   size_type;
        typedef typename base::value_type  value_type;
        typedef typename base::pointer     pointer;
        typedef typename base::reference   reference;

        graph() : base::__graph_imp() { }
        ~graph() = default;

        void push_back(const value_type& __v) { base::push_back(__v); }
        void insert_edge(const edge& __e) { base::insert_edge(__e); }
        void connect_forward_traveral(std::size_t src, std::size_t dest) { base::connect_forward_traveral(src, dest); }

        void clear() noexcept { base::clear(); }
        bool empty() { return base::empty(); }
        size_type size() noexcept { return base::size(); }
        const std::vector< edge >& edges() const { return base::edges(); }

        void print() { base::print(); }

};





int main (int argc, const char **argv)
{
    
    graph<char> gr;

    for (int i = 65; i < 68; i++)
    {
        std::cout << "Inserting value " << char(i) << " into Node " << i-65 << std::endl;
        gr.push_back(char(i));
    }
    std::cout << "gr.size(): " << gr.size() << std::endl;

    gr.print();

    std::cout << "Connect forward traversal edge FROM Node 0 TO Node " << gr.size()-1 << std::endl;
    gr.connect_forward_traveral(0, gr.size()-1);

    gr.print();

    auto edges = gr.edges();



    return EXIT_SUCCESS;
}