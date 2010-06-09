/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TBX_APPLICATION_H
#define TBX_APPLICATION_H

#include "object.h"

/**
 * A library for creating RISC OS toolbox applications.
 */
namespace tbx
{
	class AutoCreateListener;
	class Command;
	class WimpMessageListener;
	class PreQuitListener;
	class QuitListener;
	class SpriteArea;

	/**
	 * The main application class.
	 *
	 * Each application create one Application object, set
	 * up any initial commands and listeners and then
	 * call the run method.
	 *
	 * @code
	 * int main()
	 * {
	 *    Application my_app("<MyApp$Dir>");
	 *
	 *    // Add commands and listeners here
	 *
	 *    my_app.run();
	 *
	 *    return 0;
	 * }
	 * @endcode
	 */
	class Application
	{
	public:
		static const int MIN_WIMP_VERSION; //!< Minimum Wimp version TBX supports
		static const int *NO_MESSAGES; //!< Don't deliver any wimp messages (except QUIT)
		static const int *ALL_MESSAGES; //!< Deliver all messages
		static const int *NO_EVENTS; //!< Don't deliver any toolbox event
		static const int *ALL_EVENTS; //!< Deliver all toolbox events

		Application(const char *task_directory,
				    int wimp_version = MIN_WIMP_VERSION,
				    const int *deliver_messages = ALL_MESSAGES,
				    const int *deliver_events = ALL_EVENTS);

		/**
		 * Get the application instance.
		 * @return Pointer to current application
		 */
		static Application *instance() {return _instance;}

		/**
		 * Get the sprite area.
		 *
		 * @returns sprite area or 0 if no area
		 */
		SpriteArea *sprite_area() const {return _sprite_area;}

		void run();

		void set_autocreate_listener(std::string template_name, AutoCreateListener *listener);
		void clear_autocreate_listener(std::string template_name);

		void add_command(int command_id, Command *command);
		void remove_command(int command_id, Command *command);

		void add_idle_command(Command *command);
		void remove_idle_command(Command *command);

		void add_message_listener(int message_id, WimpMessageListener *listener);
		void remove_message_listener(int message_id, WimpMessageListener *listener);

		void add_prequit_listener(PreQuitListener *listener);
		void remove_prequit_listener(PreQuitListener *listener);

		void add_quit_listener(QuitListener *listener);
		void remove_quit_listener(QuitListener *listener);

		ResHandle *resource(std::string template_name);

		std::string directory() const;

		/**
		 * Quit the application.
		 *
		 * The application will quit once it returns to the
		 * main polling loop inside the run() method.
		 */
		void quit() {_running = false;}

		void unix_filename_translation(bool on);
		bool unix_filename_translation() const;
		void unix_filename_control(int flags);
		int unix_filename_control() const;

	private:
		bool _running;
        int _task_handle;
        int _wimp_version;
        SpriteArea *_sprite_area;
        int _messagesFD[4];

		static Application *_instance;
	};

	/**
	 * Get the current application instance.
	 *
	 * @return A pointer to the current application.
	 */
	inline Application *app() {return Application::instance();}
}

#endif


