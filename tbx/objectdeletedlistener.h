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
 * objectdeletedlistener.h
 *
 *  Created on: 16-Sep-2008
 *      Author: alanb
 */

#ifndef TBX_OBJECTDELETEDLISTENER_H_
#define TBX_OBJECTDELETEDLISTENER_H_

#include "listener.h"
#include "pollinfo.h"
#include "object.h"

namespace tbx
{

	class ObjectDeletedListener: public tbx::Listener {
	public:
		ObjectDeletedListener() {}
		virtual ~ObjectDeletedListener() {}

		virtual void object_deleted(Object &object) = 0;

	private:
		friend class Object;
		static void handler(IdBlock &id_block, PollBlock &data, Listener *listener);
	};

}

#endif /* TBX_OBJECTDELETEDLISTENER_H_ */
