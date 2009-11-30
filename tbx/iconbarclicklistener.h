/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2009 Alan Buckley   All Rights Reserved.
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

#ifndef TBX_ICONBARCLICKLISTENER_H_
#define TBX_ICONBARCLICKLISTENER_H_

#include "listener.h"
#include "iconbar.h"

namespace tbx {

/**
 * Iconbar as been clicked event details
 */
class IconbarClickEvent
{
private:
	Iconbar _iconbar;
	int _flags;
public:
	/**
	 * Construct the event.
	 */
	IconbarClickEvent(Iconbar iconbar, int flags) : _iconbar(iconbar), _flags(flags) {};

	/**
	 * Return the source of the event.
	 */
	Iconbar source() {return _iconbar;}

	/**
	 * Check if click was with adjust
	 */
	bool adjust() const {return (_flags & 1)!=0;}

	/**
	 * Check if click was with select
	 */
	bool select() const {return (_flags & 4)!=0;}
};

/**
 * Listener for button selected events
 */
class IconbarClickListener: public tbx::Listener {
public:
	virtual void iconbar_clicked(IconbarClickEvent &event) = 0;
};

}

#endif
