#ifndef system_hpp
#define system_hpp

#if WIN32 || WIN64
#include <windows.h>
#elif LINUX || linux || __linux__
#include <X11/Xlib.h>
#elif APPLE || MACH
#include <CoreGraphics/CGDisplayConfiguration.h>
#else
#endif

static void getScreenResolution(unsigned int& width, unsigned int& height) 
{
#if WIN32 || WIN64
    width = (int)GetSystemMetrics(SM_CXSCREEN);
    height = (int)GetSystemMetrics(SM_CYSCREEN);
#elif LINUX || linux || __linux__
    Display* d = XOpenDisplay(NULL);
    Screen*  s = DefaultScreenOfDisplay(d);
    width = s->width;
    height = s->height;
#elif APPLE || MACH
    auto mainDisplayId = CGMainDisplayID();
    width = static_cast<unsigned int>(CGDisplayPixelsWide(mainDisplayId));
    height = static_cast<unsigned int>(CGDisplayPixelsHigh(mainDisplayId));
#else
#endif
}

#endif /* system_hpp */
