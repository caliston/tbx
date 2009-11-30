/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * gadget.cc
 *
 *  Created on: 12-Sep-2008
 *      Author: alanb
 */

#include "gadget.h"
#include "tbxexcept.h"
#include "swixcheck.h"
#include "swis.h"

using namespace tbx;

void Gadget::check_toolbox_class(int class_id) const
{
	if (_id == NULL_ComponentId) throw GadgetClassError();

	int tbox_class_id;
	swix_check(_swix(0x44ec6, _INR(0,3) | _OUT(0), 0,  _handle,
		70, _id, &tbox_class_id));

	// class is in lower 16 bits (top 16bits is size)
	if ((tbox_class_id  & 0xFFFF) != class_id) throw GadgetClassError();
}


unsigned int Gadget::flags() const
{
    _kernel_swi_regs regs;
    regs.r[0] = 0; // Flags are zero
    regs.r[1] = _handle;
    regs.r[2] = 64;
    regs.r[3] = _id;

    // Run Toolbox_ObjectMiscOp
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));

    return regs.r[0];
}


void Gadget::set_flag(unsigned int set)
{
	_kernel_swi_regs regs;

    regs.r[0] = 0; // Flags are zero
    regs.r[1] = _handle;
    regs.r[2] = 64;
    regs.r[3] = _id;

    // Run Toolbox_ObjectMiscOp - to get the current flags
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	regs.r[4] = regs.r[0] | set;
	regs.r[0] = 0;
	regs.r[2] = 65;
    // Run Toolbox_ObjectMiscOp - to set the current flags
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void Gadget::clear_flag(unsigned int clear)
{
	_kernel_swi_regs regs;

    regs.r[0] = 0; // Flags are zero
    regs.r[1] = _handle;
    regs.r[2] = 64;
    regs.r[3] = _id;

    // Run Toolbox_ObjectMiscOp - to get the current flags
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	regs.r[4] = regs.r[0] & ~clear;
	regs.r[0] = 0;
	regs.r[2] = 65;
    // Run Toolbox_ObjectMiscOp - to set the current flags
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void Gadget::fade()
{
	set_flag(1<<31);
}

void Gadget::unfade()
{
	clear_flag(1<<31);
}

void Gadget::fade(bool fade)
{
	if (fade) set_flag(1<<31);
	else clear_flag(1<<31);
}


bool Gadget::faded() const
{
	return (flags() & (1 << 31)) != 0;
}

/**
 * Set input focus to the gadget.
 *
 * The gadget must be a writeable field or a composite gadget
 * that includes a writeable field.
 */
void Gadget::focus()
{
	_kernel_swi_regs regs;

    regs.r[0] = 0; // Flags are zero
    regs.r[1] = _handle;
    regs.r[2] = 69;
    regs.r[3] = _id;

    // Run Toolbox_ObjectMiscOp - to get the current flags
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}
