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

#ifndef TBX_RES_RESNUMBERRANGE_H
#define TBX_RES_RESNUMBERRANGE_H

#include "resgadget.h"
#include "resjustification.h"

namespace tbx {
namespace res {

/**
 * Class for NumberRange gadget template
 */
class ResNumberRange : public ResGadget
{

public:
	enum {TYPE_ID = 832 };

	ResNumberRange(const ResGadget &other) : ResGadget(other)
	{
     check_type(TYPE_ID);
	}

	ResNumberRange(const ResNumberRange &other) : ResGadget(other)
	{
	}

	virtual ~ResNumberRange() {}

	ResNumberRange &operator=(const ResGadget &other)
	{
		other.check_type(TYPE_ID);
		ResBase::operator=(other);
		return *this;
	}

	ResNumberRange &operator=(const ResNumberRange &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	ResNumberRange()
	  : ResGadget(832,68)
	{
		slider_colour(4);
		step_size(1);
		before(-1);
		after(-1);
	}

	bool generate_value_changed() const {return flag(0, 1<<0);}
	void generate_value_changed(bool value) {flag(0,1<<0,value);}
	bool writable() const {return flag(0, 1<<2);}
	void writable(bool value) {flag(0,1<<2,value);}
	bool no_display_area() const {return flag(0, 1<<3);}
	void no_display_area(bool value) {flag(0,1<<3,value);}
	bool has_adjuster_arrows() const {return flag(0, 1<<4);}
	void has_adjuster_arrows(bool value) {flag(0,1<<4,value);}
	/**
	 * Get/Set slider_type
   * TODO: Make following an enum
	 * Values:
	*  0 no slider
	*  1 slider to the right of the display area
	*  2 slider to the left of the display area
	*/
	int slider_type() const {return flag_value(0, 224)>>5;}
	void slider_type(int value) {flag_value(0,224,value<<5);}
	/**
	 * Get/Set justification
   * TODO: Make following an enum
	 * Values:
	*  0 left-justified
	*  1 right-justified
	*  2 centred
	*/
	ResJustification justification() const {return ResJustification(flag_value(0, 768)>>8);}
	void justification(ResJustification value) {flag_value(0,768,(int)value<<8);}
	/**
	 * Get/Set slider_colour
   * TODO: usw WimpColour
	*/
	int slider_colour() const {return flag_value(0, 61440)>>12;}
	void slider_colour(int value) {flag_value(0,61440,value<<12);}
	/**
	 * Get/Set slider_background_colour
   * TODO: use WimpColour
	*/
	int slider_background_colour() const {return flag_value(0, 983040)>>16;}
	void slider_background_colour(int value) {flag_value(0,983040,value<<16);}

	int lower_bound() const {return int_value(36);}
	void lower_bound(int value) {int_value(36,value);}
	int upper_bound() const {return int_value(40);}
	void upper_bound(int value) {int_value(40,value);}
	int step_size() const {return int_value(44);}
	void step_size(int value) {int_value(44,value);}
	int initial_value() const {return int_value(48);}
	void initial_value(int value) {int_value(48,value);}
	int precision() const {return int_value(52);}
	void precision(int value) {int_value(52,value);}
	int before() const {return int_value(56);}
	void before(int value) {int_value(56,value);}
	int after() const {return int_value(60);}
	void after(int value) {int_value(60,value);}
	int display_length() const {return int_value(64);}
	void display_length(int value) {int_value(64,value);}

};

}
}

#endif // TBX_RES_RESNUMBERRANGE_H

