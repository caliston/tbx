
#ifndef _%APPNAME%WND_H_
#define _%APPNAME%WND_H_

#include "tbx/autocreatelistener.h"

/**
 * Class to set up listeners an variables when the single
 * window is shown.
 */
class %APPNAME%Wnd : public tbx::AutoCreateListener
{
public:
    virtual void auto_created(std::string template_name, tbx::Object object);
};

#endif
