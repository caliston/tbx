
#include "tbx/application.h"
#include "tbx/autocreatelistener.h"
#include "%APPNAME%wnd.h"

using namespace std;

int main()
{
   tbx::Application my_app("<%APPNAME%$Dir>");

   %APPNAME%Wnd window_handler;

   my_app.set_autocreate_listener("Window", &window_handler);

   my_app.run();

   return 0;
}
