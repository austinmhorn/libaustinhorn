#ifndef range_hpp
#define range_hpp

#include <austinhorn/iterator.hpp>
#ifndef iterator_hpp
#error "Missing header files"
#endif
#include <utility>

template <long FROM, long TO>
struct range
    : public range_iterator<FROM, TO> 
{
    
    typename range_iterator<FROM, TO>::reference front() noexcept {return range_iterator<FROM, TO>(FROM);}
    typename range_iterator<FROM, TO>::reference back() noexcept {return TO;}

    range_iterator<FROM, TO> begin() {return range_iterator<FROM, TO>(FROM);}
    range_iterator<FROM, TO> end() {return range_iterator<FROM, TO>(TO >= FROM? TO+1 : TO-1);}
};

#endif /* range_hpp */
