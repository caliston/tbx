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
#include "numberrange.h"

#include "valuechangedlistener.h"
#include "pollinfo.h"

namespace tbx {

void value_changed_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
 	 ValueChangedEvent event(id_block, data);
 	 static_cast<ValueChangedListener *>(listener)->value_changed(event);
}


/**
 * Add listener for when the value of the number range changes
 *
 * The default value change event must have been enabled in the resources
 * for this number range.
 */
void NumberRange::add_value_changed_listener(ValueChangedListener *listener)
{
	add_listener(0x8288d, listener, value_changed_router);
}

/**
 * Remove listener for when the value of the number range changes
 */
void NumberRange::remove_value_changed_listener(ValueChangedListener *listener)
{
	remove_listener(0x8288d, listener);
}

}
