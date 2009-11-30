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

#ifndef TBX_SHOWPOINTOBJECT_H
#define TBX_SHOWPOINTOBJECT_H

#include "object.h"
#include "menu.h"
#include "point.h"

namespace tbx {

/**
 * Object that can be shown at a specific position
 * as well as its default place.
 */
class ShowPointObject: public tbx::Object
{
public:
	ShowPointObject()  {}
	ShowPointObject(ObjectId handle) : Object(handle) {}
	ShowPointObject(const Object &other) : Object(other) {}
	ShowPointObject(const std::string &template_name) : Object(template_name) {}

	using Object::show;

	void show(const Object &parent);
	void show(const Component &parent);
	void show(const Point &pos);
	void show(const Point &pos, const Object &parent);
	void show(const Point &pos, const Component &parent);

	void show_as_menu();
	void show_as_menu(const Object &parent);
	void show_as_menu(const Component &parent);
	void show_as_menu(const Point &pos);
	void show_as_menu(const Point &pos, const Object &parent);
	void show_as_menu(const Point &pos, const Component &parent);

	void show_as_submenu(const MenuItem &parent);
	void show_as_submenu(const Point &pos, const MenuItem &parent);
};

}

#endif /* TBX_SHOWPOINTOBJECT_H */
