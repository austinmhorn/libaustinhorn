#include <austinhorn/forward_list.hpp>

int main (int argc, const char **argv)
{
    forward_list<int> intList;

    for (int i = 0; i < 10; i++)
        intList.push_front(i);

    // capable of range based looping 
    for ( const int& i : intList )
        std::cout << i << " ";
    std::cout << std::endl;

    // find() returns an iterator
    auto iter = intList.find(4);
    iter = intList.erase(iter);
    // iterator value still accessible
    std::cout << "erase value: " << *iter << std::endl;

    for ( auto i : intList )
        std::cout << i << " ";
    std::cout << std::endl;

    // iterator type is public
    forward_list<int>::iterator first = intList.find(7);
    forward_list<int>::iterator last = intList.find(3);

    // iterators contain garbage values after calling erase(), so they should only be accessed prior
    std::cout << "erase range: " << *first << " - " << *last << std::endl;
    intList.erase(first, last);

    
    for ( auto i : intList )
        std::cout << i << " ";
    std::cout << std::endl;

    for (int i = 1; i <= 3; i++)
        intList.push_front(i);

    intList.print();
    


    return EXIT_SUCCESS;
}