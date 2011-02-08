#ifndef ICONBARHANDLER_H
#define ICONBARHANDLER_H

#include "tbx/iconbar.h"
#include "tbx/autocreatelistener.h"
#include "tbx/application.h"
#include "tbx/loader.h"
#include "tbx/reporterror.h"
#include <stdexcept>
#include "%APPNAME%Wnd.h"


/**
 * Simple class to setup iconbar handlers
 */
class IconbarHandler
   : public tbx::AutoCreateListener,
   public tbx::Loader

{
public:
    /**
     * This is called when the iconbar icon is created
     */
    virtual void auto_created(std::string template_name, tbx::Object object)
    {
        tbx::Iconbar iconbar(object);

        // Set this class up to handle files dropped on the icon bar
        // loads %FILEDESC% files
        iconbar.add_loader(this, 0x%FILETYPE%);

        // Iconbar is only created once so we no longer need to listen
        // for it to be created
        tbx::app()->clear_autocreate_listener(template_name);
    }

    /**
     * Listener for file dropped on icon bar
     *
     * Creates class to handle window and load file,
     * gets class to load file and then shows window
     * if load is successful.
     */
    virtual bool load_file(tbx::LoadEvent &event)
    {
        bool loadOK = false;
        %APPNAME%Wnd *wnd = 0;

        try
        {
           // Create the object that handles each window
           wnd = new %APPNAME%Wnd();
           loadOK = wnd->load_file(event.file_name(), event.estimated_size());
        } catch (std::exception &e)
        {
           std::string msg="Unable to load file ";
           msg += e.what();
           tbx::report_error(msg);
        }
        if (loadOK)
        {
           wnd->show();
        } else
        {
           delete wnd;
        }


         return loadOK;
     }
};

#endif
