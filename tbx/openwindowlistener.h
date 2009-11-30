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


#ifndef OPENWINDOWLISTENER_H_
#define OPENWINDOWLISTENER_H_

#include "bbox.h"
#include "handles.h"
#include "listener.h"
#include "window.h"

namespace tbx
{
	/**
	 * Event data for open window request listener
	 */
	class OpenWindowEvent
	{
	private:
		Window _window;
		WindowHandle _handle;
		BBox _visible_area;
		Point _scroll;
		WindowHandle _behind;

		//TODO: Mechanism for updating details

	public:
		OpenWindowEvent(Window window, PollBlock &poll_block) :
			_window(window),
			_handle(poll_block.word[0]),
			_visible_area(poll_block.word[1], poll_block.word[2], poll_block.word[3], poll_block.word[4]),
			_scroll(poll_block.word[5], poll_block.word[6]),
			_behind(poll_block.word[7]) {}

		/**
		 * Get Window that generated this event
		 */
		Window window() const {return _window;}

		/**
		 * Wimp window handle of window being opened
		 */
		WindowHandle handle() const {return _handle;}

		/**
		 * Visible area on screen
		 */
		const BBox &visible_area() const {return _visible_area;}

		/**
		 * Scroll offsets in work area
		 */
		const Point &scroll() const {return _scroll;}

		/**
		 * Wimp Window handle opened behind
		 */
		WindowHandle behind() const {return _behind;}
	};

	/**
	 * Listener for Wimp Open Window Request events
	 */
	class OpenWindowListener : public Listener
	{
	public:
		/**
		 * virtual function called when the application receives the open window
		 * request for the window.
		 *
		 * Note: The framework calls Wimp_OpenWindow automatically after
		 * all OpenWindowListeners are called for a window so do no call
		 * it in this routine.
		 *
		 * @param event details of the open request
		 */
		virtual void open_window(OpenWindowEvent &event) = 0;
	};
}

#endif /* WINDOWOPENLISTENER_H_ */
