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
 * textarea.h
 *
 *  Created on: 30-Mar-2009
 *      Author: alanb
 */

#ifndef TBX_TEXTAREA_H_
#define TBX_TEXTAREA_H_

#include "gadget.h"
#include "colour.h"

namespace tbx {

/**
 * Class to manipulate a toolbox TextArea gadget.
 *
 * A text area allows multiple lines of text to be displayed and/or editied
 * in a window.
 */
class TextArea: public tbx::Gadget
{
public:
	enum {TOOLBOX_CLASS = 0x4018}; //!< Toolbox class for this gadget.

	TextArea() {} //!< Construct an uninitialised text area.
	/**
	 * Destroy a text area gadget.
	 *
	 * This does not delete the underlying toolbox gadget.
	 */
	~TextArea() {}

	/**
	 * Construct a text area from another text area.
	 *
	 * Both TextArea will refer to the same underlying toolbox gadget.
	 */
	TextArea(const TextArea &other) : Gadget(other) {}

	/**
	 * Construct a text area from another gadget.
	 *
	 * The TextArea and the Gadget will refer to the same
	 * underlying toolbox gadget.
	 *
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a text area
	 */
	TextArea(const Gadget &other) : Gadget(other)	{check_toolbox_class(TextArea::TOOLBOX_CLASS);}

	/**
	 * Construct a text area from a gadget.
	 *
	 * The TextArea and the Component will refer to the same
	 * underlying toolbox component.
	 */
	TextArea(const Component &other) : Gadget(other) {Window check(other.handle()); check_toolbox_class(TextArea::TOOLBOX_CLASS);}

	/**
	 * Assign a text area to refer to the same underlying toolbox gadget
	 * as another.
	 */
	TextArea &operator=(const TextArea &other) {_handle = other._handle; _id = other._id; return *this;}

	/**
	 * Assign a text area to refer to the same underlying toolbox gadget
	 * as an existing Gadget.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a text area
	 */
	TextArea &operator=(const Gadget &other) {_handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Assign a text area to refer to the same underlying toolbox component
	 * as an existing Gadget.
	 * @throws ObjectClassError if the component is not in a window.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a text area
	 */
	TextArea &operator=(const Component &other) {Window check(other.handle()); _handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Check if this text area refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they refer to the same underlying toolbox gadget.
	 */
	bool operator==(const Gadget &other) const {return (_handle == other.handle() && _id == other.id());}

	/**
	 * Check if this text area refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they do not refer to the same underlying toolbox gadget.
	 */
	bool operator!=(const Gadget &other) const {return (_handle != other.handle() || _id != other.id());}


	/**
	 * Return all state flags
	 */
	int state() const {return int_property(0x4018);}

	/**
	 * Set all state flags
	 */
	void state(int state) {int_property(0x4019, state);}

	bool has_vertical_scrollbar() const {return flag_property(0x4018, 1);}
	void has_vertical_scrollbar(bool value) {flag_property(0x4018, 1, value);}
	bool has_horizontal_scrollbar() const {return flag_property(0x4018, 2);}
	void has_horizontal_scrollbar(bool value) {flag_property(0x4018, 2, value);}
	bool wordwrap() const {return flag_property(0x4018, 4);}
	void wordwrap(bool value) {flag_property(0x4018, 4, value);}
	bool allow_selection() const {return flag_property(0x4018, 8);}
	void allow_selection(bool value) {flag_property(0x4018, 8, value);}
	bool allow_save() const {return flag_property(0x4018, 0x10);}
	void allow_save(bool value) {flag_property(0x4018, 0x10, value);}
	bool allow_load() const {return flag_property(0x4018, 0x20);}
	void allow_load(bool value) {flag_property(0x4018, 0x20, value);}
	bool clipboard() const {return flag_property(0x4018, 0x40);}
	void clipboard(bool value) {flag_property(0x4018, 0x40, value);}
	bool auto_scroll() const {return flag_property(0x4018, 0x80);}
	void auto_scroll(bool value) {flag_property(0x4018, 0x80, value);}

	void text(const std::string &text) {string_property(0x401A, text);}
	std::string text() const {return string_property(0x401B);}
	int text_length() const {return string_property_length(0x401B);}

	void insert_text(int where, const std::string &text);
	void replace_text(int start, int end, const std::string &text);

	void get_selecton(int &start, int &end) const;
	void set_selection(int start, int end);
	int selection_length() const;
	std::string selection_text() const;

	void font(const std::string &name, int width, int height);
	void system_font(int width, int height);

	void set_colour(WimpColour foreground, WimpColour background);
	void set_colour(Colour foreground, Colour background);
	void get_colour(Colour &foreground, Colour &background) const;

	void border_size(int value) {int_property(0x4023, value);}
};

}

#endif /* TBX_TEXTAREA_H_ */
