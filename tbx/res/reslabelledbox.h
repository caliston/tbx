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

#ifndef TBX_RES_RESLABELLEDBOX_H
#define TBX_RES_RESLABELLEDBOX_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for LabelledBox gadget template
 */
class ResLabelledBox : public ResGadget
{

public:
	enum {TYPE_ID = 256 };

	ResLabelledBox(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResLabelledBox(const ResLabelledBox &other) : ResGadget(other)
	{
	}

	virtual ~ResLabelledBox() {}

	ResLabelledBox &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResLabelledBox &operator=(const ResLabelledBox &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResLabelledBox()
	  : ResGadget(256,40)
	{
		back(true);
		init_message(36,0); // label
	}


	const char *label() const {return message(36);}
	void label(const char *value) {message(36, value);}
	void label(const std::string &value) {message(36, value);}

};

}
}

#endif // TBX_RES_RESLABELLEDBOX_H

