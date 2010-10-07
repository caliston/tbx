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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TBX_RES_RESDRAGGABLE_H
#define TBX_RES_RESDRAGGABLE_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for Draggable gadget template
 */
class ResDraggable : public ResGadget
{

public:
	enum {TYPE_ID = 640 };

	ResDraggable(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResDraggable(const ResDraggable &other) : ResGadget(other)
	{
	}

	virtual ~ResDraggable() {}

	ResDraggable &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResDraggable &operator=(const ResDraggable &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResDraggable()
	  : ResGadget(640,52)
	{
		int_value(0, 0x80); // drop_shadow
		init_message(36,0); // text
		init_string(44,0); // sprite
	}

	bool generate_drag_started() const {return flag(0, 1<<0);}
	void generate_drag_started(bool value) {flag(0,1<<0,value);}
	bool has_sprite() const {return flag(0, 1<<1);}
	void has_sprite(bool value) {flag(0,1<<1,value);}
	bool has_text() const {return flag(0, 1<<2);}
	void has_text(bool value) {flag(0,1<<2,value);}
	/**
	 * Get/Set type
   * TODO: Make following an enum
	 * Values:
	*  0 drag only
	*  1 click, drag, doubleclick
	*  2 click selects, doubleclick, drag
	*/
	int draggable_type() const {return flag_value(0, 56)>>3;}
	void draggable_type(int value) {flag_value(0,56,value<<3);}
	bool deliver_toolbox_ids() const {return flag(0, 1<<6);}
	void deliver_toolbox_ids(bool value) {flag(0,1<<6,value);}
	bool drop_shadow() const {return flag(0, 1<<7);}
	void drop_shadow(bool value) {flag(0,1<<7,value);}
	bool not_dithered() const {return flag(0, 1<<8);}
	void not_dithered(bool value) {flag(0,1<<8,value);}

	const char *text() const {return message(36);}
	void text(const char *value, int max_length = -1) {message_with_length(36, value, max_length);}
	void text(const std::string &value, int max_length = -1) {message_with_length(36, value, max_length);}
	int max_text_len() const {return int_value(40);}
	const char *sprite() const {return string(44);}
	void sprite(const char *value, int max_length = -1) {string_with_length(44, value, max_length);}
	void sprite(const std::string &value, int max_length = -1) {string_with_length(44, value, max_length);}
	int max_sprite_len() const {return int_value(48);}

};

}
}

#endif // TBX_RES_RESDRAGGABLE_H

