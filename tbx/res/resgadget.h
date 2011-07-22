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

#ifndef TBX_RES_RESGADGET_H
#define TBX_RES_RESGADGET_H

#include "resbase.h"

namespace tbx {

class Window;

namespace res {

class ResWindow;

/**
 * Base class for gadget memory resources
 */
class ResGadget : public ResBase
{
	friend class ResWindow;

	ResGadget(void *item_header, int size, ResData *data);

	// Access to allow window object to add a gadget from
	// this template
	friend class tbx::Window;
	void *header() const {return _impl->header();}

public:
	ResGadget(const ResGadget &other) : ResBase(other)
	{
	}

	virtual ~ResGadget() {}

	ResGadget &operator=(const ResGadget &other)
	{
		ResBase::operator=(other);
		return *this;
	}
	
	ResGadget(int type, int size);

	void check_type(int type) const;

protected:
	/**
	 * Make item writeable.
	 */
	void make_writeable()
	{
		if (!_impl->unique())
		{
			ResImpl *new_impl = new ResImpl(*_impl);
			_impl->release();
			_impl = new_impl;
		}
	}

public:
  	  /**
	   * Get all flags as a word raw access.
	   *
	   * Not normally used as there are named methods that allow access
	   * to the individual items in the flags
	   * 
	   */
	  unsigned int flags() const {return uint_value(0);}

	  /**
	   * Set all flags as a word
	   *
	   * Not normally used as there are named methods that allow access
	   * to the individual items in the flags
	   */
	  void flags(unsigned int value) {uint_value(0, value);}

	  /**
	   * Gadget is at the back (created first)
	   */
	  bool back() const {return flag(0, (1<<30));}
	  void back(bool value) {flag(0, (1<<30), value);}

	  
	  /**
	   * Gadget is at the faded
	   */
	  bool faded() const {return flag(0, (1<<31));}
	  void faded(bool value) {flag(0, (1<<31), value);}

	  /**
	   * Gadget type
	   */
	  unsigned short type() const {return ushort_value(4);}

	  /**
	   * Size stored in template
	   */
	  unsigned short stored_size() const {return ushort_value(6);}

	  /**
	   * Size of gadget (including header). Takes into account size of
	   * standard controls.
	   */
	  int size() const {return gadget_size(uint_value(4));}

	  static int gadget_size(unsigned int type_and_size);

	  /**
	   * The minimum x coordinate of the gadget's bounding box (in window work area coordinates).  
	   */
	  int xmin() const {return int_value(8);}
	  void xmin(int x) {int_value(8, x);}

  	  /**
	   * The minimum y coordinate of the gadget's bounding box (in window work area coordinates).  
	   */
	  int ymin() const {return int_value(12);}
	  void ymin(int y) {int_value(12, y);}

	  /**
	   * The maximum x coordinate of the gadget's bounding box (in window work area coordinates).  
	   */
	  int xmax() const {return int_value(16);}
	  void xmax(int x) {int_value(16, x);}

  	  /**
	   * The maximum y coordinate of the gadget's bounding box (in window work area coordinates).  
	   */
	  int ymax() const {return int_value(20);}
	  void ymax(int y) {int_value(20, y);}

	  /**
	   * Component id
	   */
	  int component_id() const {return int_value(24);}
	  void component_id(int value) {int_value(24, value);}
	 
	  /**
	   * Help message
	   */
	  const char *help_message() const {return message(28);}
	  void help_message(const char *msg, int max_length = -1) {message_with_length(28, msg, max_length);}
	  void help_message(const std::string &msg, int max_length = -1) {message_with_length(28, msg, max_length);}

	  /**
	   * Max size of message
	   */
	  int max_help() const {return int_value(32);}
};

}
}

#endif // TBX_RES_RESGADGET_H
