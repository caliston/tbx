
#ifndef _StopWatchWND_H_
#define _StopWatchWND_H_

#include "tbx/autocreatelistener.h"

/**
 * Class to set up listeners an variables when the single
 * window is shown.
 */
class StopWatchWnd : public tbx::AutoCreateListener
{
public:
    virtual void auto_created(std::string template_name, tbx::Object object);
};

#endif
