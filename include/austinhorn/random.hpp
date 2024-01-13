#ifndef random_ranged_hpp
#define random_ranged_hpp

#include <random>
#include <limits.h>
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed unsigned integer value across a range
/// @param min Minimum generated unsigned integer value possible
/// @param max Maxmimum generated unsigned integer value possible
/// @returns Random unsigned integer from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
unsigned int random_uint(unsigned int min = 0, unsigned int max = UINT_MAX) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<unsigned int> distrib(min, max);
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed signed integer value across a range
/// @param min Minimum generated signed integer value possible
/// @param max Maxmimum generated signed integer value possible
/// @returns Random signed integer from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
signed int random_int(signed int min = INT_MIN, signed int max = INT_MAX) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<signed int> distrib(min, max);
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed unsigned char value across a range
/// @param min Minimum generated unsigned char value possible
/// @param max Maxmimum generated unsigned char value possible
/// @returns Random unsigned char from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
unsigned char random_uchar(unsigned char min = 0, unsigned char max = UCHAR_MAX) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<unsigned> distrib( (int(min)), (int(max)) );
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed signed char value across a range
/// @param min Minimum generated signed char value possible
/// @param max Maxmimum generated signed char value possible
/// @returns Random signed char from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
signed char random_char(signed char min = SCHAR_MIN, signed char max = SCHAR_MAX) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<signed> distrib( (int(min)), (int(max)) );
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed uppercase character
/// @param min Minimum generated uppercase character value possible
/// @param max Maxmimum generated uppercase character value possible
/// @returns Random uppercase character from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
unsigned char random_upper(unsigned char min = 65, unsigned char max = 90) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<unsigned> distrib( (int(min)), (int(max)) );
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed lowercase character
/// @param min Minimum generated lowercase character value possible
/// @param max Maxmimum generated lowercase character value possible
/// @returns Random lowercase character from min to max inclusively 
//////////////////////////////////////////////////////////////////////////////////////////
unsigned char random_lower(unsigned char min = 97, unsigned char max = 122) noexcept {
    std::mt19937 gen32(std::random_device{}());
    std::uniform_int_distribution<unsigned> distrib( (int(min)), (int(max)) );
    return distrib(gen32);
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a randomly distributed alpha character, upper and lower
/// @returns Random upper or lower character
//////////////////////////////////////////////////////////////////////////////////////////
unsigned char random_alpha() noexcept {
    return (random_uint(0, 1)) ? random_upper() : random_lower();
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a random string consisting only of uppercase character
/// @param length Length of string to generate
/// @param unique When true, function returns a string of unique uppercase characters
/// @returns Random string consisting only of uppercase letters
//////////////////////////////////////////////////////////////////////////////////////////
std::string random_string_upper(unsigned int length, bool unique = false) noexcept {
    std::string str = "";
    for (int i = 0; i < length; i++)
    {
        char c = random_upper();
        while ( unique  &&  str.find(c) != std::string::npos )
            c = random_upper();
        str += c;
    }
    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a random string consisting only of lowercase character
/// @param length Length of string to generate
/// @param unique When true, function returns a string of unique lowercase characters
/// @returns Random string consisting only of lowercase letters
//////////////////////////////////////////////////////////////////////////////////////////
std::string random_string_lower(unsigned int length, bool unique = false) noexcept {
    std::string str = "";
    for (int i = 0; i < length; i++)
    {
        char c = random_lower();
        while ( unique  &&  str.find(c) != std::string::npos )
            c = random_lower();
        str += c;
    }
    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a random alphanumeric string with uppercase characters
/// @param length Length of alphanumeric string to generate
/// @returns Randomly generated uppercase alphanumeric string
//////////////////////////////////////////////////////////////////////////////////////////
std::string random_alphanumeric_upper(unsigned int length) noexcept {
    std::string str = "";
    for (int i = 0; i < length; i++)
        (random_uint(0, 1)) ? str += random_upper() : str += std::to_string(random_uint(0, 9));
    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Generates a random alphanumeric string with lowercase characters
/// @param length Length of alphanumeric string to generate
/// @returns Randomly generated lowercase alphanumeric string
//////////////////////////////////////////////////////////////////////////////////////////
std::string random_alphanumeric_lower(unsigned int length) noexcept {
    std::string str = "";
    for (int i = 0; i < length; i++)
        ( random_uint(0, 1) ) ? str += random_lower() : str += std::to_string(random_uint(0, 9));
    return str;
}

#endif /* random_ranged_hpp */
