#include <austinhorn/list.hpp>

int main (int argc, const char **argv)
{
    list<int> intList;

    for (int i = 0; i < 25; i++)
        intList.push_front(i);

    intList.print();
    intList.clear();

    for (int i = 0; i < 25; i++)
        intList.push_back(i);

    intList.print();

    std::cout << *intList.begin() << std::endl;

    return EXIT_SUCCESS;
}