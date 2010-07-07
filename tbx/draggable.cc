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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * draggable.cc
 *
 *  Created on: 6-Jul-2010
 *      Author: alanb
 */

#include "draggable.h"

namespace tbx
{

static void draggable_drag_started_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	DragStartedEvent ev(id_block, data);
	static_cast<DragStartedListener *>(listener)->drag_started(ev);
}

/**
 * Add listener for start of drag.
 *
 * A flag must be set in the draggable for this event to be generated
 */
void Draggable::add_drag_started_listener(DragStartedListener *listener)
{
	add_listener(0x82887, listener, draggable_drag_started_router);
}

/**
 * Remove listener for start of drag
 */
void Draggable::remove_drag_started_listener(DragStartedListener *listener)
{
	remove_listener(0x82887, listener);
}

static void draggable_drag_ended_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	DragEndedEvent ev(id_block, data);
	static_cast<DragEndedListener *>(listener)->drag_ended(ev);
}

/**
 * Add listener for end of drag
 */
void Draggable::add_drag_ended_listener(DragEndedListener *listener)
{
	add_listener(0x82888, listener, draggable_drag_ended_router);
}

/**
 * Remove listener for end of drag
 */
void Draggable::remove_drag_ended_listener(DragEndedListener *listener)
{
	remove_listener(0x82888, listener);
}

}
