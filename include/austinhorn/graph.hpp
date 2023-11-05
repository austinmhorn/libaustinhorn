#include <utility>
#include <vector>
#include <iostream>

struct Edge { unsigned int src, dest, weight; };    ///< Structure representing an edge between two vertices
typedef std::pair<unsigned int, unsigned int> Pair; ///< Used by Edge structure to pair multiple dest/weights to one src

// Edges of planar dodecahedron
static const std::vector<Edge> s_planar_dodecahedron_edges
{
    { 1,  2, 1}, { 1,  5, 1}, { 1,  8, 1}, ///< Outer Layer
    { 2,  1, 1}, { 2,  3, 1}, { 2, 10, 1},
    { 3,  2, 1}, { 3,  4, 1}, { 3, 12, 1},
    { 4,  3, 1}, { 4,  5, 1}, { 4, 14, 1},
    { 5,  1, 1}, { 5,  4, 1}, { 5,  6, 1},
    { 6,  5, 1}, { 6,  7, 1}, { 6, 15, 1}, ///< Middle Layer
    { 7,  6, 1}, { 7,  8, 1}, { 7, 17, 1},
    { 8,  1, 1}, { 8,  7, 1}, { 8,  9, 1},
    { 9,  8, 1}, { 9, 10, 1}, { 9, 18, 1},
    {10,  2, 1}, {10,  9, 1}, {10, 11, 1},
    {11, 10, 1}, {11, 12, 1}, {11, 19, 1},
    {12,  3, 1}, {12, 11, 1}, {12, 13, 1},
    {13, 12, 1}, {13, 14, 1}, {13, 20, 1},
    {14,  4, 1}, {14, 13, 1}, {14, 15, 1},
    {15,  6, 1}, {15, 14, 1}, {15, 16, 1},
    {16, 15, 1}, {16, 17, 1}, {16, 20, 1}, ///< Inner Layer
    {17,  7, 1}, {17, 16, 1}, {17, 18, 1},
    {18,  9, 1}, {18, 17, 1}, {18, 19, 1},
    {19, 11, 1}, {19, 18, 1}, {19, 20, 1},
    {20, 13, 1}, {20, 16, 1}, {20, 19, 1}
};
static const unsigned int s_planar_dodecahedron_vertex_count = 20;

template <typename _Tp>
void selection_sort(_Tp* arr) 
{
    for (int i = 0; i < ARR_BOUND-1; i++)
        for (int j = i+1; j < ARR_BOUND; j++)
            if (arr[j] < arr[i])
                std::swap(arr[i],arr[j]);
}

/// A class to represent a form directed graph
class graph 
{
    protected:
        std::vector<std::vector<Pair>>  __adj_;    // Adjacency list
        std::vector<Edge>               __edges_;  // Vertices edges
        std::size_t                     __size_;   // Number of vertices (nodes)
    public:
        graph()
        {
            __adj_.clear();
            __edges_.clear();
            __size_ = 0;
        }
        graph(std::vector<Edge> const &edges, const unsigned int n)
            : __edges_{ edges }
            , __size_(n)
        {        
            __adj_.clear();

            /// Resize vector to hold `n` elements of type vector<Edge>
            __adj_.resize(n * 3);
            
            /// Iterate through edges
            for (auto &edge : edges)
            {
                /// Get current edge data to insert
                int src = edge.src;
                int dest = edge.dest;
                int weight = edge.weight;
                
                __adj_[src].push_back( std::make_pair(dest, weight) );
            }
        }
        virtual ~graph() = default;

        std::vector<Edge> edges() const noexcept 
        { 
            return __edges_; 
        }
        std::size_t size() const noexcept 
        { 
            return __size_; 
        }
};
