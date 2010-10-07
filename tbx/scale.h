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

/*
 * scale.h
 *
 *  Created on: 01-Jul-2010
 *      Author: alanb
 */

#ifndef TBX_SCALE_H_
#define TBX_SCALE_H_

#include "showfullobject.h"
#include "listener.h"
#include "eventinfo.h"
#include "window.h"

namespace tbx {

// Listeners
class AboutToBeShownListener;
class HasBeenHiddenListener;
class ScaleApplyFactorListener;

namespace res
{
    class ResScale;
}

/**
 * Class to show a toolbox Scale object.
 *
 * The scale object show a dialogue that allow the scale factors of
 * a document to be set.
 */
class Scale : public tbx::ShowFullObject
{
public:
	enum { TOOLBOX_CLASS = 0x82c00 };

	Scale() {};
	Scale(const Scale &other) : ShowFullObject(other._handle) {}
	Scale(const Object &other) : ShowFullObject(other)	{check_toolbox_class(Scale::TOOLBOX_CLASS);}
	Scale(const std::string &template_name) : ShowFullObject(template_name)	{check_toolbox_class(Scale::TOOLBOX_CLASS);}
	Scale(const res::ResScale &object_template);

	Scale &operator=(const Scale &other) {_handle = other.handle(); return *this;}
	Scale &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
	bool operator==(const Object &other) const {return (_handle == other.handle());}
	bool operator!=(const Object &other) const {return (_handle != other.handle());}


	// Properties
	/**
	 * Return the underlying toolbox window used to implement the Scale object
	 */
	Window window() const {return Window((ObjectId)int_property(0));}

	/**
	 * Set the scale percentage
	 */
	void value(int pc) {int_property(1, pc);}

	/**
	 * Get the scale percantage
	 */
	int value() const {return int_property(2);}

	// Bounds and step
	void lower_bound(int value);
	int lower_bound() const;
	void upper_bound(int value);
	int upper_bound() const;
	void step_size(int value);
	int step_size() const;

	/**
	 * Set the title for the scale dialogue
	 */
	void title(std::string msg) {string_property(5, msg);}

	/**
	 * Return the title of the scale dialogue
	 */
	std::string title() const {return string_property(6);}

	/**
	 * Return the title length
	 */
	int title_length() const {return string_property_length(6);}

	// Listeners
	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void add_apply_factor_listener(ScaleApplyFactorListener *listener);
	void remove_apply_factor_listener(ScaleApplyFactorListener *listener);
};

/**
 * Class to provide apply factor event information for Scale object
 */
class ScaleApplyFactorEvent : public EventInfo
{
public:
	ScaleApplyFactorEvent(IdBlock &id_block, PollBlock &data) :
		EventInfo(id_block, data) {}

	/**
	 * Return the choses scale factor
	 */
	int factor() const {return _data.word[4];}
};

/**
 * Listener for scale factor events
 */
class ScaleApplyFactorListener : public Listener
{
public:
	~ScaleApplyFactorListener() {}

	virtual void scale_apply_factor(const ScaleApplyFactorEvent &apply_event) = 0;
};

}

#endif /* TBX_SCALE_H_ */
