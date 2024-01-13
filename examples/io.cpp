#include <austinhorn/io.hpp>

#include <cctype>

#define TAB_KEY         9
#define ENTER_KEY      10
#define ESCAPE_KEY     27
#define SPACE_KEY      32
#define BACKSPACE_KEY 127

enum class Key : int
{
    Unknown = -1, //!< Unhandled key
    A       = 0,  //!< The A key
    B,            //!< The B key
    C,            //!< The C key
    D,            //!< The D key
    E,            //!< The E key
    F,            //!< The F key
    G,            //!< The G key
    H,            //!< The H key
    I,            //!< The I key
    J,            //!< The J key
    K,            //!< The K key
    L,            //!< The L key
    M,            //!< The M key
    N,            //!< The N key
    O,            //!< The O key
    P,            //!< The P key
    Q,            //!< The Q key
    R,            //!< The R key
    S,            //!< The S key
    T,            //!< The T key
    U,            //!< The U key
    V,            //!< The V key
    W,            //!< The W key
    X,            //!< The X key
    Y,            //!< The Y key
    Z,            //!< The Z key
    Num0,         //!< The 0 key
    Num1,         //!< The 1 key
    Num2,         //!< The 2 key
    Num3,         //!< The 3 key
    Num4,         //!< The 4 key
    Num5,         //!< The 5 key
    Num6,         //!< The 6 key
    Num7,         //!< The 7 key
    Num8,         //!< The 8 key
    Num9,         //!< The 9 key
    Escape,       //!< The Escape key
    LControl,     //!< The left Control key
    LShift,       //!< The left Shift key
    LAlt,         //!< The left Alt key
    LSystem,      //!< The left OS specific key: window (Windows and Linux), apple (macOS), ...
    RControl,     //!< The right Control key
    RShift,       //!< The right Shift key
    RAlt,         //!< The right Alt key
    RSystem,      //!< The right OS specific key: window (Windows and Linux), apple (macOS), ...
    Menu,         //!< The Menu key
    LBracket,     //!< The [ key
    RBracket,     //!< The ] key
    Semicolon,    //!< The ; key
    Comma,        //!< The , key
    Period,       //!< The . key
    Apostrophe,   //!< The ' key
    Slash,        //!< The / key
    Backslash,    //!< The \ key
    Grave,        //!< The ` key
    Equal,        //!< The = key
    Hyphen,       //!< The - key (hyphen)
    Space,        //!< The Space key
    Enter,        //!< The Enter/Return keys
    Backspace,    //!< The Backspace key
    Tab,          //!< The Tabulation key
    PageUp,       //!< The Page up key
    PageDown,     //!< The Page down key
    End,          //!< The End key
    Home,         //!< The Home key
    Insert,       //!< The Insert key
    Delete,       //!< The Delete key
    Add,          //!< The + key
    Subtract,     //!< The - key (minus, usually from numpad)
    Multiply,     //!< The * key
    Divide,       //!< The / key
    Left,         //!< Left arrow
    Right,        //!< Right arrow
    Up,           //!< Up arrow
    Down,         //!< Down arrow
    Numpad0,      //!< The numpad 0 key
    Numpad1,      //!< The numpad 1 key
    Numpad2,      //!< The numpad 2 key
    Numpad3,      //!< The numpad 3 key
    Numpad4,      //!< The numpad 4 key
    Numpad5,      //!< The numpad 5 key
    Numpad6,      //!< The numpad 6 key
    Numpad7,      //!< The numpad 7 key
    Numpad8,      //!< The numpad 8 key
    Numpad9,      //!< The numpad 9 key
    F1,           //!< The F1 key
    F2,           //!< The F2 key
    F3,           //!< The F3 key
    F4,           //!< The F4 key
    F5,           //!< The F5 key
    F6,           //!< The F6 key
    F7,           //!< The F7 key
    F8,           //!< The F8 key
    F9,           //!< The F9 key
    F10,          //!< The F10 key
    F11,          //!< The F11 key
    F12,          //!< The F12 key
    F13,          //!< The F13 key
    F14,          //!< The F14 key
    F15,          //!< The F15 key
    Pause,        //!< The Pause key

    KeyCount, //!< Keep last -- the total number of keyboard keys
};

#include <austinhorn/primitive_map.hpp>
#include <map>

static std::map<unsigned, Key>      __BASE_KEY_MAP_;
static std::map<Key, const char *>  __CHAR_KEY_MAP_;

static void __init_base_key_map()
{
    init_primitive_map(__BASE_KEY_MAP_)
        (TAB_KEY, Key::Tab)
        (ENTER_KEY, Key::Enter)
        (ESCAPE_KEY, Key::Escape)
        (SPACE_KEY, Key::Space)
        (BACKSPACE_KEY, Key::Backspace)
    ;
}
static void __init_char_key_map()
{
    init_primitive_map(__CHAR_KEY_MAP_)
        (Key::Tab, "Tab")
        (Key::Enter, "Enter")
        (Key::Escape, "Escape")
        (Key::Space, "Space")
        (Key::Backspace, "Backspace")
    ;
}

int main (int argc, const char **argv)
{
    //waitForEnter();
    //cbcprint("Do you like the austinhorn library? So do I! It has so many useful capabilites. \nLike this one, cbcprint(), that prints a string character-by-character, with a delay given in milliseconds between. \nDid you notice it also pauses for punctuation? Fancy! ", std::chrono::milliseconds(50));

    //(ask_polar("Do you like pizza?")== 'Y') 
    //    ? std::cout << "I like pizza too!" << std::endl 
    //    : std::cout << "Why don't you like pizza? :(" << std::endl;

    __init_base_key_map();
    __init_char_key_map();

    try {
        // Prompt for key press
        auto key = waitForKey();

        if ( key == -1 )
            throw std::runtime_error("Bad key input");

        std::cout << "| int:  " << std::setw(16) << std::setfill(' ') << int(key) << " |" << std::endl;
        if ( std::isprint( key ) )
            if ( int(key)  ==  SPACE_KEY )
                std::cout << "| char: " << std::setw(16) << std::setfill(' ') << __CHAR_KEY_MAP_[__BASE_KEY_MAP_[int(key)]] << " |" << std::endl;
            else 
                std::cout << "| char: " << std::setw(16) << std::setfill(' ') << char(key) << " |" << std::endl;
        else
        {
            std::cout << "| key:  " << std::setw(16) << std::setfill(' ') << __CHAR_KEY_MAP_[__BASE_KEY_MAP_[int(key)]] << " |" << std::endl;
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
