#include <austinhorn/priority_queue.hpp>
#include <austinhorn/random.hpp>

#include <iostream>

int main (int argc, const char **argv)
{
    priority_queue<int, std::less<>> lessIntPQ;

    for (int i = 0; i < 10; i++)
        lessIntPQ.push(random_int(-100, 100));

    std::cout << "\npriority_queue<int, std::less<>>" << std::endl;
    lessIntPQ.print();

    priority_queue<int, std::greater<>> greaterIntPQ;

    for (int i = 0; i < 10; i++)
        greaterIntPQ.push(random_int(-100, 100));

    std::cout << "\npriority_queue<int, std::greater<>>" << std::endl;
    greaterIntPQ.print();

    priority_queue<char, std::less<>> lessCharPQ;

    for (int i = 0; i < 10; i++)
        lessCharPQ.push( random_upper() );

    std::cout << "\npriority_queue<char, std::less<>>" << std::endl;
    lessCharPQ.print();

    priority_queue<char, std::greater<>> greaterCharPQ;
    std::string str = random_string_upper(10, true);

    for (int i = 0; i < str.size(); i++)
        greaterCharPQ.push( str.at(i) );

    std::cout << "\npriority_queue<char, std::greater<>>" << std::endl;
    greaterCharPQ.print();

    std::cout << "\npopping top element  \"" << greaterCharPQ.top() << "\"" << std::endl;
    greaterCharPQ.pop();
    greaterCharPQ.print();
    std::cout << "popping top element  \"" << greaterCharPQ.top() << "\"" << std::endl;
    greaterCharPQ.pop();
    greaterCharPQ.print();

    return EXIT_SUCCESS;
}