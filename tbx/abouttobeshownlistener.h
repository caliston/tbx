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

#ifndef TBX_ABOUTTOBESHOWNLISTENER_H
#define TBX_ABOUTTOBESHOWNLISTENER_H

#include "listener.h"
#include "showfullspec.h"
#include "pollinfo.h"

namespace tbx
{
	class AboutToBeShownEvent
	{
	public:
		AboutToBeShownEvent(IdBlock &id_block, PollBlock &data) :
			_id_block(id_block), _data(data)
		{
		}

		enum ShowType {SHOW_DEFAULT = 0, SHOW_FULL_SPEC = 1, SHOW_TOP_LEFT = 2};
		ShowType show_type() const {return ShowType(_data.word[4]);}

		Point &top_left() const {return reinterpret_cast<Point &>(_data.word[5]);}
		Point &top_left() {return reinterpret_cast<Point &>(_data.word[5]);}

		ShowFullSpec &full_spec() {return reinterpret_cast<ShowFullSpec &>(_data.word[5]);}

		IdBlock id_block() {return _id_block;}

	private:
		IdBlock &_id_block;
		PollBlock &_data;
	};

	class AboutToBeShownListener : public Listener
	{
	public:
		AboutToBeShownListener() {}
		virtual ~AboutToBeShownListener() {}

		virtual void about_to_be_shown(AboutToBeShownEvent &event) = 0;
	};

	void about_to_be_shown_router(IdBlock &id_block, PollBlock &data, Listener *listener);
}

#endif

