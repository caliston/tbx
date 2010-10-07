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

#ifndef TBX_RES_RESQUIT_H
#define TBX_RES_RESQUIT_H

#include "ResObject.h"

namespace tbx {
namespace res {

/**
 * Class for Quit object template
 */
class ResQuit : public ResObject
{

public:
	enum {CLASS_ID = 0x82a90 };

	ResQuit(const ResObject &other) : ResObject(other)
	{
     check_class_id(CLASS_ID);
	}

	ResQuit(const ResQuit &other) : ResObject(other)
	{
	}

	virtual ~ResQuit() {}

	ResQuit &operator=(const ResObject &other)
	{
		other.check_class_id(CLASS_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResQuit &operator=(const ResQuit &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResQuit(std::string name)
	  : ResObject(name, CLASS_ID, 100, 36 + 24)
	{
		init_message(4,0); // title
		init_message(12,0); // message
		init_string(20,0); // window
	}


	int flags() const {return int_value(0);}
	void flags(int value) {int_value(0,value);}
	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}
	bool generate_dialogue_completed() const {return flag(0, 1<<1);}
	void generate_dialogue_completed(bool value) {flag(0,1<<1,value);}

	const char *title() const {return message(4);}
	void title(const char *value, int max_length = -1) {message_with_length(4, value, max_length);}
	void title(const std::string &value, int max_length = -1) {message_with_length(4, value, max_length);}
	int max_title() const {return int_value(8);}
	const char *alternative_message() const {return message(12);}
	void alternative_message(const char *value, int max_length = -1) {message_with_length(12, value, max_length);}
	void alternative_message(const std::string &value, int max_length = -1) {message_with_length(12, value, max_length);}
	int max_alternative_message() const {return int_value(16);}
	const char *window() const {return string(20);}
	void window(const char *value) {string(20, value);}
	void window(const std::string &value) {string(20, value);}

};

}
}

#endif // TBX_RES_RESQUIT_H

