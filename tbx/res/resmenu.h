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

#ifndef TBX_RES_MENU_H
#define TBX_RES_MENU_H

#include "resobject.h"
#include "resiteratorbase.h"
#include "../handles.h"

namespace tbx
{
class Menu;
namespace res
{

class ResMenu;

const int MENU_DATA_SIZE = 32;
const int MENU_ITEM_SIZE = 40;

/**
 * Menu item object
 */
class ResMenuItem : public ResBase
{
	friend class ResMenu;

	ResMenuItem(void *item_header, int size, ResData *data);

	// Allow Menu to add items
	friend class tbx::Menu;
	char *header() const {return _impl->header();}

public:
	ResMenuItem(const ResMenuItem &other) : ResBase(other)
	{
	}

	virtual ~ResMenuItem() {}

	ResMenuItem &operator=(const ResMenuItem &other)
	{
		ResBase::operator=(other);
		return *this;
	}
	
	ResMenuItem();

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
	   * Item it ticked
	   */
	  bool ticked() const {return flag(0,1);}
	  void ticked(bool t) {flag(0,1,t);}

	  /**
	   * Entry followed by a dotted line
	   */
	  bool dotted_line() const {return flag(0,2);}
	  void dotted_line(bool value) {flag(0,2,value);}

	  /**
	   * Entry is faded
	   */
	  bool faded() const {return flag(0,256);}
	  void faded(bool value) {flag(0,256, value);}

	  /**
	   * Entry is a sprite.
	   * When not set entry is a text menu item
	   */
	  bool sprite() const {return flag(0,512);}

      // TODO: Changing this flag changes in which table the text is stored (string or messages)
	  // void sprite(bool value) {flag(0,512,value);}

  	  /**
	   * Entry has a submenu (ie a submenu arrow appears next to the entry).
	   */
	  bool has_submenu() const {return flag(0,1024);}
	  void has_submenu(bool value) {flag(0,1024, value);}
  	  /**
	   * Entry deliver submenu_event when the user traverses this entry's submenu arrow with
	   * the mouse pointer (if has_submenu() == true).  
	   */
	  bool generate_submenu_event() const {return flag(0,2048);}
	  void generate_submenu_event(bool value) {flag(0,2048, value);}
  	  /**
	   * Shows object to be shown with Wimp_CreateMenu semantics. The default is to show persistently.  
	   */
	  bool show_transient() const {return flag(0,4096);}
	  void show_transient(bool value) {flag(0,4096, value);}

	/**
	 * Get component id of menu item
	 */
	  ComponentId component_id() const {return int_value(4);}
	  /**
	   * Set component id
	   */
	  void component_id(ComponentId id) {int_value(4,id);}

    /**
	 * Text or sprite name for entry
	 *
	 * The sprite() flag determines if this is a sprite or text entry
	 */
	 const char *text() const {return string(8);}
	 void text(const char *value, int max_length = -1) {make_writeable(); _impl->text_with_length(8, value, max_length, sprite());}
	 void text(const std::string &value, int max_length = -1) {make_writeable(); _impl->text_with_length(8, value, max_length, sprite());}

	 /**
	  * Maximum space for text entry
	  */
	 int max_text() const {return int_value(12);}

	 /**
	  * Item to show on click or null pointer for none
	  */
	 const char *click_show() const {return string(16);}
	 void click_show(const char *show_name) {string(16, show_name);}
	 void click_show(const std::string &show_name) {string(16, show_name);}

 	 /**
	  * Submenu to show for this menu item or null pointer for none
	  */
	 const char *submenu_show() const {return string(20);}
	 void submenu_show(char *show_name) {string(20, show_name);}
	 void submenu_show(const std::string &show_name) {string(20, show_name);}

	 /**
	  * Event to show on submenu or -1 for default
	  */
	 int submenu_event() const {return int_value(24);}
	 void submenu_event(int event_id) {int_value(24, event_id);}

	 int click_event() const {return int_value(28);}
	 void click_event(int event_id) {int_value(28, event_id);}

	   /**
	    * Get the menu item help message
		*/
	   const char *help_message() const {return message(32);}

	   /**
	    * Set the item help message
		*
		* @param value menu help message
		* @param max_length maximum length for help message or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the help message.
		*/
	   void help_message(const char *value, int max_length = -1) {message_with_length(32, value, max_length);}
	   void help_message(const std::string &value, int max_length = -1) {message_with_length(32, value, max_length);}
	   
	   /**
	    * Get the maximum help message length
		*/
	   int max_help_message() const {return int_value(36);}
};

/**
 * Menu object template class
 */
class ResMenu : public ResObject
{
   public:
      enum { CLASS_ID = 0x828c0};

      /**
      * Constructor and empty ResMenu
      * @param name template name of the object
      */
      ResMenu(std::string name) : 
          ResObject(name, CLASS_ID, 102, 36 + 32)
      {
		init_message(4, 0); // Title bar
		init_message(12, 0); // Help message
		about_to_be_shown_event(-1);
		has_been_hidden_event(-1);
      }

      /**
      * Copy constructor
      *
      * @param other object to create copy of
      */
      ResMenu(const ResMenu &other) : ResObject(other) {}

      /**
      * Construct from a ResObject
      *
      * @param other object to create copy of
      */
      ResMenu(const ResObject &other) : ResObject(other) {check_class_id(CLASS_ID);}

      /**
      * Assignment
      *
      * @param other object to create copy of
      */
      ResMenu&operator=(const ResMenu&other)
      {
         ResObject::operator=(other);
         return *this;
      }

      /**
      * Assignment from ResObject
      *
      * @param other object to create copy of
      */
      ResMenu&operator=(const ResObject &other)
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
	   * Generate an event when SWI Toolbox_ShowObject is called for this Menu.
	   */
	  bool generate_about_to_be_shown() const {return flag(0, 1);}
	  void generate_about_to_be_shown(bool generate) {flag(0, 1, generate);}

	  /**
	   * Generate when the Menu has been removed from the screen.
	   */
  	  bool generate_has_been_hidden() const {return flag(0, 2);}
	  void generate_has_been_hidden(bool generate) {flag(0, 2, generate);}

	    /**
	    * Get the iconbar icon title
		*/
	   const char *title() const {return message(4);}

	   /**
	    * Set the title
		*
		* @param title menu title (0 means no title, an empty string means no titlebar)
		* @param max_length maximum length for title or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the title.
		*/
	   void title(const char *value, int max_length = -1) {message_with_length(4, value, max_length);}
	   void title(const std::string &value, int max_length = -1) {message_with_length(4, value, max_length);}
	   
	   /**
	    * Get the maximum title length
		*/
	   int max_title() const {return int_value(8);}

	   /**
	    * Get the menu help message
		*/
	   const char *help_message() const {return message(12);}

	   /**
	    * Set the help message
		*
		* @param value menu help message
		* @param max_length maximum length for help message or -1 to use current value.
		*                   This is alway adjusted to allow for the full length of the help message.
		*/
	   void help_message(const char *value, int max_length = -1) {message_with_length(12, value, max_length);}
	   void help_message(const std::string &value, int max_length = -1) {message_with_length(12, value, max_length);}
	   
	   /**
	    * Get the maximum help message length
		*/
	   int max_help_message() const {return int_value(16);}

      /**
       * Get the event to generate when the menu is shown.
	   * Can be -1 for the default.
       */
	   int about_to_be_shown_event() const {return int_value(20);}

      /**
       * Set the event to generate when the menu is shown.
	   * Use -1 for the default.
       */
	   void about_to_be_shown_event(int event_id) {int_value(20, event_id);}

	  /**
       * Get the event to generate when menu is hidden.
	   * Can be -1 for the default.
       */
	   int has_been_hidden_event() const {return int_value(24);}

      /**
       * Set the event to generate when the menu is hidden.
	   * Use -1 for the default.
       */
	   void has_been_hidden_event(int event_id) {int_value(24, event_id);}

	  /**
	   * Get the number of menu entries
	   */
	   int item_count() const {return int_value(28);}

	   ResMenuItem item(ComponentId component_id) const;

	  /**
	   * Get a copy of item at given index
	   */
	   ResMenuItem item_at(int index) const;

	   /**
	    * Get a copy of an item at the given offset in the body
		*/
	   ResMenuItem item_at_offset(int item_offset) const;


	   /**
	    * Constant iterator for menu items
		*/
	   class const_iterator : public ResIteratorBase<ResMenu>
	   {
		   const_iterator(const ResMenu *menu, int offset) : ResIteratorBase<ResMenu>(menu, offset) {};
		   friend class ResMenu;
	   public:
			const_iterator &operator++() {_offset += MENU_ITEM_SIZE;	return *this;}
			const_iterator operator++(int) {const_iterator temp(*this);	_offset += MENU_ITEM_SIZE;	return temp;}
			ResMenuItem operator*() const {return _object->item_at_offset(_offset);}
	   };
	   friend class const_iterator;

   	   /**
	    * Iterator for menu items
		*
		* Note: You can not use *iterator=value as the iterator
		* returns a copy of the item in the Menu not the item itself.
		*/
	   class iterator : public ResIteratorBase<ResMenu>
	   {
		   iterator(const ResMenu *menu, int offset) : ResIteratorBase<ResMenu>(menu, offset) {};
		   friend class ResMenu;
	   public:
			iterator &operator++() {_offset += MENU_ITEM_SIZE;	return *this;}
			iterator operator++(int) {iterator temp(*this);	_offset += MENU_ITEM_SIZE;	return temp;}
			ResMenuItem operator*() const {return _object->item_at_offset(_offset);}
	   };
	   friend class iterator;

   	   const_iterator begin() const;
	   const_iterator end() const;
   	   const_iterator cbegin() const;
	   const_iterator cend() const;
	   const_iterator find(ComponentId component_id) const;

	   iterator begin();
	   iterator end();
	   iterator find(ComponentId component_id);

	   bool contains(ComponentId component_id) const;
	   void add(const ResMenuItem &item);
	   void replace(const ResMenuItem &item);
	   void erase(ComponentId component_id);

	   iterator insert(iterator pos, const ResMenuItem &item);
	   iterator replace(iterator pos, const ResMenuItem &item);
	   iterator erase(iterator pos);
};

}
}

#endif
