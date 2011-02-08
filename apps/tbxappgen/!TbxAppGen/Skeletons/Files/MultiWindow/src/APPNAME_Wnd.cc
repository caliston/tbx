
#include "%APPNAME%Wnd.h"
#include "tbx/window.h"
#include "tbx/deleteonhidden.h"

/**
 * Construct the object and create the toolbox window
 */
%APPNAME%Wnd::%APPNAME%Wnd() : _window("Window")
{
    //TODO: Possible things to do in this routine:-
    //TODO: Use window.gadget() to modify and add listeners to gadgets
    //TODO: Add listener to the window
    //TODO: window.add_command() can be used to add command handlers

    // Handler to delete this object when the toolbox window has been hidden
    _window.add_has_been_hidden_listener(
              new tbx::DeleteClassOnHidden<%APPNAME%Wnd>(this)
            );
}

/**
 * Destructor - deletes toolbox object handled by this class
 */
%APPNAME%Wnd::~%APPNAME%Wnd()
{
   _window.delete_object();
}
