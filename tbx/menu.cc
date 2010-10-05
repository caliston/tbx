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

#include "menu.h"
#include "swixcheck.h"
#include "swis.h"
#include "abouttobeshownlistener.h"
#include "hasbeenhiddenlistener.h"
#include "tbxexcept.h"

#include <cstring>

namespace tbx {


/**
 * Retrieve the menu item for a given component id.
 *
 * Note: There is no check the id is valid.
 */
MenuItem Menu::item(ComponentId id)
{
	return MenuItem(_handle, id);
}

/**
 * Static function to return a resource menu item from
 * a resource template.
 *
 * @param res_menu_item menu item resource to fill in with retrieved details
 * @param res Resource handle for a menu resource
 * @param id Component id of item to retrieve.
 * @throws ObjectClassError res is not a menu resource
 * @throws ResMenuItemError component id isn't part of the menu resource
 */
void Menu::resource_item(ResMenuItem &res_menu_item, ResHandle res, ComponentId id)
{
   int *res_data = (int *)res;
   int *body = (int *)res_data[7];
   int num_items = body[7];
   int *item = body + 8;

   if (res_data[0] != TOOLBOX_CLASS) throw ObjectClassError();

   while (num_items && item[1] != id)
   {
	   num_items--;
	   item += 10;
   }
   if (num_items == 0) throw ResMenuItemError();

   res_menu_item.clear_strings();
   memcpy(&res_menu_item._flags, item, 40);
   res_menu_item.copy_strings();
}

void Menu::add_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	add_listener(0x828c0, listener, about_to_be_shown_router);
}

void Menu::remove_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	remove_listener(0x828c0, listener);
}

void Menu::add_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	add_listener(0x828c1, listener, has_been_hidden_router);
}

void Menu::remove_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	remove_listener(0x828c1, listener);
}

/**
 * Add a new menu entry to the menu
 *
 * If the component id of the menu item definition is set to -1 then the
 * lowest available component id is used.
 *
 * @param res_item definition of new entry.
 * @param after item to add it after or 0 (the default) to add to end of the menu
 *
 * @returns MenuItem added
 * @throws OsError add failed.
 */
MenuItem Menu::add(const ResMenuItem &res_item, MenuItem *after /*= 0*/)
{
	ComponentId id;
	swix_check(_swix(0x44ec6, _INR(0,4)|_OUT(0),
			0,
			_handle,
			20,
			(after) ? after->id() : -2,
			&(res_item._flags),
			&id
			));

	return MenuItem(_handle, id);
}

/**
 * Insert a new menu entry in the menu
 *
 * If the component id of the menu item definition is set to -1 then the
 * lowest available component id is used.
 *
 * @param res_item definition of new entry.
 * @param before item to insert it before or 0 (the default) to insert
 *        at the beginning of the menu
 *
 * @returns MenuItem inserted
 * @throws OsError insert failed.
 */

MenuItem Menu::insert(const ResMenuItem &res_item, MenuItem *before /* = 0*/)
{
	ComponentId id;
	swix_check(_swix(0x44ec6, _INR(0,4)|_OUT(0),
			1,
			_handle,
			20,
			(before) ? before->id() : -1,
			&(res_item._flags),
			&id
			));

	return MenuItem(_handle, id);
}

/**
 * Erase the menu item with the given id
 *
 * @throws OsError erase failed.
 */
void Menu::erase(ComponentId id)
{
	swix_check(_swix(0x44ec6, _INR(0,3), 0, _handle, 21, id));
}

/**
 * Erase the menu item given.
 *
 * Sets item to uninitialised state.
 *
 * @throws OsError erase failed.
 */
void Menu::erase(MenuItem &item)
{
	erase(item.id());
	item._handle = NULL_ObjectId;
	item._id = NULL_ComponentId;
}

/**
 * Put a tick by the menu item.
 */
void MenuItem::tick(bool set)
{
	swix_check(_swix(0x44ec6, _INR(0,4), 0, _handle, 0, _id, set));
}

/**
 * Check if the menu item is ticked.
 */
bool MenuItem::ticked() const
{
	int ticked;
	swix_check(_swix(0x44ec6, _INR(0,3) | _OUT(0), 0, _handle, 1, _id, &ticked));

	return (ticked != 0);
}

/**
 * Fade the menu item
 */
void MenuItem::fade(bool set)
{
	swix_check(_swix(0x44ec6, _INR(0,4), 0, _handle, 2, _id, set));
}

/**
 * Check if the menu item is faded
 */
bool MenuItem::faded() const
{
	int faded;
	swix_check(_swix(0x44ec6, _INR(0,3) | _OUT(0), 0, _handle, 3, _id, &faded));

	return (faded != 0);
}

/**
 * Set the text for the menu item.
 *
 * @throws OsError the entries text buffer is not large enough to hold the text
 *                 or the entry is a sprite entry.
 */
void MenuItem::text(const std::string &text)
{
	return string_property(4, text);
}

/**
 * Get the text of the menu item.
 */
std::string MenuItem::text() const
{

	return string_property(5);
}

/**
 * Get the length of the buffer for the menu item text.
 */
int MenuItem::text_length() const
{
	return string_property_length(5);
}

/**
 * Set the sprite name for the menu item.
 *
 * @throws OsError the entries sprite name buffer is not large enough to hold
 *                 the name or the entry is a sprite entry.
 */
void MenuItem::sprite_name(const std::string &name)
{
	string_property(6, name);
}

/**
 * Get the sprite name for a sprite menu item
 */
std::string MenuItem::sprite_name() const
{
	return string_property(7);
}

/**
 * Get the length of the buffer for the sprite name.
 */

int MenuItem::sprite_name_length() const
{
	return string_property_length(7);
}

/**
 * Set the object to show as a submenu from this item
 */
void MenuItem::submenu(const Object &object)
{
	int_property(8, (int)object.handle());
}

/**
 * Clear the submenu from this item
 */
void MenuItem::clear_submenu()
{
	int_property(8, 0);
}

/**
 * Returns true if this object has a submenu
 */
bool MenuItem::has_submenu() const
{
	return (int_property(9) != 0);
}

/**
 * Return submenu or Object::null if none
 */
Object MenuItem::submenu()
{
	return Object((ObjectId)int_property(9));
}

/**
 * Set event to raise when mouse if moved over the submenu arrow.
 *
 * If id is 0 then Menu_SubMenu toolbox event is raised instead
 */
void MenuItem::submenu_event(int id)
{
	int_property(10, id);
}

/**
 * Get the event raised when the mouse is moved over the submenu arrow.
 *
 * If no event has been specified then 0 is returned.
 */
int MenuItem::submenu_event() const
{
	return int_property(11);
}


/**
 * Set object to show if menu item is clicked.
 *
 * @param object object to show
 * @param transient show object as transient (defaults to true)
 */
void MenuItem::click_show(const Object &object, bool transient /*= true*/)
{
	swix_check(_swix(0x44ec6, _INR(0,5),
			0,
			_handle,
			12,
			_id,
			(int)object.handle(),
			transient
			));
}

/**
 * Clear the click show object
 */
void MenuItem::clear_click_show()
{
	swix_check(_swix(0x44ec6, _INR(0,5),
			0,
			_handle,
			12,
			_id,
			0,
			0
			));
}

/**
 * Check if menu item has a click show object
 *
 * @returns true if click show defined
 */
bool MenuItem::has_click_show() const
{
	int handle, transient;
	swix_check(_swix(0x44ec6, _INR(0,3) |_OUTR(0,1),
			0,
			_handle,
			13,
			_id,
			&handle,
			&transient
			));
	return (handle != 0);
}

/**
 * Get click show object for menu item.
 *
 * @param transient pointer to variable to receive transient flag
 *  or 0 if not interested in transient flag. Defaults to 0
 * @returns object to be shown or Object::null if nothing shown
 */
Object MenuItem::click_show(bool *transient /*= 0*/)
{
	int handle, tranval;
	swix_check(_swix(0x44ec6, _INR(0,3) |_OUTR(0,1),
			0,
			_handle,
			13,
			_id,
			&handle,
			&tranval
			));
	if (transient) *transient = ((tranval & 1)!=0);

	return tbx::Object(handle);
}

/**
 * Set click event for menu item.
 *
 * @param id event id or 0 to generate Menu_Selection toolbox event
 */
void MenuItem::click_event(int id)
{
	int_property(14, id);
}

/**
 * Get the click event for this menu item
 *
 * @return click event or 0 if no event was specified
 */
int MenuItem::click_event() const
{
	return int_property(15);
}

/**
 * Set the help message for a menu item.
 *
 * @throws OsError the entries help text buffer is not large enough to hold
 *                 the message.
 */
void MenuItem::help_message(const std::string &msg)
{
	string_property(18, msg);

}

/**
 * Get the items help message
 */
std::string MenuItem::help_message() const
{
	return string_property(19);
}

/**
 * Get the length of the buffer for the help message
 */
int MenuItem::help_message_length() const
{
	return string_property_length(19);
}

/*
void MenuItem::add_submenu_listener(SubMenuListener *listener);
void MenuItem::remove_submenu_listener(SubMenuListener *listener);

void MenuItem::add_selected_listener(MenuItemSelectedListener *listener);
void MenuItem::remove_selected_listener(MenuItemSelectedListener *listener);
*/

/**
 * Construct an uninitialised menu item
 */
ResMenuItem::ResMenuItem() :
	_flags(0),
	_id(-1),
	_text(0),
	_max_text(0),
	_click_show(0),
	_submenu_show(0),
	_submenu_event(0),
	_click_event(0),
	_help_message(0),
	_max_help(0)
{
}

/**
 * Create a Menu Item with the given text.
 */
ResMenuItem::ResMenuItem(std::string text) :
	_flags(0),
	_id(-1),
	_click_show(0),
	_submenu_show(0),
	_submenu_event(0),
	_click_event(0),
	_help_message(0),
	_max_help(0)
{
	_max_text = text.size()+1;
	_text = new char[_max_text];
	text.copy(_text, _max_text - 1);
	_text[_max_text - 1] = 0;
}

/**
 * Construct a ResMenuItem as a copy of another
 */
ResMenuItem::ResMenuItem(const ResMenuItem &other)
{
	clear_strings();
	memcpy(&_flags, &other._flags, 40);
	copy_strings();
}

/**
 * Destructor, frees memory used.
 */
ResMenuItem::~ResMenuItem()
{
	delete [] _text;
	delete [] _click_show;
	delete [] _submenu_show;
	delete [] _help_message;
}

/**
 * Assignment, copies other ResMenuItem
 */
ResMenuItem &ResMenuItem::operator=(const ResMenuItem &other)
{
	clear_strings();
	memcpy(&_flags, &other._flags, 40);
	copy_strings();

	return *this;
}

/**
 * Set the text for the resource menu item.
 *
 * Automatically increase the max text field it is is less than
 * the length of the string. Never shrinks it though.
 */
void ResMenuItem::text(std::string text)
{
	int size = text.size()+1;
	if (size > _max_text) _max_text = size;
	delete [] _text;
	_text = 0;
	_text = new char[size];
	if (size > 1) text.copy(_text, size);
	_text[size-1] = 0;
}

/**
 * Set the maximum size for the text.
 *
 * This will not shrink below the size of any text currently assigned
 * to the control.
 */
void ResMenuItem::max_text(int max)
{
	int cur = (_text == 0) ? 0 : std::strlen(_text);
	_max_text = (max > cur) ? max : cur;
}

/**
 * Copy the strings from the current memory
 * when they are pointing to another items memory.
 */
void ResMenuItem::copy_strings()
{
	copy_string(_text);
	copy_string(_click_show);
	copy_string(_submenu_show);
	copy_string(_help_message);
}

/**
 * Copy string if not null
 */
void ResMenuItem::copy_string(char *&str)
{
	if (str)
	{
		char *tmp = new char[std::strlen(str)];
		std::strcpy(tmp, str);
		str = tmp;
	}
}

/**
 * Delete all the strings in this item
 */
void ResMenuItem::clear_strings()
{
	delete [] _text;
	delete [] _click_show;
	delete [] _submenu_show;
	delete [] _help_message;
}


}
