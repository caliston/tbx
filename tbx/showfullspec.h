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
 * showfullspec.h
 *
 *  Created on: 12-Sep-2008
 *      Author: alanb
 */

#ifndef SHOWFULLSPEC_H_
#define SHOWFULLSPEC_H_

#include "bbox.h"
#include "handles.h"
#include "VisibleArea.h"

namespace tbx
{
	struct ShowFullSpec
	{
	public:
		VisibleArea visible_area;
		WindowHandle wimp_window;

		enum {WINDOW_SHOW_TOP = -1, WINDOW_SHOW_BOTTOM = -2, WINDOW_SHOW_BEHIND = -3};
	};
}

#endif /* SHOWFULLSPEC_H_ */
