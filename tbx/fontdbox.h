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
 * fontdbox.h
 *
 *  Created on: 01-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_FONTDBOX_H_
#define TBX_FONTDBOX_H_

#include "showfullobject.h"
#include "listener.h"
#include "eventinfo.h"
#include "window.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class FontDboxApplyListener;

/**
 * Class to show a toolbox FontDbox object.
 * This dialogue allows you to choose a font with its size, weight and style.

 */
class FontDbox : public tbx::ShowFullObject
{
public:
	enum { TOOLBOX_CLASS = 0x82a00 };

	FontDbox() {};
	FontDbox(const FontDbox &other) : ShowFullObject(other._handle) {}
	FontDbox(const Object &other) : ShowFullObject(other)	{check_toolbox_class(FontDbox::TOOLBOX_CLASS);}
	FontDbox(const std::string &template_name) : ShowFullObject(template_name)	{check_toolbox_class(FontDbox::TOOLBOX_CLASS);}

	FontDbox &operator=(const FontDbox &other) {_handle = other.handle(); return *this;}
	FontDbox &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Return the underlying toolbox window used to implement the FontDbox object
	 */
	Window window() const {return Window((ObjectId)int_property(0));}

	/**
	 * Set font to select.
	 *
	 * @param value name of font to select or "SystemFont" for the System entry.
	 */
	void font(std::string value) {string_property(1, value);}

	/**
	 * Set dialog to no font selected
	 */
	void no_font() {int_property(1, 0);}

	/**
	 * Return selected font
	 */
	std::string font() const {return string_property(2);}

	/**
	 * Return selected font name length
	 */
	int font_length() const {return string_property_length(2);}

	/**
	 * Set height of font
	 */
	void height(int size);

	/**
	 * Get height of font
	 */
	int height() const;

	/**
	 * Set aspect ratio of font
	 */
	void aspect_ratio(int value);

	/**
	 * Get aspect ration of font
	 */
	int aspect_ratio() const;

	/**
	 * Set string to show in try box
	 */
	void try_string(std::string value) {string_property(5, value);}

	/**
	 * Get string to shown in try box
	 */
	std::string try_string() const {return string_property(6);}

	/**
	 * Get length of string shown in try box
	 */
	int try_string_length() const {return string_property_length(6);}

	/**
	 * Set title
	 */
	void title(std::string value) {string_property(7, value);}

	/**
	 * Get title
	 */
	std::string title() const {return string_property(8);}

	/**
	 * Get length of title
	 */
	int title_length() const {return string_property_length(8);}


	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void add_apply_listener(FontDboxApplyListener *listener);
	void remove_apply_listener(FontDboxApplyListener *listener);
};

/**
 * Event information for FontDboxApplyListener
 */
class FontDboxApplyEvent : public EventInfo
{
public:
	FontDboxApplyEvent(const IdBlock &id_block, PollBlock &data) :
		EventInfo(id_block, data) {}

	/**
	 * Height selected
	 */
	int height() const {return _data.word[4];}

	/**
	 * Aspect ratio selected
	 */
	int aspect_ratio() const {return _data.word[5];}

	/**
	 * Name of font selected.
	 * This will be "SystemFont" if the System entry was selected.
	 */
	const std::string name() const {return reinterpret_cast<const char *>(&_data.word[6]);}
};

/**
 * Listener for FontDbox Apply events
 */
class FontDboxApplyListener : public Listener
{
public:
	~FontDboxApplyListener() {};

	virtual void fontdbox_apply(const FontDboxApplyEvent &apply_event) = 0;
};

}

#endif /* TBX_FONTDBOX_H_ */
