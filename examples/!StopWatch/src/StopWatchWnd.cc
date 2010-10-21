
#include "StopWatchWnd.h"
#include "tbx/window.h"

using namespace tbx;

/**
 * Set up listeners and commands for the window.
 */
void StopWatchWnd::auto_created(std::string template_name, Object object)
{
    Window window(object);
    _display = window.gadget(0);
    _button = window.gadget(1);
    //TODO: Possible things to do in this routine:-
    //TODO: Use window.gadget() to modify and add listeners to gadgets
    //TODO: Add listener to the window
    //TODO: window.add_command() can be used to add command handlers
}
