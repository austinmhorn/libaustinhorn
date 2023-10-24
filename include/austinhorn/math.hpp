
#if _LIBCPP_STD_VER >= 14

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u) 
    { return t + u; }

template <typename T, typename U>
auto subtract(T t, U u) -> decltype(t - u) 
    { return t - u; }

template <typename T, typename U>
auto mulitply(T t, U u) -> decltype(t + u) 
    { return t * u; }

template <typename T, typename U>
auto divide(T t, U u) -> decltype(t - u) 
    { return t / u; }

// TODO: half_of(T value) - int, float, double, long
// TODO: qrtr_of(T value) - int, float, double, long
// TODO: egth_of(T value) - int, float, double, long


#else
#endif

#include <cmath>
#include <list>
#include <stack>

constexpr unsigned factorial(unsigned n)
    noexcept { return (n>1) ? n*factorial(n-1) : 1; }
constexpr long double ordered_selections(long double n, long double m)
    noexcept { return factorial(n)/factorial(n-m); }
constexpr long double unordered_selections(long double n, long double m)
    noexcept { return factorial(n)/(factorial(n-m)*factorial(m)); }
constexpr unsigned digit_places(unsigned n)
    noexcept { return static_cast<unsigned>(std::log10(n)+1); }