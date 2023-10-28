#include <austinhorn/list.hpp>

int main(int argc, const char **argv)
{
    list<int> intList;

    for (int i = 0; i >= -3; i--)
        intList.push_front(i);

    for (int i = 1; i <= 3; i++)
        intList.push_back(i);

    std::cout << "\n";
    std::cout << "front(): " << intList.front() << std::endl;
    std::cout << "back(): " << intList.back() << std::endl;

    std::cout << "\n";
    std::cout << "forward" << std::endl;
    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "backward" << std::endl;
    for ( auto iter = intList.rbegin(); iter != intList.rend(); ++iter )
        std::cout << *iter << " ";
    std::cout << std::endl;

    std::cout << "clear()" << std::endl;
    intList.clear();

    for (int i = 1; i <= 5; i++)
        intList.push_back( i );

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\n";
    std::cout << "pop_front()" << std::endl;
    intList.pop_front();
    std::cout << "pop_back()" << std::endl;
    intList.pop_back();

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    intList.push_front( 2 );
    intList.push_front( 1 );
    intList.push_back( 4 );
    intList.push_back( 1 );

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "unique()" << std::endl;
    intList.unique();

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "clear()" << std::endl;
    intList.clear();
    
    for (int i = 0; i >= -5; i--)
        intList.push_back(i);

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\n";
    std::cout << "reverse()" << std::endl;
    intList.reverse();

    std::cout << "forward" << std::endl;
    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "backward" << std::endl;
    for ( auto iter = intList.rbegin(); iter != intList.rend(); ++iter )
        std::cout << *iter << " ";
    std::cout << std::endl;

    std::cout << "clear()" << std::endl;
    intList.clear();

    for (int i = 9; i >= 0; i--)
        intList.push_back(i);

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\n";
    std::cout << "sort<std::less<>>() " << std::endl;
    intList.sort<std::less<>>();
    
    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\n";
    std::cout << "sort<std::greater<>>() " << std::endl;
    intList.sort<std::greater<>>();
    
    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;
    
    std::cout << "clear()" << std::endl;
    intList.clear();

    for ( int i = 0; i < 3; i++ )
        intList.push_back(i);

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "insert 0 after 0" << std::endl;
    intList.insert(intList.find(0), 0);
    std::cout << "insert 1 after 1" << std::endl;
    intList.insert(intList.find(1), 1);
    std::cout << "insert 2 after 2" << std::endl;
    intList.insert(intList.find(2), 2);
    
    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "\n";
    std::cout << "unique() " << std::endl;
    intList.unique();

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    for ( int i = 1; i < 4; i++ )
        intList.push_front(i);

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    intList.erase(intList.find_last_of(1));
    std::cout << "erased last of value 1" << std::endl;
    intList.erase(intList.find_last_of(2));
    std::cout << "erased last of value 2" << std::endl;

    for (auto i : intList)
        std::cout << i << " ";
    std::cout << std::endl;

    for ( int i = 1; i < 7; i++ )
        intList.push_back(i % 3);
    intList.push_back(1);
    intList.push_back(3);

    intList.print_fwd();

    intList.erase(intList.find(1, 2));
    std::cout << "erased 2nd occurence of value 1" << std::endl;
    intList.print_fwd();

    std::cout << "erase()" << std::endl;
    intList.erase(intList.find(0, 3));
    std::cout << "erased 3rd occurence of value 0" << std::endl;
    intList.print_fwd();

    std::cout << "erase from 2nd occurence of value 2 -> last of value 2" << std::endl;
    int numErased = intList.erase(intList.find(2, 2), intList.find_last_of(2));
    std::cout << "erased " << numErased << " elements " << std::endl;
    intList.print_fwd();

    std::cout << "sort<std::less<>>()" << std::endl;
    intList.sort();
    intList.print_fwd();
    std::cout << "unique()" << std::endl;
    intList.unique();
    intList.print_fwd();
    intList.print_bwd();

    std::cout << "size(): " << intList.size() << std::endl;
    std::cout << "clear()" << std::endl;
    intList.clear();

    for ( int i = 0; i < 10; i++ )
        intList.push_back(i % 5);
    intList.print_fwd();

    std::cout << "remove_all(1)" << std::endl;
    auto numRemoved = intList.remove_all(1);
    std::cout << numRemoved << " elements equal to 1 were removed " << std::endl;
    intList.print_fwd();

    numRemoved = intList.erase_if(intList.begin(), 
                                   intList.end(),
                                   [](int i) { return i % 2; });
    std::cout << numRemoved << " elements with even values were removed " << std::endl;
    intList.print_fwd();

    std::cout << "clear()" << std::endl;
    intList.clear();

    for ( int i = 0; i < 6; i++ )
        intList.push_back( i % 3 );

    intList.print_fwd();
    std::cout << "remove(1)" << std::endl;
    intList.remove(1);
    intList.print_fwd();
    std::cout << "remove(1)" << std::endl;
    intList.remove(1);
    intList.print_fwd();

    std::cout << "erase_after( begin() )" << std::endl;
    intList.erase_after(intList.begin());
    intList.print_fwd();
    std::cout << "erase_before( end() )" << std::endl;
    intList.erase_before(intList.end());

    intList.print_fwd();
    std::cout << "clear()" << std::endl;
    intList.clear();

    for ( int i = 0; i < 8; i++ )
        intList.push_back( i );

    list<int> secList;

    for (int i = 4; i < 10; i++)
        secList.push_back(i);

    std::cout << "\n";
    std::cout << "intList" << std::endl;
    intList.print_fwd();
    std::cout << "secList" << std::endl;
    secList.print_fwd();

    std::cout << "\n";
    std::cout << "intList.merge<std::greater<>>( secList )" << std::endl;
    intList.merge<std::greater<>>(secList);
    intList.print_fwd();
    std::cout << "\n";
    std::cout << "sort<std::greater<>>" << std::endl;
    intList.sort<std::greater<>>();
    intList.print_fwd();
    std::cout << "\n";
    std::cout << "unique()" << std::endl;
    intList.unique();
    intList.print_fwd();
    std::cout << "clear()" << std::endl;
    intList.clear();

    for ( int i = 0; i < 8; i++ )
        intList.push_back( i );

    std::cout << "\n";
    std::cout << "intList.merge( secList )" << std::endl;
    intList.merge(secList);

    intList.print_fwd();
    

    return EXIT_SUCCESS;
}