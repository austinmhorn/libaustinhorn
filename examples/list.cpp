#include <austinhorn/list.hpp>

int main (int argc, const char **argv)
{
    list<int> intList;

    for (int i = 0; i < 100; i++)
        intList.push_back(i);

    intList.print();

    

    return EXIT_SUCCESS;
}