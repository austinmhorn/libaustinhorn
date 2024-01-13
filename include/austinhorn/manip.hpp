#ifndef manip_hpp
#define manip_hpp

#include <cstdlib>
#include <string>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <algorithm>


std::string str_toupper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::toupper(c); } );
    return s;
}

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return std::tolower(c); } );
    return s;
}





#endif /* manip_hpp */