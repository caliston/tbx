
#include "%APPNAME%Wnd.h"

// Reposition each new window class
tbx::doc::DocPositionWindow %APPNAME%Wnd::_start_pos;

/**
 * Initial creation of the window for the document
 */
%APPNAME%Wnd::%APPNAME%Wnd(%APPNAME%Doc *doc) : DocWindow(doc, "DocWindow")
{
     //TODO: Add code to setup/display window for the given document
     //which will either be a new document or a loaded document
     //Listeners for the window or its gadgets can also be added
     //to allow the document to be changed.
	_start_pos.show(_window);
}

%APPNAME%Wnd::~%APPNAME%Wnd()
{
}

