#ifndef io_hpp
#define io_hpp

#include <austinhorn/manip.hpp>

#include <iostream>       // std::cout
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <fstream>        // open, select
#include <cstdlib>        // atoi
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define RESPONSE_YES 'Y'
#define RESPONSE_NO  'N'

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

static const unsigned int waitForPrintable()
{
    struct termios oldSettings, newSettings;

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

static const char getPolarResponse() noexcept
{
    std::cout.flush() << "\t***Respond Y/N ***\n:";
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

static const bool isResponseYes(const char response) noexcept
{
    return response == RESPONSE_YES;
}

static const bool isResponseNo(const char response) noexcept
{
    return response == RESPONSE_NO;
}




#endif /* io_hpp */