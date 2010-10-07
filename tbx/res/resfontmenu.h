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

#ifndef TBX_RES_RESFONTMENU_H
#define TBX_RES_RESFONTMENU_H

#include "ResObject.h"

namespace tbx {
namespace res {

/**
 * Class for FontMenu object template
 */
class ResFontMenu : public ResObject
{

public:
	enum {CLASS_ID = 0x82a40 };

	ResFontMenu(const ResObject &other) : ResObject(other)
	{
     check_class_id(CLASS_ID);
	}

	ResFontMenu(const ResFontMenu &other) : ResObject(other)
	{
	}

	virtual ~ResFontMenu() {}

	ResFontMenu &operator=(const ResObject &other)
	{
		other.check_class_id(CLASS_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResFontMenu &operator=(const ResFontMenu &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResFontMenu(std::string name)
	  : ResObject(name, CLASS_ID, 100, 36 + 8)
	{
		init_string(4,0); // ticked_font
	}


	int flags() const {return int_value(0);}
	void flags(int value) {int_value(0,value);}
	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}
	bool generate_has_been_hidden() const {return flag(0, 1<<1);}
	void generate_has_been_hidden(bool value) {flag(0,1<<1,value);}
	bool include_system_font() const {return flag(0, 1<<2);}
	void include_system_font(bool value) {flag(0,1<<2,value);}

	const char *ticked_font() const {return string(4);}
	void ticked_font(const char *value) {string(4, value);}
	void ticked_font(const std::string &value) {string(4, value);}

};

}
}

#endif // TBX_RES_RESFONTMENU_H
