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

#ifndef TBX_RES_RESACTIONBUTTON_H
#define TBX_RES_RESACTIONBUTTON_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for ActionButton gadget template
 */
class ResActionButton : public ResGadget
{

public:
	enum {TYPE_ID = 128 };

	ResActionButton(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResActionButton(const ResActionButton &other) : ResGadget(other)
	{
	}

	virtual ~ResActionButton() {}

	ResActionButton &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResActionButton &operator=(const ResActionButton &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResActionButton()
	  : ResGadget(128,52)
	{
		init_message(36,0); // text
		init_string(44,0); // click_show
	}

	bool default() const {return flag(0, 1<<0);}
	void default(bool value) {flag(0,1<<0,value);}
	bool cancel() const {return flag(0, 1<<1);}
	void cancel(bool value) {flag(0,1<<1,value);}
	bool local() const {return flag(0, 1<<2);}
	void local(bool value) {flag(0,1<<2,value);}
	bool click_show_transient() const {return flag(0, 1<<3);}
	void click_show_transient(bool value) {flag(0,1<<3,value);}

	const char *text() const {return message(36);}
	void text(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void text(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_text_len() const {return int_value(40);}
	const char *click_show() const {return string(44);}
	void click_show(const char *value) {string(44, value);}
	void click_show(const std::string &value) {string(44, value);}
	int event() const {return int_value(48);}
	void event(int value) {int_value(48,value);}

};

}
}

#endif // TBX_RES_RESACTIONBUTTON_H

