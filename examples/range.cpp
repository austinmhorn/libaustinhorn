#include <austinhorn/range.hpp>

#include <iostream>

int main (int argc, const char **argv)
{
    range<0, 100> myRange;

    for (auto n : myRange)
        std::cout << n << std::endl;

    return EXIT_SUCCESS;
}