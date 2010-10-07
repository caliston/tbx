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

	ResToolAction(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResToolAction(const ResToolAction &other) : ResGadget(other)
	{
	}

	virtual ~ResToolAction() {}

	ResToolAction &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResToolAction &operator=(const ResToolAction &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResToolAction()
	  : ResGadget(0x4014,76)
	{
		init_string(36,0); // off_text
		init_string(44,0); // on_text
		init_string(56,0); // select_show
		init_string(64,0); // adjust_show
		init_string(68,0); // fade_text
	}

	bool generate_select() const {return flag(0, 1<<0);}
	void generate_select(bool value) {flag(0,1<<0,value);}
	bool has_text() const {return flag(0, 1<<1);}
	void has_text(bool value) {flag(0,1<<1,value);}
	bool on() const {return flag(0, 1<<2);}
	void on(bool value) {flag(0,1<<2,value);}
	bool auto_toggle() const {return flag(0, 1<<3);}
	void auto_toggle(bool value) {flag(0,1<<3,value);}
	bool no_pressed_sprite() const {return flag(0, 1<<4);}
	void no_pressed_sprite(bool value) {flag(0,1<<4,value);}
	bool auto_repeat() const {return flag(0, 1<<8);}
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

