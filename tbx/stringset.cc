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
 * stringset.cc
 *
 *  Created on: 28-Nov-2008
 *      Author: alanb
 */

#include "stringset.h"
#include "swixcheck.h"
#include "textchangedlistener.h"
#include <swis.h>

namespace tbx {


/**
 * StringSet text change event
 */

class StringSetTextChangedEvent : public TextChangedEvent
{
private:
	StringSet _source;

public:
	StringSetTextChangedEvent(StringSet source, bool too_long, const std::string & text) :
		TextChangedEvent(too_long, text), _source(source)
	{

	}
	/// Virtual destructor
	virtual ~StringSetTextChangedEvent() {};

	/**
	 * The gadget that generated the event.
	 */
	virtual Gadget &source() {return _source;}

	/**
	 * Override in sub class to fetch text from the gadget.
	 *
	 * Called from the text method if the text wasn't delivered in
	 * the event buffer.
	 */
	virtual std::string text_from_gadget() const {return _source.selected();}
};

/*
 * handle StringSet changed event
 */
static void stringset_changed_handler(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	StringSet source(id_block.self_component());
	bool too_long =((data.word[3] & 1) == 0);
	std::string text;
	if (!too_long) text = reinterpret_cast<const char *>(data.word +4);
	StringSetTextChangedEvent event(source, too_long, text);
	static_cast<TextChangedListener *>(listener)->text_changed(event);
}


void StringSet::selected_index(int index)
{
   // Run Toolbox_ObjectMiscOp
	swix_check(_swix(0x44ec6, _INR(0,4), 1, _handle, 898, _id, index));
}

int StringSet::selected_index() const
{
   // Run Toolbox_ObjectMiscOp
	int index;
	swix_check(_swix(0x44ec6, _INR(0,3)|_OUT(0), 1, _handle, 899, _id, &index));
	return index;
}

/**
 * Add a listener to report when the text has changed in the string set
 *
 * The string set must have been set to deliver the default
 * event for this listener to run.
 */
void StringSet::add_text_changed_listener(TextChangedListener *listener)
{
	add_listener(0x8288e, listener, stringset_changed_handler);
}

/**
 * Remove a text changed listener.
 */
void StringSet::remove_text_changed_listener(TextChangedListener *listener)
{
	remove_listener(0x8288e, listener);
}


}

