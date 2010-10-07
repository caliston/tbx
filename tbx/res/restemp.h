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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef TBX_RES_TTT_H
#define TBX_RES_TTT_H

#include "resobject.h"

namespace tbx
{
namespace res
{

class ResTTT : public ResObject
{
   public:
	   enum { CLASS_ID = 0x };

      /**
      * Constructor and empty ResTTT
      * @param name template name of the object
      */
      ResTTT(std::string name) : 
          ResObject(name, CLASS_ID, 100, XX)
      {
      }

      /**
      * Copy constructor
      *
      * @param other object to create copy of
      */
      ResTTT(const ResTTT &other) : ResObject(other) {}

      /**
      * Construct from a ResObject
      *
      * @param other object to create copy of
      */
      ResTTT(const ResObject &other) : ResObject(other) {check_class_id(CLASS_ID);}

      /**
      * Assignment
      *
      * @param other object to create copy of
      */
      ResTTT&operator=(const ResTTT&other)
      {
         ResObject::operator=(other);
         return *this;
      }

      /**
      * Assignment from ResObject
      *
      * @param other object to create copy of
      */
      ResTTT&operator=(const ResObject &other)
      {
         other.check_class_id(CLASS_ID);
		 ResObject::operator =(other);
         return *this;
      }

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
	    * Get the help message
		*/
	   const char *help_message() const {return message(XX);}

	   /**
	    * Set the help message
		*
		* @param value help message
		* @param max_length maximum length for help message or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the help message.
		*/
	   void help_message(const char *value, int max_length = -1) {message_with_length(XX, value, max_length);}
	   void help_message(const std::string &value, int max_length = -1) {message_with_length(XX, value, max_length);}
	   
	   /**
	    * Get the maximum help message length
		*/
	   int max_help_message() const {return int_value(XX);}

};

}

}
#endif // TBX_RES_TTT_H
