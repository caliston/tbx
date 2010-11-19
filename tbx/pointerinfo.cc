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

/*
 * pointerinfo.cc
 *
 *  Created on: 4 Nov 2010
 *      Author: alanb
 */

#include "pointerinfo.h"
#include "kernel.h"

namespace tbx {

PointerInfo::PointerInfo(bool update_now /*= true */, bool get_objects /* = true */)
{
	if (update_now) Update(get_objects);
}

PointerInfo::PointerInfo(WindowHandle window_handle, IconHandle icon_handle, int dest_x, int dest_y, int buttons)
{
	_block[3] = window_handle;
	_block[4] = icon_handle;
	_block[0] = dest_x;
	_block[1] = dest_y;
	_block[2] = buttons;
}

PointerInfo::~PointerInfo()
{

}

void PointerInfo::Update(bool get_objects /* = true */)
{
	_kernel_swi_regs regs;

	if (get_objects)
	{
		regs.r[0] = 0;
		_kernel_swi(0x82883, &regs, &regs);
		_block[0] = regs.r[0];
		_block[1] = regs.r[1];
		_block[2] = regs.r[2];
		_block[3] = regs.r[3];
		_block[4] = regs.r[4];
	} else
	{
		regs.r[1] = (int)_block;
		_kernel_swi(0x400CF, &regs, &regs);
	}
}

Object PointerInfo::object() const
{
	Object obj;
	if (_block[2] & 256) obj = Object(_block[3]);
	return obj;
}

Gadget PointerInfo::gadget() const
{
	Gadget gadget;
    if ((_block[2] & 256) != 0 && _block[4] != -1)
    {
        Window window(_block[3]);
        gadget = window.gadget(_block[4]);
    }

	return gadget;
}

}
