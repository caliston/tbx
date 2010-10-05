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
 * fontmenu.h
 *
 *  Created on: 01-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_FONTMENU_H_
#define TBX_FONTMENU_H_

#include "showpointobject.h"
#include "listener.h"
#include "eventinfo.h"
#include "window.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class FontMenuSelectionListener;


/**
 * Class to show a toolbox FontMenu object.
 * Shows a menu where a font can be selected.static void saveas_dialog_completed_router(IdBlock &id_block, PollBlock &data, Listener *listener)
 */
class FontMenu : public tbx::ShowPointObject
{
public:
	enum { TOOLBOX_CLASS = 0x82a40 };

	FontMenu() {};
	FontMenu(const FontMenu &other) : ShowPointObject(other._handle) {}
	FontMenu(const Object &other) : ShowPointObject(other)	{check_toolbox_class(FontMenu::TOOLBOX_CLASS);}
	FontMenu(const std::string &template_name) : ShowPointObject(template_name)	{check_toolbox_class(FontMenu::TOOLBOX_CLASS);}

	FontMenu &operator=(const FontMenu &other) {_handle = other.handle(); return *this;}
	FontMenu &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Set font to select.
	 *
	 * @param value name of font to select or "SystemFont" for the System entry.
	 */
	void font(std::string value) {string_property(0, value);}

	/**
	 * Set dialog to no font selected
	 */
	void no_font() {int_property(0, 0);}

	/**
	 * Return selected font
	 */
	std::string font() const {return string_property(1);}

	/**
	 * Return selected font name length
	 */
	int font_length() const {return string_property_length(1);}

	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void add_selection_listener(FontMenuSelectionListener *listener);
	void remove_selection_listener(FontMenuSelectionListener *listener);
};


/**
 * Class to return information on font menu selection event
 */
class FontMenuSelectionEvent : public EventInfo
{
public:
	FontMenuSelectionEvent(IdBlock &id_block, PollBlock & data) :
		EventInfo(id_block, data) {}

	/**
	 * Returns id of the font selected or "SystemFont" for the system entry.
	 */
	std::string font() const {return reinterpret_cast<const char *>(_data.word[4]);}
};

/**
 * Listener for font menu selection events
 */
class FontMenuSelectionListener : public Listener
{
public:
	~FontMenuSelectionListener() {};

	virtual void fontmenu_selection(const FontMenuSelectionEvent &font_selected) = 0;
};

}

#endif /* TBX_FONTMENU_H_ */
