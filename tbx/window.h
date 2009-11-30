/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
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

#ifndef TBX_WINDOW_H
#define TBX_WINDOW_H

#include "showfullobject.h"
#include "pollinfo.h"
#include "visiblearea.h"

namespace tbx
{
	class Gadget;
	class Listener;
	class AboutToBeShownListener;
	class HasBeenHiddenListener;
	class RedrawListener;
	class OpenWindowListener;
	class CloseWindowListener;
	class PointerEnteringListener;
	class PointerLeavingListener;
	class MouseClickListener;
	class LoseCaretListener;
	class GainCaretListener;

	class WindowState;
	class WindowInfo;
	class Loader;
	class DragHandler;

	/**
	 * The Window object represents a toolbox window.
	 */

	class Window : public ShowFullObject
	{
	public:
		enum { TOOLBOX_CLASS = 0x82880 };

		Window() {};
		Window(const Window &other) : ShowFullObject(other._handle) {}
		Window(const Object &other) : ShowFullObject(other)	{check_toolbox_class(Window::TOOLBOX_CLASS);}
		Window(const std::string &template_name) : ShowFullObject(template_name)	{check_toolbox_class(Window::TOOLBOX_CLASS);}

		Window &operator=(const Window &other) {_handle = other.handle(); return *this;}
		Window &operator=(const Object &other) {_handle = other.handle(); check_toolbox_class(TOOLBOX_CLASS); return *this;}
		bool operator==(const Object &other) const {return (_handle == other.handle());}
		bool operator!=(const Object &other) const {return (_handle != other.handle());}

		Gadget gadget(ComponentId component_id);

		static Window from_handle(WindowHandle window_handle);
		static Gadget from_handle(WindowHandle window_handle, IconHandle icon_handle, Window *window = 0);

		// Properties
		/**
		 * Return the underlying WIMP windows handle of the window.
		 */
		WindowHandle window_handle() const {return (WindowHandle)int_property(0);}

		//TODO: add_gadget
		//TODO: remove_gadget
		//TODO: set_menu
		//TODO: get_menu
		//TODO: set_pointer
		//TODO: get_pointer

		void help_message(const std::string &message) {string_property(7, message);}
		std::string help_message() const {return string_property(8);}

		//TODO: add_keyboard_shortcuts
		//TODO: remove_keyboard_shortcuts

		std::string title() const {return string_property(11);}
		void title(std::string new_title) {string_property(11, new_title);}

		//TODO: Gadget default_focus()
		//TODO: default_focus(Gadget gadget)

		void set_toolbars(Window *internal_bottom_left, Window *internal_top_left, Window *external_bottom_left, Window *external_top_left);
		void get_toolbars(Window *internal_bottom_left, Window *internal_top_left, Window *external_bottom_left, Window *external_top_left);

		void ibl_toolbar(Window window);
		Window ibl_toolbar() const;
		void itl_toolbar(Window window);
		Window itl_toolbar() const;
		void ebl_toolbar(Window window);
		Window ebl_toolbar() const;
		void etl_toolbar(Window window);
		Window etl_toolbar() const;



		void extent(const BBox &new_extent);
		BBox extent() const;

		// Methods
		void force_redraw(const BBox &work_area);
		void update(const BBox &bounds, RedrawListener *background);
		void update(const BBox &bounds, RedrawListener **redrawers, int redrawer_count);
		void get_state(WindowState &state) const;
		void get_info(WindowInfo &info) const;

		// Listeners - toolbox events
		void add_about_to_be_shown_listener(AboutToBeShownListener *listener);
		void remove_about_to_be_shown_listener(AboutToBeShownListener *listener);

		void add_has_been_hidden_listener(HasBeenHiddenListener *listener);
		void remove_has_been_hidden_listener(HasBeenHiddenListener *listener);

		// Listeners - WIMP events
		void add_redraw_listener(RedrawListener *listener);
		void remove_redraw_listener(RedrawListener *listener);
		void add_open_window_listener(OpenWindowListener *listener);
		void remove_open_window_listener(OpenWindowListener *listener);
		void add_close_window_listener(CloseWindowListener *listener);
		void remove_close_window_listener(CloseWindowListener *listener);
		void add_pointer_leaving_listener(PointerLeavingListener *listener);
		void remove_pointer_leaving_listener(PointerLeavingListener *listener);
		void add_pointer_entering_listener(PointerEnteringListener *listener);
		void remove_pointer_entering_listener(PointerEnteringListener *listener);
		void add_mouse_click_listener(MouseClickListener *listener);
		void remove_mouse_click_listener(MouseClickListener *listener);
		void add_lose_caret_listener(LoseCaretListener *listener);
		void remove_lose_caret_listener(LoseCaretListener *listener);
		void add_gain_caret_listener(GainCaretListener *listener);
		void remove_gain_caret_listener(GainCaretListener *listener);

		// Listeners - drag and drop between applications
		void add_loader(Loader *loader, int file_type = -2);
		void remove_loader(Loader *loader, int file_type = -2);

		// Drags from the application
		void drag_rubber_box_local(const Point &start, DragHandler *handler);
		void cancel_drag();
	};

	/**
	 * Window state structure for the Window get_state method
	 */
	class WindowState
	{
	private:
		WindowHandle _window_handle;
		VisibleArea _visible_area;
		WindowHandle _window_handle_in_front;
		unsigned int _flags;

		friend class Window;

	public:
		/**
		 * WIMP window handle of this window
		 */
		WindowHandle window_handle() const {return _window_handle;}

		/**
		 * Visible area of this window
		 */
		const VisibleArea &visible_area() const {return _visible_area;}

		/**
		 * WIMP handle of window on top of this window or -1 for none
		 */
		WindowHandle window_handle_in_front() const {return _window_handle_in_front;}

		/**
		 * Window flags
		 *
		 * See RISC OS PRM for details of all flags. Common flags have
		 * a method to interrogate it in this class
		 */
		unsigned int flags() const {return _flags;}

		/**
		 * Window is open
		 */
		bool open() const {return (_flags & (1<<16));}

		/**
		 * Window is fully visible.
		 *
		 * i.e. not covered at all
		 */
		bool fully_visible() const {return (_flags & (1<<17));}

		/**
		 *  window has been toggled to full size
		 */
		bool full_size() const {return (_flags & (1<<17));}

		/**
		 * the current Open_Window_Request was caused by a click on the Toggle Size icon
		 */
		bool toggle_size() const {return (_flags & (1<<19));}

		/**
		 * window has the input focus
		 */
		bool input_focus() const {return (_flags & (1<<20));}

		/**
		 * force window to screen once on the next Open_Window
		 */
		bool force_to_screen() const {return (_flags & (1<<21));}
	};

	enum ButtonType
	{
		BUTTONTYPE_IGNORE_CLICKS,
		BUTTONTYPE_ALWAYS,
		BUTTONTYPE_CLICK_AUTO,
		BUTTONTYPE_CLICK_ONCE,
		BUTTONTYPE_RELEASE,
		BUTTONTYPE_DOUBLE,
		BUTTONTYPE_CLICK_DRAG,
		BUTTONTYPE_RELEASE_DRAG,
		BUTTONTYPE_DOUBLE_DRAG,
		BUTTONTYPE_CLICK_ONCE9,
		BUTTONTYPE_DOUBLE_CLICK_DRAG,
		BUTTONTYPE_CLICK_DRAG11,
		BUTTONTYPE_RESERVED12,
		BUTTONTYPE_RESERVED13,
		BUTTONTYPE_RESERVED14,
		BUTTONTYPE_GAIN_FOCUS
	};

	/**
	 * Window Information structure for the Window get_info method
	 */
	class WindowInfo
	{
	private:
		WindowHandle _window_handle;
		VisibleArea _visible_area;
		WindowHandle _window_handle_in_front;
		unsigned int _flags;
		unsigned int _colours1;
		unsigned int _colours2;
		BBox _work_area;
		unsigned int _title_bar_flags;
		unsigned int _work_area_flags;
		unsigned int _sprite_area_pointer;
		unsigned int _min_width_and_height;
		unsigned int _title_data[3];
		unsigned int _initial_icon_count;

		friend class Window;

	public:
		/**
		 * WIMP window handle of this window
		 */
		WindowHandle window_handle() const {return _window_handle;}

		/**
		 * Visible area of this window
		 */
		const VisibleArea &visible_area() const {return _visible_area;}

		/**
		 * WIMP handle of window on top of this window or -1 for none
		 */
		WindowHandle window_handle_in_front() const {return _window_handle_in_front;}

		/**
		 * Window flags
		 *
		 * See RISC OS PRM for details of all flags. Common flags have
		 * a method to interrogate it in this class
		 */
		unsigned int flags() const {return _flags;}

		/**
		 * Work area flags.
		 *
		 * Split up in methods below
		 */
		unsigned int work_area_flags() const {return _work_area_flags;}

		/**
		 * Button type
		 */
		ButtonType button_type() const {return ButtonType((_work_area_flags>>12) & 15);}
	};
}

#endif
