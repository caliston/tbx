#ifndef ICONBARHANDLER_H
#define ICONBARHANDLER_H

#include "tbx/iconbar.h"
#include "tbx/autocreatelistener.h"
#include "tbx/application.h"
#include "tbx/command.h"
#include "%APPNAME%Wnd.h"

/**
 * Simple class to setup iconbar handlers
 */
class IconbarHandler
   : public tbx::AutoCreateListener,
   public tbx::Command

{
public:
    /**
     * This is called when the iconbar icon is created
     */
    virtual void auto_created(std::string template_name, tbx::Object object)
    {
        tbx::Iconbar iconbar(object);
        // Add this as a command that will create the window
        iconbar.add_select_command(this);

        // Iconbar is only created once so we no longer need to listen
        // for it to be created
        tbx::app()->clear_autocreate_listener(template_name);
    }

    /**
     * Command handler for select command that creates a new window
     */
    virtual void execute()
    {
        // Create the object that handles each window
        new %APPNAME%Wnd();
    }
};

#endif
