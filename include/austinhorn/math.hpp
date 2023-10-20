
#if _LIBCPP_STD_VER >= 14

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u) 
    { return t + u; }

template <typename T, typename U>
auto subtract(T t, U u) -> decltype(t - u) 
    { return t - u; }
#else
#endif 