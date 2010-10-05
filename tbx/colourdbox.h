/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2010 Alan Buckley   All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * colourdbox.h
 *
 *  Created on: 01-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_COLOURDBOX_H_
#define TBX_COLOURDBOX_H_

#include "showpointobject.h"
#include "listener.h"
#include "window.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class ColourSelectedListener;
class ColourDboxDialogueCompletedListener;

/**
 * Class to show a toolbox ColourDbox object which allows a user to
 * pick a colour using a variety of colour models.
 *
 * The ColourDbox object uses the system ColourPicker module to implement
 * the Colour dialogue.
 */
class ColourDbox : public tbx::ShowPointObject
{
public:
	enum { TOOLBOX_CLASS = 0x829C0 };

	ColourDbox() {};
	ColourDbox(const ColourDbox &other) : ShowPointObject(other._handle) {}
	ColourDbox(const Object &other) : ShowPointObject(other)	{check_toolbox_class(ColourDbox::TOOLBOX_CLASS);}
	ColourDbox(const std::string &template_name) : ShowPointObject(template_name)	{check_toolbox_class(ColourDbox::TOOLBOX_CLASS);}

	ColourDbox &operator=(const ColourDbox &other) {_handle = other.handle(); return *this;}
	ColourDbox &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Return the underlying wimp window used by the ColourPicker module.
	 */
	WindowHandle wimp_handle() const {return (WindowHandle)int_property(0);}

	/**
	 * Return the underlying dialogue handle used by the ColourPicker module.
	 */
	int dialog_handle() const {return int_property(1);}

	enum ColourModel {RGB, CYMK, HSV};

	/**
	 * Set if an option for no colour is included
	 */
	void none_available(bool available) {bool_property(6, available);}
	/**
	 * Return true if a no colour option is included on the dialog box
	 */
	bool none_available() const {return bool_property(7);}

	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_dialogue_completed_listener(ColourDboxDialogueCompletedListener *listener);
	void remove_dialogue_completed_listener(ColourDboxDialogueCompletedListener *listener);
	void add_colour_selected_listener(ColourSelectedListener *listener);
	void remove_colour_selected_listener(ColourSelectedListener *listener);

};

/**
 * Listener for when the Colour dialogue box has been closed
 */
class ColourDboxDialogueCompletedListener : public Listener
{
public:
	/**
	 * Called when dialog has be closed
	 * @param colour_selected true if the dialogue box was closed after a colour was selected.
	 * This value will also be false if a colour was selected by a click on OK with adjust
	 * and then cancelling the dialogue box.
	 */
	virtual void colourdbox_dialogue_completed(ColourDbox colour_dbox, bool colour_selected) = 0;
};

/**
 * Event data for colour selected event
 */
class ColourSelectedEvent
{
	ColourDbox _colour_dbox;
	bool _none;
	unsigned char *_block;
public:
	ColourSelectedEvent(ColourDbox cbox, bool none, const unsigned char *block);
	ColourSelectedEvent(const ColourSelectedEvent &other);
	~ColourSelectedEvent() {delete _block;}
	ColourSelectedEvent &operator=(const ColourSelectedEvent &other);

	/**
	 * Return colour dbox that generated this event
	 */
	ColourDbox colour_dbox() const {return _colour_dbox;}

	/**
	 * None colour was selected.
	 */
	bool none() const {return _none;}

	/**
	 * Blue amount selected
	 */
	unsigned char blue() const {return _block[1];}
	/**
	 * Green amount selected
	 */
	unsigned char green() const {return _block[2];}
	/**
	 * Red amount selected
	 */
	unsigned char red() const {return _block[3];}
	/**
	 * Colour model selected
	 */
	ColourDbox::ColourModel model() const {return ColourDbox::ColourModel(_block[8]);}
};

/**
 * Event listener for colour selected event
 */
class ColourSelectedListener : public Listener
{
public:
	virtual ~ColourSelectedListener() {};

	/**
	 * Colour has been selected in ColourDbox nd OK is pressed.
	 */
	virtual void colour_selected(const ColourSelectedEvent &event) = 0;
};


}

#endif /* TBX_COLOURDBOX_H_ */
