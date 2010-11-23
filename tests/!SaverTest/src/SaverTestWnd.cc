
#include "SaverTestWnd.h"
#include "logging.h"

#include "tbx/window.h"
#include "tbx/deleteonhidden.h"
#include "tbx/saver.h"
#include <fstream>
#include <iostream>

/**
 * Construct the object and create the toolbox window
 */
SaverTestWnd::SaverTestWnd() : _window("Window")
{
	_text = _window.gadget(0);
	_dragger = _window.gadget(1);
	_dragger.add_drag_ended_listener(this);

	_save_buffer = 0;

    // Handler to delete this object when the toolbox window has been hidden
    _window.add_has_been_hidden_listener(
              new tbx::DeleteClassOnHidden<SaverTestWnd>(this)
            );
}

/**
 * Destructor - deletes toolbox object handled by this class
 */
SaverTestWnd::~SaverTestWnd()
{
   _window.delete_object();
   delete [] _save_buffer;
}


/**
 * Load file and return true if successful
 *
 * @param filename file to load
 * @param estimated_size estimated size of the file to load
 * @return true if file is loaded successfully or false otherwise
 */
bool SaverTestWnd::load_file(const std::string &file_name, int estimated_size)
{
    //For now just set the window title
    _window.title(file_name);

    if (logging) log << "load_file: File name " << file_name << " size " << estimated_size;

    std::ifstream file(file_name.c_str());
    if (!file)
    {
    	if (logging) log << " load failed" << std::endl;
    } else
    {
        file.seekg(0, std::ios::end);
    	int len = file.tellg();
        file.seekg(0, std::ios::beg);
        if (logging) log << " actual size " << len << std::endl;
        char buf[len+1];
        file.read(buf, len);
        buf[len] = 0;
        _text.text(buf);
    }

    return true;
}

/**
 * Add text to control
 */
void SaverTestWnd::add_text(const char *text)
{
	_text.insert_text(_text.text_length(), text);
}

/**
 * Show the window after a file has been loaded
 */
void SaverTestWnd::show()
{
   // Show in default place
   _window.show();
}


/**
 * Draggable drag has ended, so start save
 */
void SaverTestWnd::drag_ended(const tbx::DragEndedEvent &drag_ended_event)
{
	tbx::Saver saver;
	saver.set_finished_handler(this);
	saver.set_save_completed_handler(this);
	saver.set_save_to_file_handler(this);
	saver.set_fill_buffer_handler(this);
	saver.save(drag_ended_event.where(), "dragtext", 0xFFF, _text.text_length());
}

/**
 * Save has finished
 */
void SaverTestWnd::saver_finished(const tbx::SaverFinishedEvent &finished)
{
	if (logging) log << "saver_finished, save done " << finished.save_done() << std::endl;
	delete [] _save_buffer;
	_save_buffer = 0;
}

/**
 * Save has completed
 */
void SaverTestWnd::saver_save_completed(tbx::SaverSaveCompletedEvent &event)
{
	if (logging) log << "saver_save_completed, file name " << event.file_name()
			<< " safe " << event.safe() << std::endl;
}

/**
 * Request to save to file
 */
void SaverTestWnd::saver_save_to_file(tbx::Saver saver, std::string file_name)
{
	if (logging) log << "saver_save_to_file, file name " << file_name << std::endl;

	std::ofstream file(file_name.c_str());
	file << _text.text();
	bool ok = file.good();
	file.close();

	saver.file_save_completed(ok, file_name);
}

/**
 * Request to fill a buffer for transfer
 */
void SaverTestWnd::saver_fill_buffer(tbx::Saver saver, int size, void *buffer, int already_transmitted)
{
	if (logging) log << "saver_fill_buffer, size = " << size << ", buffer = " << (int)buffer
			<< ", already transmitted " << already_transmitted << std::endl;

	std::string text = _text.text();
	int left = text.size() - already_transmitted;

	if (buffer == 0)
	{
		// Only maximum of our size and their size is needed
		if (size > left) size = left;
		_save_buffer = new char[size+1]; // + 1 just for logging
		buffer = _save_buffer;
	}
	if (left > size) left = size;
	text.copy(_save_buffer, left, already_transmitted);
	_save_buffer[left] = 0;

	if (logging) log << "saver_fill_buffer, sending " << left
			<< " chars " << _save_buffer << std::endl;

	saver.buffer_filled(_save_buffer, left);

}
