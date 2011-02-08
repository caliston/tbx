
#ifndef _%APPNAME%WND_H_
#define _%APPNAME%WND_H_

#include "tbx/window.h"

/**
 * Class to handle the main window.
 */
class %APPNAME%Wnd
{
     // The toolbox window this is handling
     tbx::Window _window;
public:
     %APPNAME%Wnd();
     ~%APPNAME%Wnd();

     bool load_file(const std::string &file_name, int estimated_size);
     void show();
};

#endif
