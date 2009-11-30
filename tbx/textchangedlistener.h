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

#ifndef TBX_TEXTCHANGEDLISTENER_H_
#define TBX_TEXTCHANGEDLISTENER_H_

#include "listener.h"
#include "Gadget.h"

namespace tbx
{
    /**
     * Event details for a change of text in a gadget.
     *
     * @see TextChangedListener
     */
	class TextChangedEvent
	{
		bool _too_long;
		std::string _text;

	public:
		/**
		 * Contstruct the event.
		 *
		 * @param source The gadget that generated the event
		 * @param too_long true if the changed text was too long to be
		 *        delivered with the event.
		 * @param text The text for the event if it was delivered with the event.
		 */
		TextChangedEvent(bool too_long, std::string text) :
			_too_long(too_long), _text(text) {};

		/**
		 * Virtual destructor
		 */
		virtual ~TextChangedEvent() {};


		/**
		 * Check if new text was delivered with the toolbox event.
		 *
		 * @returns true If the gadget contained more text than would
		 *          fit in the event delivery buffer.
		 */
		bool too_long() const {return _too_long;}

		/**
		 * Get the text of the gadget that delivered the event.
		 *
		 * This will be the text delivered with the event if the too_long
		 * parameter is false. Otherwise the text will be retrieved from
		 * the gadget.
		 *
		 * @returns the gadget text.
		 */
		std::string text() const {return (_too_long) ? text_from_gadget(): _text;}

		/**
		 * The gadget that generated the event.
		 */
		virtual Gadget &source() = 0;

	protected:
		/**
		 * Override in sub class to fetch text from the gadget.
		 *
		 * Called from the text method if the text wasn't delivered in
		 * the event buffer.
		 */
		virtual std::string text_from_gadget() const = 0;
	};

	/**
	 * Listener for text changed events
	 */
	class TextChangedListener : public Listener
	{
	public:
		/**
		 * The text has been changed.
		 */
		virtual void text_changed(TextChangedEvent &event) = 0;
	};
}

#endif /* TBX_TEXTCHANGEDLISTENER_H_ */
