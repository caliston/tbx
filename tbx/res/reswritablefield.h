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

#ifndef TBX_RES_RESWRITABLEFIELD_H
#define TBX_RES_RESWRITABLEFIELD_H

#include "resgadget.h"
#include "ResJustification.h"

namespace tbx {
namespace res {

/**
 * Class for WritableField gadget template
 */
class ResWritableField : public ResGadget
{

public:
	enum {TYPE_ID = 512 };

	ResWritableField(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResWritableField(const ResWritableField &other) : ResGadget(other)
	{
	}

	virtual ~ResWritableField() {}

	ResWritableField &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResWritableField &operator=(const ResWritableField &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResWritableField()
	  : ResGadget(512,60)
	{
		init_message(36,0); // text
		init_message(44,0); // allowable
		before(-1);
		after(-1);
	}

	bool generate_value_changed() const {return flag(0, 1<<0);}
	void generate_value_changed(bool value) {flag(0,1<<0,value);}
	/**
	 * Get/Set justification:
	*/
	ResJustification justification() const {return ResJustification(flag_value(0, 12)>>2);}
	void justification(ResJustification value) {flag_value(0,12,(int)value<<2);}
	bool do_not_display_text() const {return flag(0, 1<<4);}
	void do_not_display_text(bool value) {flag(0,1<<4,value);}

	const char *text() const {return message(36);}
	void text(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void text(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_text_len() const {return int_value(40);}
	const char *allowable() const {return message(44);}
	void allowable(const char *value, int max_length = -1) {message_with_length(44, value, max_length);}
	void allowable(const std::string &value, int max_length = -1) {message_with_length(44, value, max_length);}
	int max_allowable_len() const {return int_value(48);}
	int before() const {return int_value(52);}
	void before(int value) {int_value(52,value);}
	int after() const {return int_value(56);}
	void after(int value) {int_value(56,value);}

};

}
}

#endif // TBX_RES_RESWRITABLEFIELD_H

