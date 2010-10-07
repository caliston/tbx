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

#ifndef TBX_RES_RESLABEL_H
#define TBX_RES_RESLABEL_H

#include "resgadget.h"
#include "resjustification.h"

namespace tbx {
namespace res {

/**
 * Class for Label gadget template
 */
class ResLabel : public ResGadget
{

public:
	enum {TYPE_ID = 320 };

	ResLabel(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResLabel(const ResLabel &other) : ResGadget(other)
	{
	}

	virtual ~ResLabel() {}

	ResLabel &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResLabel &operator=(const ResLabel &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResLabel()
	  : ResGadget(320,40)
	{
		init_message(36,0); // label
	}

	bool omit_bounding_box() const {return flag(0, 1<<0);}
	void omit_bounding_box(bool value) {flag(0,1<<0,value);}

	/* TODO: bits 1-2  justification:     
			0  left-justified  
			1  right-justified  
			2  centred  
			*/

	ResJustification justification() const {return ResJustification(flag_value(0, 6) >> 1);}
	void justification(ResJustification value) {flag_value(0, 6, (int)value<<1);}

	const char *label() const {return message(36);}
	void label(const char *value) {message(36, value);}
	void label(const std::string &value) {message(36, value);}
};

}
}

#endif // TBX_RES_RESLABEL_H

