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

#ifndef TBX_RES_ProgInfo_H
#define TBX_RES_ProgInfo_H

#include "resobject.h"

#include <stdexcept>

namespace tbx
{
namespace res
{

/**
 * Class to represent a ProgInfo resource
 *
 * TODO: New version with web url
 */
class ResProgInfo : public ResObject
{
   public:
	   enum { CLASS_ID = 0x82b40 };

      /**
      * Constructor and empty ResProgInfo
      * @param name template name of the object
	  * @param has_web_button us newer (version 1.01 item with web page button)
      */
      ResProgInfo(std::string name, bool has_web_button = false ) : 
	   ResObject(name, CLASS_ID, has_web_button ? 101 : 100, 36 + (has_web_button ? 40 : 32))
      {
		  if (has_web_button) flags(24);
		  init_message(4,  0) ; // Title - default
		  init_message(12, 0) ; // Purpose
		  init_message(16, 0) ; // Author
		  licence_type(-1);
		  init_message(24, 0) ; // Version
		  init_string(28, 0);  ; // Alternative window
		  if (has_web_button)
		  {
			  init_message(32,0);
		  }
      }

      /**
      * Copy constructor
      *
      * @param other object to create copy of
      */
      ResProgInfo(const ResProgInfo &other) : ResObject(other) {}

      /**
      * Construct from a ResObject
      *
      * @param other object to create copy of
      */
      ResProgInfo(const ResObject &other) : ResObject(other) {check_class_id(CLASS_ID);}

      /**
      * Assignment
      *
      * @param other object to create copy of
      */
      ResProgInfo&operator=(const ResProgInfo&other)
      {
         ResObject::operator=(other);
         return *this;
      }

      /**
      * Assignment from ResObject
      *
      * @param other object to create copy of
      */
      ResProgInfo&operator=(const ResObject &other)
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

	  bool generate_about_to_be_shown() const {return flag(0,1);}
	  void generate_about_to_be_shown(bool value) {flag(0,1,value);}

	  bool generate_dialogue_completed() const {return flag(0,2);}
	  void generate_dialogue_completed(bool value) {flag(0,2,value);}

	  bool include_licence_type() const {return flag(0,4);}
	  void include_licence_type(bool value) {flag(0,4,value);}

	  bool include_web_page_button() const {return flag(0, 8);}
	  bool generate_web_page_click() const {return flag(0, 16);}
	  
   	   /**
	    * Get the title. 0 means use the default.
		*/
	   const char *title() const {return message(4);}

	   /**
	    * Set the title
		*
		* @param value help message or 0 for the default
		* @param max_length maximum length for help message or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the help message.
		*/
	   void title(const char *value, int max_length = -1) {message_with_length(4, value, max_length);}
	   void title(const std::string &value, int max_length = -1) {message_with_length(4, value, max_length);}
	   
	   /**
	    * Get the maximum help message length
		*/
	   int max_title() const {return int_value(8);}

		/**
		 * Get the purpose
		 */
	   const char *purpose() const {return message(12);}

	   /**
	    * Set the purpose of the application
		*/
	   void purpose(const char *value) {message(12, value);}
	   void purpose(const std::string &value) {message(12, value);}

	   	/**
		 * Get the author
		 */
	   const char *author() const {return message(16);}

	   /**
	    * Set the author of the application
		*/
	   void author(const char *value) {message(16, value);}
	   void author(const std::string &value) {message(16, value);}

	   /**
	    * Get the licence type
		*/
	   int licence_type() const {return int_value(20);}

	   /**
	    * Set the licence type.
		* 0  public domain 
		* 1  single user 
		* 2  single machine 
		* 3  site 
		* 4  network 
		* 5  authority  
		*/
	   void licence_type(int type) {int_value(20, type);}

   	  /**
	   * Get the version
 	   */
	   const char *version() const {return message(24);}

	   /**
	    * Set the version of the application
		*/
	   void version(const char *value) {message(24, value);}
	   void version(const std::string &value) {message(24, value);}

	   	/**
		 * Get the name of the alternative window to use or null for the default
		 */
	   const char *window() const {return string(28);}

	   	/**
		 * Set the name of the alternative window to use or null for the default
		 */
	   void window(const char *value) {string(28, value);}
	   void window(const std::string &value) {string(28, value);}

	   /**
	    * URI added with Proginfo version 1.01
		*/
	   const char *uri() const {return (object_version() >= 101) ? string(32) : "";}
	   void uri(const char *value)
	   {
		   //TODO: upgrading object if necessary rather than throw an exception
		   if (object_version() < 101) throw std::invalid_argument("ProgInfo version not high enough for URI");
		   message(32, value);
	   }
	   void uri(const std::string &value)
	   {
		   //TODO: set upgrading object if necessary
		   if (object_version() < 101) throw std::invalid_argument("ProgInfo version not high enough for URI");
		   message(32, value);
	   }

	   /**
	    * event generated for web page click added with version 1.01
		*/
	   int web_event() const {return (object_version() >= 101) ? int_value(36) : -1;}
	   int web_event(int event_id)
	   {
		   //TODO: set upgrading object if necessary
		   if (object_version() < 101) throw std::invalid_argument("ProgInfo version not high enough for URI");
		   int_value(36, event_id);
	   }
};

}

}
#endif // TBX_RES_ProgInfo_H
