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

#ifndef TBX_RES_RESTEXTAREA_H
#define TBX_RES_RESTEXTAREA_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for TextArea gadget template
 */
class ResTextArea : public ResGadget
{

public:
	enum {TYPE_ID = 0x4018 };

	ResTextArea(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResTextArea(const ResTextArea &other) : ResGadget(other)
	{
	}

	virtual ~ResTextArea() {}

	ResTextArea &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResTextArea &operator=(const ResTextArea &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResTextArea()
	  : ResGadget(0x4018,56)
	{
		init_message(44,0); // text
		background(0xffffff00); // white
	}

	bool has_vertical_scrollbar() const {return flag(0, 1<<0);}
	void has_vertical_scrollbar(bool value) {flag(0,1<<0,value);}
	bool has_horizontal_scrollbar() const {return flag(0, 1<<1);}
	void has_horizontal_scrollbar(bool value) {flag(0,1<<1,value);}
	bool wordwrap() const {return flag(0, 1<<2);}
	void wordwrap(bool value) {flag(0,1<<2,value);}
	bool allow_selection() const {return flag(0, 1<<3);}
	void allow_selection(bool value) {flag(0,1<<3,value);}
	bool allow_save() const {return flag(0, 1<<4);}
	void allow_save(bool value) {flag(0,1<<4,value);}
	bool allow_load() const {return flag(0, 1<<5);}
	void allow_load(bool value) {flag(0,1<<5,value);}
	bool clipboard() const {return flag(0, 1<<6);}
	void clipboard(bool value) {flag(0,1<<6,value);}
	bool auto_scroll() const {return flag(0, 1<<7);}
	void auto_scroll(bool value) {flag(0,1<<7,value);}

	int type() const {return int_value(36);}
	void type(int value) {int_value(36,value);}
	int event() const {return int_value(40);}
	void event(int value) {int_value(40,value);}
	const char *text() const {return message(44);}
	void text(const char *value) {message(44, value);}
	void text(const std::string &value) {message(44, value);}
	int foreground() const {return int_value(48);}
	void foreground(int value) {int_value(48,value);}
	int background() const {return int_value(52);}
	void background(int value) {int_value(52,value);}

};

}
}

#endif // TBX_RES_RESTEXTAREA_H

