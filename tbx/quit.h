/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2009 Alan Buckley   All Rights Reserved.
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
/*
 * quit.h
 *
 *  Created on: 09-Jun-2009
 *      Author: alanb
 */

#ifndef TBX_QUIT_H_
#define TBX_QUIT_H_

#include "showfullobject.h"
#include "listener.h"
#include "window.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class QuitQuitListener;
class QuitCancelListener;


/**
 * Class to show a toolbox Quit object that gives
 * the user an option to Quit or Cancel the application.
 *
 * This is usually used when the prequit message is sent from the
 * WIMP and there are some modified documents.
 */
class Quit : public tbx::ShowFullObject
{
public:
	enum { TOOLBOX_CLASS = 0x82a90 };

	Quit() {};
	Quit(const Quit &other) : ShowFullObject(other._handle) {}
	Quit(const Object &other) : ShowFullObject(other)	{check_toolbox_class(Quit::TOOLBOX_CLASS);}
	Quit(const std::string &template_name) : ShowFullObject(template_name)	{check_toolbox_class(Quit::TOOLBOX_CLASS);}

	Quit &operator=(const Quit &other) {_handle = other.handle(); return *this;}
	Quit &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Return the underlying toolbox window used to implement the Quit object
	 */
	Window window() const {return Window((ObjectId)int_property(0));}

	/**
	 * Set message to display in window
	 */
	void message(std::string msg) {string_property(1, msg);}

	/**
	 * Return message
	 */
	std::string message() const {return string_property(2);}
	/**
	 * Return length of message
	 */
	int message_length() const {return string_property_length(2);}

	/**
	 * Set title of window
	 */
	void title(std::string msg) {string_property(3, msg);}

	/**
	 * Return title
	 */
	std::string title() const {return string_property(4);}
	/**
	 * Return length of title
	 */
	int title_length() const {return string_property_length(4);}

	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void add_quit_listener(QuitQuitListener *listener);
    void remove_quit_listener(QuitQuitListener *listener);
	void add_cancel_listener(QuitCancelListener *listener);
    void remove_cancel_listener(QuitCancelListener *listener);
};

/**
 * Listener for when a Quit is selected from the Quit dialog
 */
class QuitQuitListener : public Listener
{
public:
	/**
	 * Called when quit is selected from a Quit dialog
	 */
	virtual void quit_quit() = 0;
};

/**
 * Listener for when a cancel is selected from the Quit dialog
 */
class QuitCancelListener : public Listener
{
public:
	/**
	 * Called when cancel is selected from a Quit dialog
	 */
	virtual void quit_cancel() = 0;
};


}

#endif /* TBX_QUIT_H_ */
