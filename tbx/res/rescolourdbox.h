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

#ifndef TBX_RES_RESCOLOURDBOX_H
#define TBX_RES_RESCOLOURDBOX_H

#include "ResObject.h"

namespace tbx {
namespace res {

/**
 * Class for ColourDbox object template
 */
class ResColourDbox : public ResObject
{

public:
	enum {CLASS_ID = 0x829C0 };

	/**
	 * Construct a colour dialogue resource from a ResObject
	 *
	 * @param other ResObject to construct from
	 * @throws ResObjectClassMismatch if the ResObject is not a colour dialogue resource
	 */
	ResColourDbox(const ResObject &other) : ResObject(other)
	{
     check_class_id(CLASS_ID);
	}

	/**
	 * Construct a colour dialogue resource by copying another
	 *
	 * @param other ResColourDbox to copy
	 */
	ResColourDbox(const ResColourDbox &other) : ResObject(other)
	{
	}

	virtual ~ResColourDbox() {}

	/**
	 * Assign from a ResObject
	 *
	 * @param other ResObject to copy
	 * @throws ResObjectClassMismatch if the ResObject is not a colour dialogue resource
	 */
	ResColourDbox &operator=(const ResObject &other)
	{
		other.check_class_id(CLASS_ID);
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Assign from another colour dialogue resource
	 *
	 * @param other button gadget resource to copy
	 */
	ResColourDbox &operator=(const ResColourDbox &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Construct a colour dialogue resource
	 *
	 * All options are false, events unset and messages are null.
	 *
	 * @param name the name of this object (maximum 12 characters)
	 */
	ResColourDbox(std::string name)
	  : ResObject(name, CLASS_ID, 100, 36+16)
	{
		init_message(4,0); // title
	}


	int flags() const {return int_value(0);}
	void flags(int value) {int_value(0,value);}
	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}
	bool generate_dialogue_completed() const {return flag(0, 1<<1);}
	void generate_dialogue_completed(bool value) {flag(0,1<<1,value);}
	bool include_none() const {return flag(0, 1<<2);}
	void include_none(bool value) {flag(0,1<<2,value);}
	bool select_none() const {return flag(0, 1<<3);}
	void select_none(bool value) {flag(0,1<<3,value);}

	const char *title() const {return message(4);}
	void title(const char *value, int max_length = -1) {message_with_length(4, value, max_length);}
	void title(const std::string &value, int max_length = -1) {message_with_length(4, value, max_length);}
	int max_title() const {return int_value(8);}
	int colour() const {return int_value(12);}
	void colour(int value) {int_value(12,value);}

};

}
}

#endif // TBX_RES_RESCOLOURDBOX_H

