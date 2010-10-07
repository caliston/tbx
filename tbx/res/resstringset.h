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

#ifndef TBX_RES_RESSTRINGSET_H
#define TBX_RES_RESSTRINGSET_H

#include "resgadget.h"
#include "resjustification.h"

namespace tbx {
namespace res {

/**
 * Class for StringSet gadget template
 */
class ResStringSet : public ResGadget
{

public:
	enum {TYPE_ID = 896 };

	ResStringSet(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResStringSet(const ResStringSet &other) : ResGadget(other)
	{
	}

	virtual ~ResStringSet() {}

	ResStringSet &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResStringSet &operator=(const ResStringSet &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResStringSet()
	  : ResGadget(896,68)
	{
		init_message(36,0); // string_set
		init_message(40,0); // Title
		init_message(44,0); // initial_selected_string
		init_message(52,0); // allowable
		before(-1);
		after(-1);
	}

	bool generate_value_changed() const {return flag(0, 1<<0);}
	void generate_value_changed(bool value) {flag(0,1<<0,value);}
	bool writable() const {return flag(0, 1<<1);}
	void writable(bool value) {flag(0,1<<1,value);}
	bool generate_event_before_showing_menu() const {return flag(0, 1<<3);}
	void generate_event_before_showing_menu(bool value) {flag(0,1<<3,value);}
	bool no_value_field() const {return flag(0, 1<<4);}
	void no_value_field(bool value) {flag(0,1<<4,value);}
	ResJustification justification() const {return ResJustification(flag_value(0, 96)>>5);}
	void justification(ResJustification value) {flag_value(0,96,(int)value<<5);}

	const char *string_set() const {return message(36);}
	void string_set(const char *value) {message(36, value);}
	void string_set(const std::string &value) {message(36, value);}
	const char *title() const {return message(40);}
	void title(const char *value) {message(40, value);}
	void title(const std::string &value) {message(40, value);}
	const char *selected_string() const {return message(44);}
	void selected_string(const char *value, int max_length = -1) {message_with_length(44, value, max_length);}
	void selected_string(const std::string &value, int max_length = -1) {message_with_length(44, value, max_length);}
	int max_selected_string() const {return int_value(48);}
	const char *allowable() const {return message(52);}
	void allowable(const char *value, int max_length = -1) {message_with_length(52, value, max_length);}
	void allowable(const std::string &value, int max_length = -1) {message_with_length(52, value, max_length);}
	int max_allowable() const {return int_value(56);}
	int before() const {return int_value(60);}
	void before(int value) {int_value(60,value);}
	int after() const {return int_value(64);}
	void after(int value) {int_value(64,value);}

};

}
}

#endif // TBX_RES_RESSTRINGSET_H

