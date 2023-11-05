#include <austinhorn/io.hpp>

int main (int argc, const char **argv)
{
    waitForEnter();

    //cbcprint("Do you like the austinhorn library? So do I! It has so many useful capabilites. \nLike this one, cbcprint(), that prints a string character-by-character, with a delay given in milliseconds between. \nDid you notice it also pauses for punctuation? Fancy! ", std::chrono::milliseconds(50));

    
    std::cout << "\nDo you like pizza?" << std::endl;
    char response = getPolarResponse();
    if ( response  ==  'Y' )
        std::cout << "Yay! You like pizza!" << std::endl;
    else if ( response  ==  'N' )
        std::cout << "Why don't you like pizza?" << std::endl;
    
    std::cout << "\nIs C++ your favorite language?:" << std::endl;
    if ( isResponseYes(getPolarResponse()) )
        std::cout << "Mine is as well!" << std::endl;
    else
        std::cout << "No? You''ll grow to love it!" << std::endl;

    std::cout << "\nPaused - press any printable key" << std::endl;
    int key = waitForPrintable();
    std::cout << "printable key: " << std::endl;
    std::cout << "\tDEC: " << key << std::endl;

    return EXIT_SUCCESS;
}