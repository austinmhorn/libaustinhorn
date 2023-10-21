#include <austinhorn/tree.hpp>

#include <iostream>

int main (int argc, const char **argv)
{
    tree<int> intTree;
    
    for (int i = 0; i < 1000; i++)
        intTree.insert(i);

    intTree.showTreeStats();

    intTree.balance();
    intTree.inorder();

    intTree.showTreeStats();

    return EXIT_SUCCESS;
}