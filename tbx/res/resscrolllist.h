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

#ifndef TBX_RES_RESSCROLLLIST_H
#define TBX_RES_RESSCROLLLIST_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for ScrollList gadget template
 */
class ResScrollList : public ResGadget
{

public:
	enum {TYPE_ID = 0x401A };

	ResScrollList(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResScrollList(const ResScrollList &other) : ResGadget(other)
	{
	}

	virtual ~ResScrollList() {}

	ResScrollList &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResScrollList &operator=(const ResScrollList &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResScrollList()
	  : ResGadget(0x401A,48)
	{
		background(0xffffff00); // White background
	}


	int event() const {return int_value(36);}
	void event(int value) {int_value(36,value);}
	int foreground() const {return int_value(40);}
	void foreground(int value) {int_value(40,value);}
	int background() const {return int_value(44);}
	void background(int value) {int_value(44,value);}

};

}
}

#endif // TBX_RES_RESSCROLLLIST_H

