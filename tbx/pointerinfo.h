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
 * pointerinfo.h
 *
 *  Created on: 4 Nov 2010
 *      Author: alanb
 */

#ifndef TBX_POINTERINFO_H_
#define TBX_POINTERINFO_H_

#include "gadget.h"

namespace tbx {

/**
 * Class to store and fetch information about the
 * WIMP mouse pointer
 */
class PointerInfo
{
	int _block[5];

public:
	PointerInfo(bool update_now = true, bool get_objects = true);
	PointerInfo(WindowHandle window_handle, IconHandle icon_handle, int dest_x, int dest_y, int buttons);
	virtual ~PointerInfo();

	void Update(bool get_objects = true);

	// Properties
	int mouse_x() const		{return _block[0];}
	int mouse_y() const		{return _block[1];}
	int button_state() const	{return _block[2];}

	// More details
	bool adjust_down() const	{return (_block[2] & 1)!=0;}
	bool menu_down() const		{return (_block[2] & 2)!=0;}
	bool select_down() const	{return (_block[2] & 4)!=0;}

	// Object/component hands
	bool on_object() const	{return (_block[2] & 256)!=0;}
	Object object() const;
	Gadget gadget() const;

	// WIMP window/icon handles
	WindowHandle window_handle() const {return _block[3];}
	IconHandle icon_handle() const	{return _block[4];}
	bool user_window() const	{return (_block[3] >= 0);}
	bool on_background() const {return (_block[3] == -1);}
	bool on_iconbar() const	{return (_block[3] == -2);}

	bool system_icon() const	{return (_block[4] < 0);}

	// TODO: define enum to give system icon numbers

};

}

#endif /* TBX_POINTERINFO_H_ */
