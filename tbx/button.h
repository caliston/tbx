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
 * button.h
 *
 *  Created on: 6-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_BUTTON_H_
#define TBX_BUTTON_H_

#include "gadget.h"
#include "mouseclicklistener.h"
#include "eventinfo.h"

namespace tbx
{

/**
 * A Button is a gadget that show a sprite or some text in the window.
 * It is similar to a Wimp icon, but has the following restrictions.
 * A Button created as sprite only cannot be made into any sort of text Button.
 * A Button created as text only cannot be made into a sprite only Button.
 * A sprite only Button can only refer to sprites by name and these must be in the Wimp sprite pool or the task's sprite area.
 */
class Button: public tbx::Gadget
{
public:
	enum {TOOLBOX_CLASS = 960}; //!< Toolbox class for this gadget.

	Button() {} //!< Construct an uninitialised button.
	/**
	 * Destroy a button gadget.
	 *
	 * This does not delete the underlying toolbox gadget.
	 */
	~Button() {}

	/**
	 * Construct a button from another button.
	 *
	 * Both Button will refer to the same underlying toolbox gadget.
	 */
	Button(const Button &other) : Gadget(other) {}

	/**
	 * Construct a button from another gadget.
	 *
	 * The Button and the Gadget will refer to the same
	 * underlying toolbox gadget.
	 *
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a button
	 */
	Button(const Gadget &other) : Gadget(other)	{check_toolbox_class(Button::TOOLBOX_CLASS);}

	/**
	 * Construct a button from a component.
	 *
	 * The Button and the Component will refer to the same
	 * underlying toolbox component.
	 */
	Button(const Component &other) : Gadget(other) {Window check(other.handle()); check_toolbox_class(Button::TOOLBOX_CLASS);}

	/**
	 * Assign a button to refer to the same underlying toolbox gadget
	 * as another.
	 */
	Button &operator=(const Button &other) {_handle = other._handle; _id = other._id; return *this;}

	/**
	 * Assign a button to refer to the same underlying toolbox gadget
	 * as an existing Gadget.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a button
	 */
	Button &operator=(const Gadget &other) {_handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Assign a button to refer to the same underlying toolbox component
	 * as an existing Gadget.
	 * @throws ObjectClassError if the component is not in a window.
	 * @throws GadgetClassError if the underlying toolbox gadget
	 *         isn't a button
	 */
	Button &operator=(const Component &other) {Window check(other.handle()); _handle = other.handle(); _id = other.id(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Check if this button refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they refer to the same underlying toolbox gadget.
	 */
	bool operator==(const Gadget &other) const {return (_handle == other.handle() && _id == other.id());}

	/**
	 * Check if this button refers to the same underlying toolbox gadget
	 * as another gadget.
	 * @returns true if they do not refer to the same underlying toolbox gadget.
	 */
	bool operator!=(const Gadget &other) const {return (_handle != other.handle() || _id != other.id());}

	//TODO: enum ButtonFlag {} - add wimp icon flags for flags properties
	/**
	 * Returns the flags for the button, these are the
	 * same as for a wimp icon.
	 */
	int flags() const {return int_property(960);}

	/**
	 * Set the button flags.
	 *
	 * This is similar to Wimp_SetIconState but not all flags are settable.
	 *
	 * @param clear bits to clear
	 * @param eor bits to exclusive or
	 */
	void flags(int clear, int eor);

	/**
	 * Set the button value. i.e. the text or sprite name.
	 */
	void value(std::string v) {string_property(962, v);}

	/**
	 * Get the button value
	 */
	std::string value() const {return string_property(963);}

	/**
	 * Return the length of the button value
	 */
	int value_length() const {return string_property_length(963);}

	/**
	 * Set the button validation
	 */
	void validation(std::string v) {string_property(964, v);}

	/**
	 * Get the button validation
	 */
	std::string validation() const {return string_property(965);}

	/**
	 * Return the length of the button validation
	 */
	int validation_length() const {return string_property_length(965);}

	void font(const std::string &name, int width, int height);
	void system_font(int width, int height);

	void add_mouse_click_listener(MouseClickListener *listener);
	void remove_mouse_click_listener(MouseClickListener *listener);
};

}

#endif /* TBX_BUTTON_H_ */
