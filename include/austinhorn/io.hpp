#ifndef io_hpp
#define io_hpp

#include <austinhorn/manip.hpp>

#include <iostream>       // std::cout
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iomanip>          // std::setw, std::setfill
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define RESPONSE_YES 'Y'
#define RESPONSE_NO  'N'

#define BOLD_GREEN "\033[32;1m"
#define BOLD_RED "\033[31;1m"
#define RESET "\033[0m"

void __before_main (void) __attribute__((constructor));

void __before_main (void)
{
    printf ("hello before main()\n");
}

/// @brief Fetch a string of empty spaces
/// @param n # of spaces to inject 
/// @return String comprised spaces with a length of n
std::string spacing(unsigned n) noexcept 
{ 
    return std::string(n, ' '); 
}

/// @brief 
/// @param ch 
/// @param n 
/// @return 
std::string repeat(char ch, unsigned n) noexcept 
{ 
    return std::string(n, ch); 
}
std::string repeat(const std::string& str, unsigned n) noexcept 
{ 
    std::string s;
    for (int i = 0; i < n; i++)
        s += str;
    return s;
}

/// @brief 
/// @param n 
void printnl(unsigned n) noexcept
{ 
    for (int i = 0; i < n; i++) 
        std::cout << std::endl;
}

// Character-By-Character Print function
static const void cbcprint(const std::string& str, std::chrono::milliseconds delay = std::chrono::milliseconds(100)) noexcept
{    
    for (int i = 0; i < str.size(); i++)
    {
        std::cout << str.at(i);
        std::this_thread::sleep_for (std::chrono::milliseconds(delay));
        if ( i )
        {
            if ( str.at(i - 1) == ','  ||  str.at(i - 1) == ':'  ||  str.at(i - 1) == ';'  )
                std::this_thread::sleep_for (std::chrono::milliseconds(200));
            else if ( str.at(i - 1) == '.'  ||  str.at(i - 1) == '!'  ||  str.at(i - 1) == '?' )
                std::this_thread::sleep_for (std::chrono::milliseconds(300));
        }
        std::cout.flush();
    }
    std::cout << std::endl;
}

static const void waitForEnter() noexcept
{
    std::cout.flush() << "\t*** Press \033[1mENTER\033[0m to continue ***";
    std::cin.ignore();
    std::cout.flush();
}

static const unsigned int waitForKey(bool printMessage = true)
{
    if ( printMessage )
    {
        std::cout.flush() << std::endl;
        std::cout << spacing(4) << '*' << std::setw(16) << std::setfill('-') << '*' << std::endl;
        std::cout << spacing(4) << "| PROGRAM PAUSE |" << std::endl;
        std::cout << spacing(4) << '|' << std::setw(16) << std::setfill('-') << '|' << std::endl;
        std::cout << spacing(4) << "| Press Any Key |" << std::endl;
        std::cout << spacing(4) << '*' << std::setw(16) << std::setfill('-') << '*' << std::endl;
        std::cout.flush() << std::endl;
    }

    termios oldSettings, newSettings;

    tcgetattr( fileno( stdin ), &oldSettings );
    newSettings = oldSettings;
    newSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr( fileno( stdin ), TCSANOW, &newSettings );    
    char c = '\0';

    try
    {
        while ( c == '\0' )
        {
            fd_set set;
            struct timeval tv;
            std::chrono::seconds timeout_dur = std::chrono::seconds(1800); // 30 minutes

            tv.tv_sec = timeout_dur.count(); 
            tv.tv_usec = 0;

            FD_ZERO( &set );
            FD_SET( fileno( stdin ), &set );

            int retval = select( fileno( stdin )+1, &set, NULL, NULL, &tv );

            if( retval > 0 )
                read( fileno( stdin ), &c, 1 );
            else if( retval < 0 )
                throw std::runtime_error("select() error");
            else
                throw std::runtime_error("select() timeout -- request expired: reason: (" + std::to_string(timeout_dur.count()) + " seconds elapsed)");
        }
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }

    tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );

    return stoi(std::to_string(c));
}



char ask_polar(const std::string& question = "") noexcept
{
    if ( question.size() )
        std::cout << '[' << BOLD_GREEN << 'Y' << RESET << '/' << BOLD_RED << 'N' << RESET << "] : " << question << std::endl;
    std::string str = "";
    std::getline(std::cin, str);
    if ( str.size() == 1  &&  std::tolower(str.at(0)) == 'y' )
        return RESPONSE_YES;
    else if ( str.size() == 3  &&  str_tolower(str.substr(0, 3)) == "yes" )
        return RESPONSE_YES;
    else if ( str.size() == 2  &&  str_tolower(str.substr(0, 2)) == "no" )
        return RESPONSE_NO;
    return RESPONSE_NO;
}



#endif /* io_hpp */
