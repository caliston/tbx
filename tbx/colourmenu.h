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
 * colourmenu.h
 *
 *  Created on: 01-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_COLOURMENU_H_
#define TBX_COLOURMENU_H_

#include "showpointobject.h"
#include "listener.h"
#include "window.h"
#include "colour.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class ColourMenuSelectionListener;

namespace res
{
    class ResColourMenu;
}

/**
 * Class to show a toolbox ColourMenu object.

 */
class ColourMenu : public tbx::ShowPointObject
{
public:
	enum { TOOLBOX_CLASS = 0x82980 };

	ColourMenu() {};
	ColourMenu(const ColourMenu &other) : ShowPointObject(other._handle) {}
	ColourMenu(const Object &other) : ShowPointObject(other)	{check_toolbox_class(ColourMenu::TOOLBOX_CLASS);}
	ColourMenu(const std::string &template_name) : ShowPointObject(template_name)	{check_toolbox_class(ColourMenu::TOOLBOX_CLASS);}
	ColourMenu(const res::ResColourMenu &object_template);

	ColourMenu &operator=(const ColourMenu &other) {_handle = other.handle(); return *this;}
	ColourMenu &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}

	// Properties

	/**
	 * Set selected colour for menu
	 */
	void colour(WimpColour colour) {int_property(0, (int)colour);}

	/**
	 * Get selected menu colour.
	 */
	WimpColour colour() {return (WimpColour)int_property(1);}

	/**
	 * Set if "no colour" is available
	 */
	void none_available(bool allow_none) {bool_property(2, allow_none);}

	/**
	 * Get if menu include "no colour" (none) option
	 */
	bool none_availble() const {return bool_property(3);}


	/**
	 * Set Title
	 */
	void title(std::string msg) {string_property(4, msg);}

	/**
	 * Return title
	 */
	std::string title() const {return string_property(5);}

	/**
	 * Return the length of the title
	 */
	int title_length() const {return string_property_length(5);}

	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void add_selection_listener(ColourMenuSelectionListener *listener);
	void remove_selection_listener(ColourMenuSelectionListener *listener);

};

/**
 * Listener for when an item is selected from the colour menu
 */
class ColourMenuSelectionListener : public Listener
{
public:
	~ColourMenuSelectionListener() {};

	/**
	 * Called when colour selected from menu
	 *
	 * @param colour_menu Menu colour was selected from
	 * @param WimpColour selected. Can be WimpColour::no_colour if none was selected.
	 */
	virtual void colourmenu_selection(ColourMenu colour_menu, WimpColour colour) = 0;
};

}

#endif /* TBX_COLOURMENU_H_ */
