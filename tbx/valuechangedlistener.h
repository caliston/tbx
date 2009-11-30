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

#ifndef TBX_VALUECHANGEDLISTENER_H_
#define TBX_VALUECHANGEDLISTENER_H_

#include "listener.h"
#include "numberrange.h"

//TODO: This event should be generalised for more gadgets to use.

namespace tbx
{
    /**
     * Event details for a change of an integer value in a gadget.
     *
     * @see ValueChangedListener
     */
	class ValueChangedEvent
	{
		NumberRange _source;
		int _value;

	public:
		/**
		 * Contstruct the event.
		 *
		 * @param source The gadget that generated the event
		 * @param value The new value in the gadget
		 */
		ValueChangedEvent(NumberRange &source, int value) :
			_source(source), _value(value) {};

		/**
		 * The gadget that generated the event.
		 */
		NumberRange &source() {return _source;}

		/**
		 * Get the value of the gadget that delivered the event.
		 *
		 * @returns the gadget value.
		 */
		int value() const {return _value;}
	};

	/**
	 * Listener for value changed events
	 */
	class ValueChangedListener : public Listener
	{
	public:
		/**
		 * The value has been changed.
		 */
		virtual void value_changed(ValueChangedEvent &event) = 0;
	};
}

#endif /* TBX_VALUECHANGEDLISTENER_H_ */
