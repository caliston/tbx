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

#ifndef TBX_RES_RESSCALE_H
#define TBX_RES_RESSCALE_H

#include "ResObject.h"

namespace tbx {
namespace res {

/**
 * Class for Scale object template
 */
class ResScale : public ResObject
{

public:
	enum {CLASS_ID = 0x82c00 };

	/**
	 * Construct a scale resource from a ResObject
	 *
	 * @param other ResObject to construct from
	 * @throws ResObjectClassMismatch if the ResObject is not a scale resource
	 */
	ResScale(const ResObject &other) : ResObject(other)
	{
     check_class_id(CLASS_ID);
	}

	/**
	 * Construct a scale resource by copying another
	 *
	 * @param other ResScale to copy
	 */
	ResScale(const ResScale &other) : ResObject(other)
	{
	}

	virtual ~ResScale() {}

	/**
	 * Assign from a ResObject
	 *
	 * @param other ResObject to copy
	 * @throws ResObjectClassMismatch if the ResObject is not a scale resource
	 */
	ResScale &operator=(const ResObject &other)
	{
		other.check_class_id(CLASS_ID);
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Assign from another scale resource
	 *
	 * @param other button gadget resource to copy
	 */
	ResScale &operator=(const ResScale &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Construct a scale resource
	 *
	 * All options are false, events unset and messages are null.
	 *
	 * @param name the name of this object (maximum 12 characters)
	 */
	ResScale(std::string name)
	  : ResObject(name, CLASS_ID, 100, 36+44)
	{
		min_val(10);
		max_val(400);
		step_size(1);
		init_message(16,0); // title
		init_string(24,0); // window
		std1_value(33);
		std2_value(80);
		std3_value(100);
		std4_value(120);
	}


	int flags() const {return int_value(0);}
	void flags(int value) {int_value(0,value);}
	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}
	bool generate_dialogue_completed() const {return flag(0, 1<<1);}
	void generate_dialogue_completed(bool value) {flag(0,1<<1,value);}
	bool has_scale_to_fit() const {return flag(0, 1<<2);}
	void has_scale_to_fit(bool value) {flag(0,1<<2,value);}

	int min_val() const {return int_value(4);}
	void min_val(int value) {int_value(4,value);}
	int max_val() const {return int_value(8);}
	void max_val(int value) {int_value(8,value);}
	int step_size() const {return int_value(12);}
	void step_size(int value) {int_value(12,value);}
	const char *title() const {return message(16);}
	void title(const char *value, int max_length = -1) {message_with_length(16, value, max_length);}
	void title(const std::string &value, int max_length = -1) {message_with_length(16, value, max_length);}
	int max_title() const {return int_value(20);}
	const char *window() const {return string(24);}
	void window(const char *value) {string(24, value);}
	void window(const std::string &value) {string(24, value);}
	int std1_value() const {return int_value(28);}
	void std1_value(int value) {int_value(28,value);}
	int std2_value() const {return int_value(32);}
	void std2_value(int value) {int_value(32,value);}
	int std3_value() const {return int_value(36);}
	void std3_value(int value) {int_value(36,value);}
	int std4_value() const {return int_value(40);}
	void std4_value(int value) {int_value(40,value);}

};

}
}

#endif // TBX_RES_RESSCALE_H

/** Dump routine
void dump_resscale(std::ostream &os, const ResObject &obj)
{
	ResScale g(obj);

	os << MARGIN << MARGIN << "Flags: " << g.flags() << endl;
	os << MARGIN << MARGIN << "Flags" << endl;
	if (g.generate_about_to_be_shown())	os << MARGIN << MARGIN << " generate_about_to_be_shown" << endl;
	if (g.generate_dialogue_completed())	os << MARGIN << MARGIN << " generate_dialogue_completed" << endl;
	if (g.has_scale_to_fit())	os << MARGIN << MARGIN << " has_scale_to_fit" << endl;
	os << MARGIN << MARGIN << "Min_val: " << g.min_val() << endl;
	os << MARGIN << MARGIN << "Max_val: " << g.max_val() << endl;
	os << MARGIN << MARGIN << "Step_size: " << g.step_size() << endl;
	os << MARGIN << MARGIN << "Title: " << string_or_null(g.title()) << endl;
	os << MARGIN << MARGIN << "Max_title: " << g.max_title() << endl;
	os << MARGIN << MARGIN << "Window: " << string_or_null(g.window()) << endl;
	os << MARGIN << MARGIN << "Std1_value: " << g.std1_value() << endl;
	os << MARGIN << MARGIN << "Std2_value: " << g.std2_value() << endl;
	os << MARGIN << MARGIN << "Std3_value: " << g.std3_value() << endl;
	os << MARGIN << MARGIN << "Std4_value: " << g.std4_value() << endl;
}
**/
