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

#include "writeablefield.h"
#include "pollinfo.h"
#include "swixcheck.h"
#include "textchangedlistener.h"

#include "kernel.h"

namespace tbx {

/**
 * Writeable field text change event
 */

class WriteableFieldTextChangedEvent : public TextChangedEvent
{
public:
	WriteableFieldTextChangedEvent(IdBlock &id_block, PollBlock &data) :
		TextChangedEvent(id_block, data)
	{

	}
	/// Virtual destructor
	virtual ~WriteableFieldTextChangedEvent() {};

	/**
	 * Override in sub class to fetch text from the gadget.
	 *
	 * Called from the text method if the text wasn't delivered in
	 * the event buffer.
	 */
	virtual std::string text_from_gadget() const
	{
		WriteableField source(id_block().self_component());
		return source.text();
	}
};

/*
 * handle writeable field changed event
 */
static void writeable_changed_handler(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	WriteableFieldTextChangedEvent event(id_block, data);
	static_cast<TextChangedListener *>(listener)->text_changed(event);
}

/**
 * Set the font used for the writeable field.
 *
 * @param name The name of the font
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */
void WriteableField::font(const std::string &name, int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 516;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(name.c_str() ));
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Set the writeable field to use the system font to display the text
 *
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */

void WriteableField::system_font(int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 516;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Add a listener to report when the text has changed in the
 * writeable field.
 *
 * The writeable field must have been set to deliver the default
 * event for this listener to run.
 */
void WriteableField::add_text_changed_listener(TextChangedListener *listener)
{
	add_listener(0x82885, listener, writeable_changed_handler);
}

/**
 * Remove a text changed listener.
 */
void WriteableField::remove_text_changed_listener(TextChangedListener *listener)
{
	remove_listener(0x82885, listener);
}


}
