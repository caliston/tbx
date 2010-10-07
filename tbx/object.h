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

#ifndef TBX_OBJECT_H
#define TBX_OBJECT_H

#include <string>
#include "handles.h"
#include "pollinfo.h"

namespace tbx
{
	class Application;
	class Command;
	class UserEventListener;
	class ObjectDeletedListener;
	class Component;

	namespace res
	{
	    class ResObject;
	}

	/**
	 * Class to manipulate a toolbox object.
	 *
	 * This is the base class of all toolbox objects
	 */
	class Object
	{
	protected:
		Object(const res::ResObject &object_template);

	public:
		Object() : _handle(NULL_ObjectId) {}
		Object(ObjectId handle) : _handle(handle) {};
		Object(const Object &other) : _handle(other._handle) {};
		Object(const std::string &template_name);

		void delete_object();

		bool null() const {return (_handle == NULL_ObjectId);}
		ObjectId handle() const {return _handle;}

		Object &operator=(const Object &other) {_handle = other._handle; return *this;}
		bool operator==(const Object &other) const {return (_handle == other._handle);}
		bool operator!=(const Object &other) const {return (_handle != other._handle);}
		operator bool() const {return (_handle != NULL_ObjectId);}

		int toolbox_class() const;
		void check_toolbox_class(int class_id) const;

		void *client_handle() const;
		void client_handle(void *client_handle);

		Object parent_object() const;
		Component parent_component() const;
		Object ancestor_object() const;
		Component ancestor_component() const;

		// Visibility
		void show();
		void hide();

		// Listeners for all objects
		void add_command(int command_id, Command *command);
		void remove_command(int command_id, Command *command);

		void add_user_event_listener(int event_id, UserEventListener *listener);
		void remove_user_event_listener(int event_id, UserEventListener *listener);

		void add_object_deleted_listener(ObjectDeletedListener *listener);
		void remove_object_deleted_listener(ObjectDeletedListener *listener);

		void remove_all_listeners();

	protected:
		// Add listener helper
		void add_listener( int action, Listener *listener, RawToolboxEventHandler handler);
		void remove_listener(int action, Listener *listener);

		void set_handler(int action,  Listener *listener, RawToolboxEventHandler handler);

		// Helper functions for standard properties
		int int_property(int property_id) const;
		void int_property(int property_id, int value);

		std::string string_property(int property_id) const;
		void string_property(int property_id, const std::string &value);
		int string_property_length(int property_id) const;

	    bool bool_property(int property_id) const;
	    void bool_property(int property_id, bool value);

	protected:
		ObjectId _handle;
	};
}

#endif
