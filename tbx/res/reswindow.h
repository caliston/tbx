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
#ifndef TBX_RES_Window_H
#define TBX_RES_Window_H

#include "resobject.h"
#include "resgadget.h"
#include "resshortcut.h"
#include "resiteratorbase.h"
#include "../handles.h"
#include "../colour.h"

namespace tbx
{
typedef int *OsSpriteAreaPtr;

namespace res
{
/**
 * Window resource object
 */
class ResWindow : public ResObject
{
   public:
	   enum { CLASS_ID = 0x82880 };

      /**
      * Constructor and empty ResWindow
      * @param name template name of the object
	  * @param toolbar true to set defaults for a toolbar
      */
      ResWindow(std::string name, bool toolbar = false) : 
          ResObject(name, CLASS_ID, 102, 36 + 164)
      {
		if (toolbar) flags(16); // is a toolbar flag
		else flags(6); // Default to auto-show/auto-close
		init_message(4, 0); // Help message
		init_string(12, 0); // Pointer shape
		init_string(28, 0); // Menu
	    default_focus(-1);
		show_event(-1);
		hide_event(-1);
		init_string(60, 0); // internal_bl
		init_string(64, 0); // internal_tl
		init_string(68, 0); // external_bl
		init_string(72, 0); // external_tl
		if (toolbar)
		{
			visible_xmin(154);
			visible_ymin(828);
			visible_xmax(554);
			visible_ymax(1228);
		} else
		{
			visible_xmin(32);
			visible_ymin(840);
			visible_xmax(724);
			visible_ymax(1264);
		}
		behind(-1);
		if (toolbar) window_flags(0x80000032);
		else window_flags(0xff000012); // Window flags - TODO: lookup what bits mean
		int_value(108, 0x1070207); // Colours - title and work fore and back
		int_value(112, 0xc0103); // scroll bar for and bakcand title focus
		if (toolbar)
		{
			work_ymin(-1000);
			work_xmax(1000);
		} else
		{
			work_ymin(-1024);
			work_xmax(1280);
		}
		title_flags(0x119); // TODO: Look up what these mean
		int_value(144, 0x640064);  // Min width and height 100x100
		init_string(152, 0); // title_validation
		init_sprite_area_ref(140); // window sprite area
		if (toolbar)
		{
			init_message(148, 0); // title_text
			title_buflen(0); // Title buffer length
		} else
		{
			init_message(148, "Window object"); // title_text
			title_buflen(14); // Title buffer length
		}
		init_object_ref(36); // keyboard short cuts pointer
		init_object_ref(44); // Gadget pointer
      }

      /**
      * Copy constructor
      *
      * @param other object to create copy of
      */
      ResWindow(const ResWindow &other) : ResObject(other) {}

      /**
      * Construct from a ResObject
      *
      * @param other object to create copy of
      */
      ResWindow(const ResObject &other) : ResObject(other) {check_class_id(CLASS_ID);}

      /**
      * Assignment
      *
      * @param other object to create copy of
      */
      ResWindow&operator=(const ResWindow&other)
      {
         ResObject::operator=(other);
         return *this;
      }

      /**
      * Assignment from ResObject
      *
      * @param other object to create copy of
      */
      ResWindow&operator=(const ResObject &other)
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
	   * Generate a Window_AboutToBeShown event before showing the underlying Wimp window  
	   */
	  bool generate_about_to_be_shown() const {return flag(0,1);}
	  void generate_about_to_be_shown(bool value) {flag(0,1, value);}

	  /**
	   * Automatically open this Window when a Wimp OpenWindowRequest is received
	   * (when set the client will not see the underlying Wimp requests)  
	   */
	  bool auto_show() const {return flag(0,2);}
	  void auto_show(bool value) {flag(0,2, value);}

	  /**
	   *  Automatically close this Window when a Wimp CloseWindowRequest is received
	   * (when set the client will not see the underlying Wimp requests)  
	   */
	  bool auto_close() const {return flag(0,4);}
	  void auto_close(bool value) {flag(0,4, value);}

	  /**
	   * Generate a Window_HasBeenHidden Event after hiding the underlying Wimp window  
	   */
	  bool generate_has_been_hidden() const {return flag(0,8);}
	  void generate_has_been_hidden(bool value) {flag(0,8, value);}

	  /**
	   * Indicates that this template is of a toolbar
	   */
	  bool toolbar() const {return flag(0,16);}
	  void toolbar(bool value) {flag(0,16, value);}

   	   /**
	    * Get the help message
		*/
	   const char *help_message() const {return message(4);}

	   /**
	    * Set the help message
		*
		* @param value help message
		* @param max_length maximum length for help message or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the help message.
		*/
	   void help_message(const char *value, int max_length = -1) {message_with_length(4, value, max_length);}
	   void help_message(const std::string &value, int max_length = -1) {message_with_length(4, value, max_length);}
	   
	   /**
	    * Get the maximum help message length
		*/
	   int max_help_message() const {return int_value(8);}

	   const char *pointer_shape() const {return string(12);}
	   void pointer_shape(const char *value, int max_length = -1) {string_with_length(12, value, max_length);}
	   void pointer_shape(const std::string &value, int max_length = -1) {string_with_length(12, value, max_length);}

   	   /**
	    * Get the maximum pointer shape length
		*/
	   int max_pointer_shape() const {return int_value(16);}

	   int pointer_x_hot() const {return int_value(20);}
	   void pointer_x_hot(int value) {int_value(20, value);}
	   int pointer_y_hot() const {return int_value(24);}
	   void pointer_y_hot(int value) {int_value(24, value);}

	   const char *menu() const {return string(28);}
	   void menu(const char *value) {string(28, value);}
	   void menu(const std::string &value) {string(28, value);}

	   int num_shortcuts() const {return int_value(32);}
	   int num_gadgets() const {return int_value(40);}
		
	   /**
	    * Component to get the default focus or NULL_ComponentID for none
		*/
	   ComponentId default_focus() const {return int_value(48);}
	   void default_focus(ComponentId id) {int_value(48, id);}

	   /**
	    * Show event or -1 for default.
		*
		* generate_about_to_be_shown must be set for this event to be generated
		*/
	   int show_event() const {return int_value(52);}
	   void show_event(int event_id) {int_value(52, event_id);}

	   /**
	    * Show event or -1 for default.
		*
		* generate_has_been_hidden must be set for this event to be generated
		*/
   	   int hide_event() const {return int_value(56);}
	   void hide_event(int value) {int_value(56, value);}

	   const char *internal_bl_toolbar() const {return string(60);}
	   void internal_bl_toolbar(const char *value) {string(60, value);}
	   void internal_bl_toolbar(const std::string &value) {string(60, value);}

   	   const char *internal_tl_toolbar() const {return string(64);}
	   void internal_tl_toolbar(const char *value) {string(64, value);}
	   void internal_tl_toolbar(const std::string &value) {string(64, value);}

	   const char *external_bl_toolbar() const {return string(68);}
	   void external_bl_toolbar(const char *value) {string(68, value);}
	   void external_bl_toolbar(const std::string &value) {string(68, value);}

	   const char *external_tl_toolbar() const {return string(72);}
	   void external_tl_toolbar(const char *value) {string(72, value);}
	   void external_tl_toolbar(const std::string &value) {string(72, value);}


// window  88  WimpWindow  
	   int visible_xmin() const {return int_value(76);}
	   void visible_xmin(int value) {int_value(76, value);}
	   int visible_ymin() const {return int_value(80);}
	   void visible_ymin(int value) {int_value(80, value);}
	   int visible_xmax() const {return int_value(84);}
	   void visible_xmax(int value) {int_value(84, value);}
	   int visible_ymax() const {return int_value(88);}
	   void visible_ymax(int value) {int_value(88, value);}
	   int scroll_x() const {return int_value(92);}
	   void scroll_x(int value) {int_value(92, value);}
	   int scroll_y() const {return int_value(96);}
	   void scroll_y(int value) {int_value(96, value);}
	   int behind() const {return int_value(100);}
	   void behind(int value) {int_value(100, value);}
	   unsigned int window_flags() const {return uint_value(104);}
	   void window_flags(unsigned int value) {uint_value(104, value);}

	   WimpColour title_fore() const {return byte_value(108);}
	   void title_fore(WimpColour value) {byte_value(108, value);}
	   WimpColour title_back() const {return byte_value(109);}
	   void title_back(WimpColour value) {byte_value(109, value);}
	   WimpColour  work_fore() const {return byte_value(110);}
	   void work_fore(WimpColour value) {byte_value(110, value);}
	   WimpColour work_back() const {return byte_value(111);}
	   void work_back(WimpColour value) {byte_value(111, value);}
	   WimpColour scroll_outer() const {return byte_value(112);}
	   void scroll_outer(WimpColour value) {byte_value(112, value);}
	   WimpColour scroll_inner() const {return byte_value(113);}
	   void scroll_inner(WimpColour value) {byte_value(113, value);}
	   WimpColour title_inputfocus() const {return byte_value(114);}
	   void title_inputfocus(WimpColour value) {byte_value(114, value);}
	   // Note: 115 is a filler byte and must be 0
   	   int work_xmin() const {return int_value(116);}
	   void work_xmin(int value) {int_value(116, value);}
	   int work_ymin() const {return int_value(120);}
	   void work_ymin(int value) {int_value(120, value);}
	   int work_xmax() const {return int_value(124);}
	   void work_xmax(int value) {int_value(124, value);}
	   int work_ymax() const {return int_value(128);}
	   void work_ymax(int value) {int_value(128, value);}
	   unsigned int title_flags() const {return uint_value(132);}
	   void title_flags(unsigned int value) {uint_value(132, value);}
	   //TODO: setters/getters for bits of title flags
	   unsigned int button_type() const {return uint_value(136);}
	   void button_type(unsigned int value) {uint_value(136, value);}
	   //TODO: enum for button_type
	   OsSpriteAreaPtr sprite_area() const {return (OsSpriteAreaPtr)uint_value(140);}
	   void sprite_area(OsSpriteAreaPtr value) {uint_value(140, (unsigned int)value);}

	   unsigned short min_width() const {return ushort_value(144);}
	   void min_width(unsigned short value) {ushort_value(144, value);}
	   unsigned short min_height() const {return ushort_value(146);}
	   void min_height(unsigned short value) {ushort_value(146, value);}

	   const char *title_text() const {return message(148);}
	   void title_text(const char *value) {message(148, value);}
	   void title_text(const std::string &value) {message(148, value);}
	   const char *title_validation() const {return string(152);}
	   void title_validation(const char *value) {string(152, value);}
	   void title_validation(const std::string &value) {string(152, value);}
	   int title_buflen() const {return int_value(156);}
	   void title_buflen(int value) {int_value(156, value);}
	   
	   // num_icons  at 160 (must be zero)  

	   class const_shortcut_iterator : public ResIteratorBase<ResWindow>
	   {
		   const_shortcut_iterator(const ResWindow *window, int offset) : ResIteratorBase<ResWindow>(window, offset) {}
		   friend class ResWindow;
	   public:
		   const_shortcut_iterator &operator++() {_offset += SHORTCUT_SIZE; return *this;}
		   const_shortcut_iterator operator++(int) {const_shortcut_iterator tmp(*this); _offset += SHORTCUT_SIZE; return tmp;}
		   ResShortcut operator*() const {return _object->shortcut_at_offset(_offset);}
	   };
	   friend class const_shortcut_iterator;

	   const_shortcut_iterator shortcut_begin() const {return const_shortcut_iterator(this, first_shortcut_offset());}
	   const_shortcut_iterator shortcut_end() const {return const_shortcut_iterator(this, end_shortcut_offset());}
	   const_shortcut_iterator shortcut_cbegin() const {return const_shortcut_iterator(this, first_shortcut_offset());}
	   const_shortcut_iterator shortcut_cend() const {return const_shortcut_iterator(this, end_shortcut_offset());}
	   const_shortcut_iterator find_shortcut(int component_id) const;

   	   class shortcut_iterator : public ResIteratorBase<ResWindow>
	   {
		   shortcut_iterator(const ResWindow *window, int offset) : ResIteratorBase<ResWindow>(window, offset) {}
		   friend class ResWindow;
	   public:
		   shortcut_iterator &operator++() {_offset += SHORTCUT_SIZE; return *this;}
		   shortcut_iterator operator++(int) {shortcut_iterator tmp(*this); _offset += SHORTCUT_SIZE; return tmp;}
		   ResShortcut operator*() const {return _object->shortcut_at_offset(_offset);}
	   };
	   friend class shortcut_iterator;

	   shortcut_iterator shortcut_begin() {return shortcut_iterator(this, first_shortcut_offset());}
	   shortcut_iterator shortcut_end() {return shortcut_iterator(this, end_shortcut_offset());}
	   //TODO: Document use of SpecialKeys from KeyListener
	   shortcut_iterator find_shortcut(int key_code);

	   ResShortcut shortcut(int key_code) const;
	   bool contains_shortcut(int key_code) const;
	   void add_shortcut(const ResShortcut &shortcut);
	   void replace_shortcut(const ResShortcut &shortcut);
	   void erase_shortcut(int key_code);

	   shortcut_iterator insert_shortcut(shortcut_iterator pos, const ResShortcut &shortcut);
	   shortcut_iterator replace_shortcut(shortcut_iterator pos, const ResShortcut &shortcut);
	   shortcut_iterator erase_shortcut(shortcut_iterator pos);


	   class const_gadget_iterator : public ResIteratorBase<ResWindow>
	   {
		   const_gadget_iterator(const ResWindow *window, int offset) : ResIteratorBase<ResWindow>(window, offset) {}
		   friend class ResWindow;
	   public:
		   const_gadget_iterator &operator++() {_object->next_gadget(_offset); return *this;}
		   const_gadget_iterator operator++(int) {const_gadget_iterator tmp(*this); _object->next_gadget(_offset); return tmp;}
		   ResGadget operator*() const {return _object->gadget_at_offset(_offset);}
	   };
	   friend class const_gadget_iterator;

	   const_gadget_iterator gadget_begin() const {return const_gadget_iterator(this, first_gadget_offset());}
	   const_gadget_iterator gadget_end() const {return const_gadget_iterator(this, end_gadget_offset());}
	   const_gadget_iterator gadget_cbegin() const {return const_gadget_iterator(this, first_gadget_offset());}
	   const_gadget_iterator gadget_cend() const {return const_gadget_iterator(this, end_gadget_offset());}
	   const_gadget_iterator find_gadget(int component_id) const;

   	   class gadget_iterator : public ResIteratorBase<ResWindow>
	   {
		   gadget_iterator(const ResWindow *window, int offset) : ResIteratorBase<ResWindow>(window, offset) {}
		   friend class ResWindow;
	   public:
		   gadget_iterator &operator++() {_object->next_gadget(_offset); return *this;}
		   gadget_iterator operator++(int) {gadget_iterator tmp(*this); _object->next_gadget(_offset); return tmp;}
		   ResGadget operator*() const {return _object->gadget_at_offset(_offset);}
	   };
	   friend class gadget_iterator;

	   gadget_iterator gadget_begin() {return gadget_iterator(this, first_gadget_offset());}
	   gadget_iterator gadget_end() {return gadget_iterator(this, end_gadget_offset());}
	   gadget_iterator find_gadget(int component_id);

	   ResGadget gadget(ComponentId component_id) const;
	   bool contains_gadget(ComponentId component_id) const;
	   void add_gadget(const ResGadget &gadget);
	   void replace_gadget(const ResGadget &gadget);
	   void erase_gadget(ComponentId component_id);

	   gadget_iterator insert_gadget(gadget_iterator pos, const ResGadget &gadget);
	   gadget_iterator replace_gadget(gadget_iterator pos, const ResGadget &gadget);
	   gadget_iterator erase_gadget(gadget_iterator pos);

	protected:
		char *gadget_start() const {return (char *)int_value(44);}
		int first_gadget_offset() const;
		int end_gadget_offset() const;
	    ResGadget gadget_at_offset(int item_offset) const;
		void next_gadget(int &item_offset) const;

		char *shortcut_start() const {return (char *)int_value(36);}
		int first_shortcut_offset() const;
		int end_shortcut_offset() const;
	    ResShortcut shortcut_at_offset(int item_offset) const;

};

}

}
#endif // TBX_RES_Window_H
