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

#ifndef TBX_RES_RESBUTTON_H
#define TBX_RES_RESBUTTON_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for Button gadget template.
 */
class ResButton : public ResGadget
{

public:
	enum {TYPE_ID = 960 };

	/**
	 * Construct an button gadget resource
	 *
	 * @param other ResGadget to copy resource from
	 * @throws ResGadgetTypeMismatch if the ResGadget is not an button
	 */
	ResButton(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	/**
	 * Construct an button gadget resource
	 *
	 * @param other button to copy resource from
	 */
	ResButton(const ResButton &other) : ResGadget(other)
	{
	}

	virtual ~ResButton() {}

	/**
	 * Assign from a ResGadget
	 *
	 * @param other ResGadget to copy
	 * @throws ResGadgetTypeMismatch if the ResGadget is not an button
	 */
	ResButton &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Assign from another button gadget resource
	 *
	 * @param other button gadget resource to copy
	 */
	ResButton &operator=(const ResButton &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Construct a button gadget resource.
	 *
	 * All options are false, events unset and messages are null.
	 */
	ResButton()
	  : ResGadget(960,56)
	{
		int_value(36, 0x7000110); // foreground 7, icon data indirected (always set)
		init_message(40,0); // value
		init_string(48,0); // validation
	}

	bool use_task_sprite_area() const {return flag(0, 1<<0);}
	void use_task_sprite_area(bool value) {flag(0,1<<0,value);}
	bool return_menu_clicks() const {return flag(0, 1<<1);}
	void return_menu_clicks(bool value) {flag(0,1<<1,value);}

	/**
	 * Raw access to all button flags.
	 *
	 * It is recommended you use the other methods in this
	 * class rather than use this directly.
	 */
	int button_flags() const {return int_value(36);}
	void button_flags(int value) {int_value(36,value);}

	bool has_text() const {return flag(36,1);}
	void has_text(bool value) {flag(36, 1, value );}
	bool is_sprite() const {return flag(36,1<<1);}
	void is_sprite(bool value) {flag(36, 1<<1, value);}
	bool has_border() const {return flag(36,1<<2);}
	void has_border(bool value) {flag(36, 1<<2, value);}
	bool hcentered() const {return flag(36,1<<3);}
	void hcentered(bool value) {flag(36, 1<<3, value);}
	bool vcentered() const {return flag(36,1<<4);}
	void vcentered(bool value) {flag(36, 1<<4, value);}
	bool filled() const {return flag(36, 1<<5);}
	void filled(bool value) {flag(36, 1<<5, value);}
	bool anti_aliased_font() const {return flag(36, 1 << 6);}
	void anti_aliased_font(bool value) {flag(36, 1<<6, value);}
	// 6  text is an anti-aliased font (affects meaning of bits 24 - 31)  
	bool redraw_needs_help() const {return flag(36, 1 << 7);}
	void redraw_needs_help(bool value) {flag(36, 1<<7, value);}
	// bit 8 - icon data is indirected always set
	bool text_right_justified() const {return flag(36, 1 << 9);}
	void text_right_justified(bool value) {flag(36, 1<<9, value);}
	bool adjust_doesnt_cancel() const {return flag(36, 1 << 10);}
	void adjust_doesnt_cancel(bool value) {flag(36, 1<<10, value);}
	bool half_size_sprite() const {return flag(36, 1 << 11);}
	void half_size_sprite(bool value) {flag(36, 1<<11, value);}

	enum ButtonType
	{
		IGNORE_CLICKS, // ignore mouse clicks or movements over the icon (except Menu)  
		CONTINUOUS,    // notify task continuously while pointer is over this icon  
		CLICK_AUTO_REPEAT, // click notifies task (auto-repeat)
		CLICK_ONCE, // click notifies task (once only)  
		CLICK_SELECTS, // click selects the icon; release over the icon notifies task; moving the pointer away deselects the icon  
		CLICK_SELECTS_DOUBLE, // click selects; double click notifies task  
		CLICK_ONCE_DRAG, // click notifies, but can also drag (returns button state * 16)  
		CLICK_SELECTS_DRAG, // as CLICK_SELECTS, but can also drag (returns button state * 16) and moving away from the icon doesn't deselect it  
		CLICK_SELECTS_DOUBLE_DRAG, // as CLICK_SELECTS_DOUBLE, but can also drag (returns button state * 16)  
		OVER_SELECTS, // pointer over icon selects; moving away from icon deselects; click over icon notifies task ('menu' icon)  
		CLICK_DOUBLE_DRAG, //click returns button state * 256, drag returns button state * 16, double click returns button state * 1  
		CLICK_NOTIFY_AND_SELECTS_DRAG, // click selects icon and returns button state, drag returns button state*16  
		FOCUS_AND_DRAG = 14, // clicks cause the icon to gain the caret and its parent window to become the input focus and can also drag (writable icon). For example, this is used by the FormEd application
		WRITEABLE, // clicks cause the icon to gain the caret and its parent window to become the input focus (writable icon) 
	};

	ButtonType button_type() const {return ButtonType(flag_value(36, 0xF000) >> 12);}
	void button_type(ButtonType type) {flag_value(36, ((int)type) << 12);}
	int esg() const {return flag_value(36, 0x1F0000) >> 16;}
	void esg(int value) {flag_value(36, value << 16);}

	int foreground() const {return flag_value(36,0xF000000)>>24;} // !anti_aliased_font
	void foreground(int value) {flag_value(36, value << 24);}
	int background() const {return flag_value(36,0xF0000000)>>28;} // !anti_aliased_font
	void background(int value) {flag_value(36, value << 28);}
	int font_handle() const {return flag_value(36,0xFF000000) >> 24;} // anti_aliased_font
	void font_handle(int value) {flag_value(36, value << 24);}

	const char *value() const {return message(40);}
	void value(const char *value, int max_length = -1) {message_with_length(40, value, max_length);}
	void value(const std::string &value, int max_length = -1) {message_with_length(40, value, max_length);}
	int max_value() const {return int_value(44);}
	const char *validation() const {return string(48);}
	void validation(const char *value, int max_length = -1) {string_with_length(48, value, max_length);}
	void validation(const std::string &value, int max_length = -1) {string_with_length(48, value, max_length);}
	int max_validation() const {return int_value(52);}

};

}
}

#endif // TBX_RES_RESBUTTON_H

