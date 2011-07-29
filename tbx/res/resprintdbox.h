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

#ifndef TBX_RES_RESPRINTDBOX_H
#define TBX_RES_RESPRINTDBOX_H

#include "ResObject.h"

namespace tbx {
namespace res {

/**
 * Class for PrintDbox object template
 */
class ResPrintDbox : public ResObject
{

public:
	enum {CLASS_ID = 0x82b00 };

	/**
	 * Construct a print dialogue resource from a ResObject
	 *
	 * @param other ResObject to construct from
	 * @throws ResObjectClassMismatch if the ResObject is not a print dialogue resource
	 */
	ResPrintDbox(const ResObject &other) : ResObject(other)
	{
     check_class_id(CLASS_ID);
	}

	/**
	 * Construct a print dialogue resource by copying another
	 *
	 * @param other ResPrintDbox to copy
	 */
	ResPrintDbox(const ResPrintDbox &other) : ResObject(other)
	{
	}

	virtual ~ResPrintDbox() {}

	/**
	 * Assign from a ResObject
	 *
	 * @param other ResObject to copy
	 * @throws ResObjectClassMismatch if the ResObject is not a print dialogue resource
	 */
	ResPrintDbox &operator=(const ResObject &other)
	{
		other.check_class_id(CLASS_ID);
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Assign from another print dialogue resource
	 *
	 * @param other button gadget resource to copy
	 */
	ResPrintDbox &operator=(const ResPrintDbox &other)
	{
		ResBase::operator=(other);
		return *this;
	}

	/**
	 * Construct a print dialogue resource
	 *
	 * All options are false, events unset and messages are null.
	 *
	 * @param name the name of this object (maximum 12 characters)
	 */
	ResPrintDbox(std::string name)
	  : ResObject(name, CLASS_ID, 100, 36+28)
	{
		flags(0x2f8); // has page range, copies, scale, orientation, save, draft
		from(1);
		to(1);
		copies(1);
		scale(100);
		init_string(20,0); // further_options
		init_string(24,0); // window
	}


	/**
	 * Raw access to all the object flags.
	 *
	 * It is recommended you use the other methods in this
	 * class rather than use this directly.
	 *
	 * @returns integer with individual bits specifying various flags
	 */
	int flags() const {return int_value(0);}
	/**
	 * Set all the flags
	 *
	 * It is recommended you use the other methods in this
	 * class rather than use this directly.
	 *
	 * @param value the new flags
	 */
	void flags(int value) {int_value(0,value);}
	/**
	 * Check if the about to be shown event should be generated
	 *
	 * @returns true if the about to be shown event should be generated
	 */
	bool generate_about_to_be_shown() const {return flag(0, 1<<0);}
	/**
	 * Set if the about to be shown event should be generated
	 *
	 * @param value set to true if the about to be shown event should be generated
	 */
	void generate_about_to_be_shown(bool value) {flag(0,1<<0,value);}
	/**
	 * Check if the dialogue completed event should be generated
	 *
	 * @returns true if the dialogue completed event should be generated
	 */
	bool generate_dialogue_completed() const {return flag(0, 1<<1);}
	/**
	 * Set if the dialogue completed event should be generated
	 *
	 * @param value set to true if the dialogue completed event should be generated
	 */
	void generate_dialogue_completed(bool value) {flag(0,1<<1,value);}
	/**
	 * Check if an event should be generated when the printer setup dialogue
	 * is about to be shown.
	 *
	 * @returns true the setup about to be shown event will be generated
	 */
	bool generate_setup_about_to_be_shown() const {return flag(0, 1<<2);}
	/**
	 * Set if an event should be generated when the printer setup dialogue
	 * is about to be shown.
	 *
	 * @param value set to true if the setup about to be shown event should be generated
	 */
	void generate_setup_about_to_be_shown(bool value) {flag(0,1<<2,value);}
	bool has_page_range() const {return flag(0, 1<<3);}
	void has_page_range(bool value) {flag(0,1<<3,value);}
	bool has_copies() const {return flag(0, 1<<4);}
	void has_copies(bool value) {flag(0,1<<4,value);}
	bool has_scale() const {return flag(0, 1<<5);}
	void has_scale(bool value) {flag(0,1<<5,value);}
	bool has_orientation() const {return flag(0, 1<<6);}
	void has_orientation(bool value) {flag(0,1<<6,value);}
	bool has_save() const {return flag(0, 1<<7);}
	void has_save(bool value) {flag(0,1<<7,value);}
	bool has_setup() const {return flag(0, 1<<8);}
	void has_setup(bool value) {flag(0,1<<8,value);}
	bool has_draft() const {return flag(0, 1<<9);}
	void has_draft(bool value) {flag(0,1<<9,value);}
	bool has_from_to() const {return flag(0, 1<<10);}
	void has_from_to(bool value) {flag(0,1<<10,value);}
	bool sideways_selected() const {return flag(0, 1<<11);}
	void sideways_selected(bool value) {flag(0,1<<11,value);}
	bool draft_selected() const {return flag(0, 1<<12);}
	void draft_selected(bool value) {flag(0,1<<12,value);}

	int from() const {return int_value(4);}
	void from(int value) {int_value(4,value);}
	int to() const {return int_value(8);}
	void to(int value) {int_value(8,value);}
	int copies() const {return int_value(12);}
	void copies(int value) {int_value(12,value);}
	int scale() const {return int_value(16);}
	void scale(int value) {int_value(16,value);}
	const char *further_options() const {return string(20);}
	void further_options(const char *value) {string(20, value);}
	void further_options(const std::string &value) {string(20, value);}
	/**
	 * Get the name of the window template that provides the window for this
	 * object.
	 *
	 * @returns name of window to use or 0 if default internal window will be used
	 */
	const char *window() const {return string(24);}
	/**
	 * Set the name of the window template that provides the window for this
	 * object.
	 *
	 * @param value The name of window to use or 0 if default internal window will be used
	 */
	void window(const char *value) {string(24);}
	/**
	 * Set the name of the window template that provides the window for this
	 * object.
	 *
	 * @param value The name of window to use or 0 if default internal window will be used
	 */
	void window(const std::string &value) {string(24, value);}

};

}
}

#endif // TBX_RES_RESPRINTDBOX_H

