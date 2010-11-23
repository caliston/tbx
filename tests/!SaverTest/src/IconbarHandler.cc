
#include "IconbarHandler.h"
#include "tbx/application.h"
#include "tbx/reporterror.h"
#include <stdexcept>
#include "SaverTestWnd.h"
#include "logging.h"

IconbarHandler::IconbarHandler() : _load_buffer(0)
{
}

IconbarHandler::~IconbarHandler()
{
	delete _load_buffer;
}
/**
 * This is called when the iconbar icon is created
 */
void IconbarHandler::auto_created(std::string template_name, tbx::Object object)
{
	tbx::Iconbar iconbar(object);

	// Set this class up to handle files dropped on the icon bar
	// loads %FILEDESC% files
	iconbar.add_loader(this, 0xFFF);

	// Iconbar is only created once so we no longer need to listen
	// for it to be created
	tbx::app()->clear_autocreate_listener(template_name);
}

bool IconbarHandler::accept_file(tbx::LoadEvent &event)
{
	if (logging) log << "accept_file: File name " << event.file_name()
					 << " type " << std::hex << event.file_type() << std::dec
					 << " size " << event.estimated_size()
					 << std::endl;
	return true;
}


/**
 * Listener for file dropped on icon bar
 *
 * Creates class to handle window and load file,
 * gets class to load file and then shows window
 * if load is successful.
 */
bool IconbarHandler::load_file(tbx::LoadEvent &event)
{
	bool loadOK = false;
	SaverTestWnd *wnd = 0;

	try
	{
	   // Create the object that handles each window
	   wnd = new SaverTestWnd();
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

/**
 * create buffer for data
 */
void *IconbarHandler::data_buffer(const tbx::LoadEvent &event, int &buffer_size)
{
   if (logging) log << "data_buffer: buffer size " << buffer_size << std::endl;

   _load_wnd = 0;
   _load_buffer = 0;

	try
	{
	   // Create the object that handles each window
	   buffer_size = 16; // We are always going to have a sixteen byte buffer to text multiple messages
	   _load_buffer = new char[17];
	   _load_wnd = new SaverTestWnd();
	} catch (std::exception &e)
	{
		delete _load_buffer;
		delete _load_wnd;
		return 0;
	}

    return _load_buffer;
}

/**
 * Data received from other app
 */
bool IconbarHandler::data_received(tbx::DataReceivedEvent &event)
{
	if (logging) log << "data_received: more = " << event.more() << ", num bytes = " << event.received() << std::endl;
	_load_buffer[event.received()] = 0;
	if (logging) log << "data_received: text = " << _load_buffer << std::endl;
	if (event.received())
	{
		_load_wnd->add_text(_load_buffer);
	}
	if (!event.more())
	{
		delete _load_buffer;
		_load_buffer = 0;
		_load_wnd->show();
	}

	return true;
}

