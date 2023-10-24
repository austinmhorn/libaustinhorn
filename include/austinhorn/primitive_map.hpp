#ifndef primitive_map_hpp
#define primitive_map_hpp

/// Custom map for primitive data types
template <typename _Tp>
struct primitive_map 
{
        primitive_map(_Tp& data) 
            : __data_(data) {}
        // Overloaded function call operator for pairing process
        primitive_map& operator() (typename _Tp::key_type const& key, typename _Tp::mapped_type const& value) 
            { __data_[key] = value; return *this; }
    private:
        _Tp& __data_;
};

/// Template function for initialzing primitive type map with value(s)
template <typename _Tp>
struct primitive_map<_Tp> init_primitive_map(_Tp& item) 
{
    return primitive_map<_Tp>(item);
}

#endif /* primitive_map_hpp */