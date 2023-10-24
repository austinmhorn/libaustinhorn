#include <austinhorn/stack.hpp>

#include <iostream>

int main (int argc, const char **argv)
{
    stack<int> intStack;

    for (int i = 0 ; i < 5; i++)
        intStack.push(i);
    intStack.print();
    std::cout << std::endl;

    intStack.pop();
    intStack.pop();

    for (auto iter = intStack.begin(); iter != intStack.end(); iter++ )
    {
        std::cout << *iter << std::endl;
    }

    return EXIT_SUCCESS;
}