#ifndef ICONBARHANDLER_H
#define ICONBARHANDLER_H

#include "tbx/iconbar.h"
#include "tbx/autocreatelistener.h"
#include "tbx/application.h"
#include "tbx/loader.h"

class SaverTestWnd;

/**
 * Simple class to setup iconbar handlers
 */
class IconbarHandler
   : public tbx::AutoCreateListener,
   public tbx::Loader

{
    char *_load_buffer;
    SaverTestWnd *_load_wnd;

public:
	IconbarHandler();
	virtual ~IconbarHandler();

	// auto create methods
	virtual void auto_created(std::string template_name, tbx::Object object);

	 // Loader methods
	 virtual bool accept_file(tbx::LoadEvent &event);
     virtual bool load_file(tbx::LoadEvent &event);
     void *data_buffer(const tbx::LoadEvent &event, int &buffer_size);
     bool data_received(tbx::DataReceivedEvent &event);

};

#endif
