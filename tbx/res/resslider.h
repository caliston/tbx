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

#ifndef TBX_RES_RESSLIDER_H
#define TBX_RES_RESSLIDER_H

#include "resgadget.h"

namespace tbx {
namespace res {

/**
 * Class for Slider gadget template
 */
class ResSlider : public ResGadget
{

public:
	enum {TYPE_ID = 576 };

	ResSlider(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResSlider(const ResSlider &other) : ResGadget(other)
	{
	}

	virtual ~ResSlider() {}

	ResSlider &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResSlider &operator=(const ResSlider &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResSlider()
	  : ResGadget(576,52)
	{
		int_value(0, 0x4010); // bar colour 4, is dragggable
		upper_bound(100);
		step_size(1);
	}

	bool generate_value_changed() const {return flag(0, 1<<0);}
	void generate_value_changed(bool value) {flag(0,1<<0,value);}
	bool generate_value_changed_while_dragging() const {return flag(0, 1<<1);}
	void generate_value_changed_while_dragging(bool value) {flag(0,1<<1,value);}
	bool vertical() const {return flag(0, 1<<3);}
	void vertical(bool value) {flag(0,1<<3,value);}
	bool bar_is_draggable() const {return flag(0, 1<<4);}
	void bar_is_draggable(bool value) {flag(0,1<<4,value);}
	/**
	 * Get/Set bar_colour
   * TODO: Make following an enum
	 * Values:
	*/
	int bar_colour() const {return flag_value(0, 61440)>>12;}
	void bar_colour(int value) {flag_value(0,61440,value<<12);}
	/**
	 * Get/Set background_colour
   * TODO: Make following an enum
	 * Values:
	*/
	int background_colour() const {return flag_value(0, 983040)>>16;}
	void background_colour(int value) {flag_value(0,983040,value<<16);}

	int lower_bound() const {return int_value(36);}
	void lower_bound(int value) {int_value(36,value);}
	int upper_bound() const {return int_value(40);}
	void upper_bound(int value) {int_value(40,value);}
	int step_size() const {return int_value(44);}
	void step_size(int value) {int_value(44,value);}
	int initial_value() const {return int_value(48);}
	void initial_value(int value) {int_value(48,value);}

};

}
}

#endif // TBX_RES_RESSLIDER_H

