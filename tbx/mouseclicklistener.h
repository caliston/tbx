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


#ifndef MOUSECLICKLISTENER_H_
#define MOUSECLICKLISTENER_H_

#include "gadget.h"
#include "listener.h"

namespace tbx
{
	/**
	 * Event for click on a window.
	 */
	class MouseClickEvent
	{
		Window _window;
		Gadget _gadget;
		Point _point;
		unsigned int _button;
		unsigned int _click_shift;

	public:
		/**
		 * Constructor
		 */
		MouseClickEvent(Window &window, Gadget _gadget, Point &pt, unsigned int b, bool d) :
			_window(window), _point(pt), _button(b)
		{
			if (d) _click_shift = 8;
			else _click_shift = 0;
		};

		/**
		 * Window mouse click occurred upon
		 */
		Window window() const {return _window;}

		/**
		 * Gadget click occurred up.
		 *
		 * Returned gadget will be null if clicked on window background.
		 */
		Gadget gadget() const {return _gadget;}

		/**
		 * Point of the click in screen coordinates
		 */
		const Point &point() const {return _point;}

		/**
		 * Button pressed.
		 *
		 * Depends on the windows button flags.
		 * See the RISC OS Wimp_CreateIcon entry in the PRM for details.
		 *
		 * The methods below take into account the icon/window button flags
		 * and return a consistent result.
		 */
		unsigned int button() const {return _button;}

		/**
		 * Adjust button has been clicked.
		 */
		bool is_adjust() const {return (_button & (1 << _click_shift)) != 0;}

		/**
		 * Menu button has been clicked
		 */
		bool is_menu() const {return (_button & 2) != 0;}

		/**
		 * Select button has been clicked.
		 */
		bool is_select() const {return (_button & (4 << _click_shift)) != 0;}

		/**
		 * Drag started with adjust.
		 *
		 * Button types 6 to 11 only
		 */
		bool is_adjust_drag() const {return (_button & 0x10) != 0;}

		/**
		 * Drag start with select.
		 *
		 * Button types 6 to 11 only.
		 */
		bool is_select_drag() const {return (_button & 0x40) != 0;}

		/**
		 * Click is a double adjust click.
		 *
		 * This only applies to window button type 5 & 10.
		 */
		bool is_adjust_double() const {return (_click_shift != 0) && (_button & 0x1) != 0;}

		/**
		 * Click is a double select click.
		 *
		 * This only applies to window button type 5 & 10.
		 */
		bool is_select_double() const {return (_click_shift != 0) && (_button & 0x4) != 0;}
	};

	/**
	 * Listener for mouse clicks on a Window
	 */
	class MouseClickListener : public Listener
	{
	public:
		/** Destructor */
		virtual ~MouseClickListener() {};

		/**
		 * Override to process the mouse click.
		 *
		 * @param event details of the type of mouse click
		 *         and where it occurred.
		 */
		virtual void mouse_click(MouseClickEvent &event) = 0;
	};
}

#endif /* MOUSECLICKLISTENER_H_ */
