
#ifndef _SaverTestWND_H_
#define _SaverTestWND_H_

#include "tbx/window.h"
#include "tbx/textarea.h"
#include "tbx/draggable.h"
#include "tbx/saver.h"

/**
 * Class to handle the main window.
 */
class SaverTestWnd :
	tbx::DragEndedListener,
	tbx::SaverFinishedHandler,
	tbx::SaverSaveCompletedHandler,
	tbx::SaverSaveToFileHandler,
	tbx::SaverFillBufferHandler
{
     // The toolbox window this is handling
     tbx::Window _window;
     tbx::TextArea _text;
     tbx::Draggable _dragger;

     char *_save_buffer;

 	void drag_ended(const tbx::DragEndedEvent &drag_ended_event);

	virtual void saver_finished(const tbx::SaverFinishedEvent &finished);
	virtual void saver_save_completed(tbx::SaverSaveCompletedEvent &event);
	virtual void saver_save_to_file(tbx::Saver saver, std::string file_name);
	virtual void saver_fill_buffer(tbx::Saver saver, int size, void *buffer, int already_transmitted);

public:
     SaverTestWnd();
     virtual ~SaverTestWnd();

      bool load_file(const std::string &file_name, int estimated_size);
      void add_text(const char *text);

     void show();
};

#endif
