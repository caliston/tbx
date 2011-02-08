
#include "%APPNAME%Wnd.h"
#include "tbx/window.h"

using namespace tbx;

/**
 * Set up listeners and commands for the window.
 */
void %APPNAME%Wnd::auto_created(std::string template_name, Object object)
{
    Window window(object);
    //TODO: Possible things to do in this routine:-
    //TODO: Use window.gadget() to modify and add listeners to gadgets
    //TODO: Add listener to the window
    //TODO: window.add_command() can be used to add command handlers
}
