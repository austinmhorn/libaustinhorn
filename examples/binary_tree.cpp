#include <austinhorn/binary_tree.hpp>

#include <iostream>

int main (int argc, const char **argv)
{
    binary_tree<int> intTree;
    
    for (int i = 0; i < 10; i++)
        intTree.insert(i);

    intTree.showTreeStats();

    intTree.balance();
    intTree.postorder();

    intTree.showTreeStats();

    return EXIT_SUCCESS;
}