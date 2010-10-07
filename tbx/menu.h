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

#ifndef TBX_MENU_H_
#define TBX_MENU_H_

#include "object.h"
#include "component.h"

namespace tbx {

class MenuItem;
class ResMenuItem;
class AboutToBeShownListener;
class HasBeenHiddenListener;
class SubMenuListener;
class MenuItemSelectedListener;

namespace res
{
class ResMenuItem;
}

/**
 * Class representing a toolbox menu
 */
class Menu: public tbx::Object {
public:
	enum { TOOLBOX_CLASS = 0x828c0 }; //!< The toolbox class for a menu

	/**
	 * Construct an uninitiased menu.
	 *
	 * The menu should not be used until it has been initialised by
	 * assigning it to another initialised menu.
	 */
	Menu() {};

	/**
	 * Construct a menu from another menu.
	 *
	 * Both menus will refer to the same underlying toolbox menu
	 */
	Menu(const Menu &other) : Object(other._handle) {}

	/**
	 * Construct a menu from another toolbox object.
	 *
	 * The menu and the object will refer to the same underlying toolbox menu
	 * @throws ObjectClassError The object assigned to does not refer to a toolbox menu
	 */
	Menu(const Object &other) : Object(other)	{check_toolbox_class(Menu::TOOLBOX_CLASS);}
	Menu(const char *template_name) : Object(template_name)	{check_toolbox_class(Menu::TOOLBOX_CLASS);}

	/**
	 * Assign a menu to an existing menu.
	 *
	 * Both menus will refer to the same underlying toolbox menu
	 */
	Menu &operator=(const Menu &other) {_handle = other.handle(); return *this;}

	/**
	 * Assign a menu to an existing object.
	 *
	 * The menu and the object will refer to the same underlying toolbox menu
	 * @throws ObjectClassError The object assigned to does not refer to a toolbox menu
	 */
	Menu &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}

	/**
	 * Checks if this menu refers to the same underlying toolbox object as
	 * the given object.
	 * @returns true if the menu and object refer to the same thing.
	 */
	bool operator==(const Object &other) const {return (_handle == other.handle());}

	/**
	 * Checks if this menu refers to the same underlying toolbox object as
	 * the given object.
	 * @returns true if the menu and object do not refer to the same thing.
	 */
	bool operator!=(const Object &other) const {return (_handle != other.handle());}

	MenuItem item(ComponentId id);

	/**
	 * Set the help message text for the menu.
	 */
	void help_message(const std::string &msg) {string_property(16,msg);}
	/**
	 * Get the help message text
	 */
	std::string help_message() const {return string_property(17);}
	/**
	 * Get the length of buffer for the help message text
	 */
	int help_message_length() const {return string_property_length(17);}

	MenuItem add(const res::ResMenuItem &res_item, MenuItem *after = 0);
	MenuItem insert(const res::ResMenuItem &res_item, MenuItem *before = 0);
	void erase(ComponentId id);
	void erase(MenuItem &item);

	/**
	 * Return the height of the work area of the menu in OS units.
	 */
	int height() const {return int_property(22);}

	/**
	 * Return the width of the work area of the menu in OS units.
	 */
	int width() const {return int_property(23);}

	/**
	 * Set the title of the menu.
	 */
	void title(const std::string &title) {string_property(24, title);}

	/**
	 * Get the title of the menu.
	 */
	std::string title() const {return string_property(25);}
	/**
	 * Get the size of the buffer for the title
	 */
	int title_length() const {return string_property_length(25);}

	void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
	void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);

	void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
	void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);
};

/**
 * Class representing a single menu item on a menu.
 */
class MenuItem : public Component
{
	MenuItem(ObjectId handle, ComponentId id) : Component(handle, id) {};
	friend class Menu;

public:
	/**
	 * Construct an uninitialised menu item.
	 */
	MenuItem() {_id = NULL_ComponentId;}

	/**
	 * Construct a menu item from an existing menu item.
	 *
	 * Both menu items will refer to the same underlying toolbox menu option
	 */
	MenuItem(const MenuItem &other) : Component(other.handle(), other.id()) {}

	/**
	 * Construct a menu item from a Component
	 *
	 * @throws ObjectClassError if the component is not on a menu.
	 */
	MenuItem(const Component &other) : Component(other) {Menu check(_handle);}

	/**
	 * Assign to another menu item.
	 *
	 * Both menu items will refer to the same underlying toolbox menu option
	 */
	MenuItem &operator=(const MenuItem &other) {_handle = other._handle; _id = other._id; return *this;}

	/**
	 * Assign a menuitem  field to refer to the same underlying toolbox component
	 * as an existing component
	 * @throws ObjectClassError if the component is not in a menu.
	 */
	MenuItem &operator=(const Component &other) {Menu check(other.handle()); _handle = other.handle(); _id = other.id(); return *this;}

	/**
	 * Check if this menu item refers to the same underlying toolbox menu option
	 *
	 * @return true if they are the same
	 */
	bool operator==(const MenuItem &other) {return (_handle == other._handle && _id == other._id);}

	/**
	 * Check if this menu item refers to the same underlying toolbox menu option
	 *
	 * @return true if they are not the same
	 */
	bool operator!=(const MenuItem &other) {return (_handle != other._handle || _id != other._id);}

	/**
	 * Return the menu this menu item belongs to.
	 */
	Menu menu() {return Menu(_handle);}

	// Properties
	void tick(bool set);
	bool ticked() const;

	void fade(bool set);
	bool faded() const;

	void text(const std::string &text);
	std::string text() const;
	int text_length() const;

	void sprite_name(const std::string &name);
	std::string sprite_name() const;
	int sprite_name_length() const;

	void submenu(const Object &object);
	void clear_submenu();
	bool has_submenu() const;
	Object submenu();
	void submenu_event(int id);
	int submenu_event() const;

	void click_show(const Object &object, bool transient = true);
	void clear_click_show();
	bool has_click_show() const;
	Object click_show(bool *transient = 0);
	void click_event(int id);
	int click_event() const;

	void help_message(const std::string &msg);
	std::string help_message() const;
	int help_message_length() const;

	/*TODO: This lot of listeners
	void add_submenu_listener(SubMenuListener *listener);
	void remove_submenu_listener(SubMenuListener *listener);

	void add_selected_listener(MenuItemSelectedListener *listener);
	void remove_selected_listener(MenuItemSelectedListener *listener);
	*/
};

/**
 * Menu item resource definition.
 *
 * This class is used to add menu items to a menu.
 * It can either be initialised by hand or retrieved from an existing
 * menu template using Menu::resource_item.
 */
class ResMenuItem
{
private:
	int _flags;
	ComponentId _id;
	char *_text;
	int _max_text;
	char *_click_show;
	char *_submenu_show;
	int _submenu_event;
	int _click_event;
	char *_help_message;
	int _max_help;

	friend class Menu;

public:
	ResMenuItem();
	ResMenuItem(std::string text);
	ResMenuItem(const ResMenuItem &other);
	~ResMenuItem();

	ResMenuItem &operator=(const ResMenuItem &other);

	/**
	 * Set the ID for the added item.
	 *
	 * Use NULL_ComponentId (or -1) for the menu to assign the lowest
	 * available value when the ResMenuItem is added to the menu.
	 *
	 * @see Menu::add
	 * @see Menu::insert
	 */
	void id(ComponentId id) {_id = id;}
	ComponentId id() const {return _id;}

	void text(std::string text);
	const char *text() const {return _text;}
	void max_text(int max);
	int max_text() const {return _max_text;}

	//TODO: The rest
private:
	void copy_strings();
	void clear_strings();
	void copy_string(char *&str);
};

}

#endif /* TBX_MENU_H_ */
