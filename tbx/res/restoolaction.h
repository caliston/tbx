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

#ifndef TBX_RES_RESTOOLACTION_H
#define TBX_RES_RESTOOLACTION_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for ToolAction gadget template
 */
class ResToolAction : public ResGadget
{

public:
	enum {TYPE_ID = 0x4014 };

	/**
	 * Construct an tool action gadget resource
	 *
	 * @param other ResGadget to copy resource from
	 * @throws ResGadgetTypeMismatch if the ResGadget is not an tool action
	 */
	ResToolAction(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	/**
	 * Construct an tool action gadget resource
	 *
	 * @param other tool action to copy resource from
	 */
	ResToolAction(const ResToolAction &other) : ResGadget(other)
	{
	}

	virtual ~ResToolAction() {}

	/**
	 * Assign from a ResGadget
	 *
	 * @param other ResGadget to copy
	 * @throws ResGadgetTypeMismatch if the ResGadget is not an tool action
	 */
	ResToolAction &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Assign from another tool action gadget resource
	 *
	 * @param other tool action gadget resource to copy
	 */
	ResToolAction &operator=(const ResToolAction &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Construct a tool action gadget resource.
	 *
	 * All options are false, events unset and messages are null.
	 */
	ResToolAction()
	  : ResGadget(0x4014,76)
	{
		init_string(36,0); // off_text
		init_string(44,0); // on_text
		init_string(56,0); // select_show
		init_string(64,0); // adjust_show
		init_string(68,0); // fade_text
	}

	/**
	 * Check if the select event will be generated
	 *
	 * @returns true if the select event will be generated
	 */
	bool generate_select() const {return flag(0, 1<<0);}
	/**
	 * Set if the select event will be generated
	 *
	 * @param value set to true if the select event will be generated
	 */
	void generate_select(bool value) {flag(0,1<<0,value);}
	/**
	 * Check if tool action includes text
	 *
	 * @returns true if the tool action includes text
	 */
	bool has_text() const {return flag(0, 1<<1);}
	/**
	 * Set if tool action includes text
	 *
	 * @param value set to true if the toolaction includes text
	 */
	void has_text(bool value) {flag(0,1<<1,value);}
	/**
	 * Check if the tool action starts off in the on state
	 *
	 * @returns true if the tool action is on
	 */
	bool on() const {return flag(0, 1<<2);}
	/**
	 * Set if the tool action starts off in the on state
	 *
	 * @param value set to true to set the tool action on
	 */
	void on(bool value) {flag(0,1<<2,value);}
	/**
	 * Check if the tool action should automatically toggle between states
	 *
	 * @returns true if auto toggle is on
	 */
	bool auto_toggle() const {return flag(0, 1<<3);}
	/**
	 * Set if the tool action should automatically toggle between states
	 *
	 * @param value set to true to turn auto toggle on
	 */
	void auto_toggle(bool value) {flag(0,1<<3,value);}
	/**
	 * Check if there is no sprite for the pressed state
	 *
	 * @returns true if there is no pressed sprite
	 */
	bool no_pressed_sprite() const {return flag(0, 1<<4);}
	/**
	 * Set if there is no sprite for the pressed state
	 *
	 * @param value set to true if there is no pressed sprite
	 */
	void no_pressed_sprite(bool value) {flag(0,1<<4,value);}
	/**
	 * Check if button presses auto repeat
	 *
	 * @returns true if button presses auto repeat
	 */
	bool auto_repeat() const {return flag(0, 1<<8);}
	/**
	 * Set if button presses auto repeat
	 *
	 * @param value set to true to make button presses auto repeat
	 */
	void auto_repeat(bool value) {flag(0,1<<8,value);}
	bool show_transient() const {return flag(0, 1<<9);}
	void show_transient(bool value) {flag(0,1<<9,value);}
	bool show_as_pop_up() const {return flag(0, 1<<10);}
	void show_as_pop_up(bool value) {flag(0,1<<10,value);}
	bool has_fade_sprite() const {return flag(0, 1<<11);}
	void has_fade_sprite(bool value) {flag(0,1<<11,value);}

	const char *off_text() const {return string(36);}
	void off_text(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void off_text(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_off_text() const {return int_value(40);}
	const char *on_text() const {return message(44);}
	void on_text(const char *value, int max_length = -1) {message_with_length(44, value, max_length);}
	void on_text(const std::string &value, int max_length = -1) {message_with_length(44, value, max_length);}
	int max_on_text() const {return int_value(48);}
	int click_event() const {return int_value(52);}
	void click_event(int value) {int_value(52,value);}
	const char *select_show() const {return string(56);}
	void select_show(const char *value) {string(56, value);}
	void select_show(const std::string &value) {string(56, value);}
	int adjust_event() const {return int_value(60);}
	void adjust_event(int value) {int_value(60,value);}
	const char *adjust_show() const {return string(64);}
	void adjust_show(const char *value) {string(64,value);}
	void adjust_show(const std::string &value) {string(64, value);}
	const char *fade_text() const {return message(68);}
	void fade_text(const char *value) {message(68, value);}
	void fade_text(const std::string &value) {message(68, value);}
	int max_fade() const {return int_value(72);}
	void max_fade(int value) {int_value(72,value);}

};

}
}

#endif // TBX_RES_RESTOOLACTION_H

