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

#ifndef TBX_RES_RESDISPLAYFIELD_H
#define TBX_RES_RESDISPLAYFIELD_H

#include "resgadget.h"
#include "resjustification.h"

namespace tbx {
namespace res {

/**
 * Class for DisplayField gadget template
 */
class ResDisplayField : public ResGadget
{

public:
	enum {TYPE_ID = 448 };

	ResDisplayField(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResDisplayField(const ResDisplayField &other) : ResGadget(other)
	{
	}

	virtual ~ResDisplayField() {}

	ResDisplayField &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResDisplayField &operator=(const ResDisplayField &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResDisplayField()
	  : ResGadget(448,44)
	{
		init_message(36,0); // text
	}

	ResJustification justification() const {return ResJustification(flag_value(0,6)>>1);}
	void justification(ResJustification value) {flag_value(0,6, (int)value << 1);}

	const char *text() const {return message(36);}
	void text(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void text(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_text_len() const {return int_value(40);}

};

}
}

#endif // TBX_RES_RESDISPLAYFIELD_H

