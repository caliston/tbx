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

#include "window.h"
#include "gadget.h"
#include "eventrouter.h"
#include "abouttobeshownlistener.h"
#include "hasbeenhiddenlistener.h"
#include "redrawlistener.h"
#include "openwindowlistener.h"
#include "closewindowlistener.h"
#include "pointerlistener.h"
#include "mouseclicklistener.h"
#include "keylistener.h"
#include "caretlistener.h"
#include "loadermanager.h"

#include "swis.h"
#include "swixcheck.h"

using namespace tbx;


/**
 * Set menu used with this window
 *
 * @param menu new menu to use.
 */
void Window::menu(Menu menu)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[1] = _handle;
	regs.r[2] = 3;
	regs.r[3] = menu.handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Removes the menu from the window
 */
void Window::remove_menu()
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[1] = _handle;
	regs.r[2] = 3;
	regs.r[3] = 0;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get the current menu used by this object
 *
 * @returns Menu on Window. If there is no menu the return menus null() method
 * will be true.
 */
Menu Window::menu() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[1] = _handle;
	regs.r[2] = 4;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return tbx::Menu(regs.r[0]);
}

/**
 * Set toolbars for this window.
 *
 * pointers set to 0 means don't change
 */
void Window::set_toolbars(Window *internal_bottom_left, Window *internal_top_left, Window *external_bottom_left, Window *external_top_left)
{
	_kernel_swi_regs regs;
	regs.r[0] = ((internal_bottom_left) ? 1 : 0)
			| ((internal_top_left) ? 2 : 0)
			| ((external_bottom_left) ? 4 : 0)
			| ((external_top_left) ? 8 : 0);
	regs.r[1] = _handle;
	regs.r[2] = 18;
	if (internal_bottom_left) regs.r[3] = internal_bottom_left->handle();
	if (internal_top_left) regs.r[4] = internal_top_left->handle();
	if (external_bottom_left) regs.r[5] = external_bottom_left->handle();
	if (external_top_left) regs.r[6] = external_top_left->handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get toolbars
 */
void Window::get_toolbars(Window *internal_bottom_left, Window *internal_top_left, Window *external_bottom_left, Window *external_top_left)
{
	_kernel_swi_regs regs;
	regs.r[0] = ((internal_bottom_left) ? 1 : 0)
			| ((internal_top_left) ? 2 : 0)
			| ((external_bottom_left) ? 4 : 0)
			| ((external_top_left) ? 8 : 0);
	regs.r[1] = _handle;
	regs.r[2] = 19;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	if (internal_bottom_left) *internal_bottom_left = tbx::Window(regs.r[0]);
	if (internal_top_left) *internal_top_left = tbx::Window(regs.r[1]);
	if (external_bottom_left) *external_bottom_left = tbx::Window(regs.r[2]);
	if (external_top_left) *external_top_left = tbx::Window(regs.r[3]);
}

/**
 * Set internal bottom left toolbar
 */
void Window::ibl_toolbar(Window window)
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[1] = _handle;
	regs.r[2] = 18;
	regs.r[3] = window.handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get internal bottom left toolbar
 */
Window Window::ibl_toolbar() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[1] = _handle;
	regs.r[2] = 19;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return tbx::Window(regs.r[0]);
}

/**
 * Set internal top left toolbar
 */
void Window::itl_toolbar(Window window)
{
	_kernel_swi_regs regs;
	regs.r[0] = 2;
	regs.r[1] = _handle;
	regs.r[2] = 18;
	regs.r[4] = window.handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get internal top left toolbar
 */
Window Window::itl_toolbar() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 2;
	regs.r[1] = _handle;
	regs.r[2] = 19;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return tbx::Window(regs.r[1]);
}
/**
 * Set external bottom left toolbar
 */
void Window::ebl_toolbar(Window window)
{
	_kernel_swi_regs regs;
	regs.r[0] = 4;
	regs.r[1] = _handle;
	regs.r[2] = 18;
	regs.r[5] = window.handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get external bottom left toolbar
 */
Window Window::ebl_toolbar() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 4;
	regs.r[1] = _handle;
	regs.r[2] = 19;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return tbx::Window(regs.r[2]);
}
/**
 * Set external top left toolbar
 */
void Window::etl_toolbar(Window window)
{
	_kernel_swi_regs regs;
	regs.r[0] = 8;
	regs.r[1] = _handle;
	regs.r[2] = 18;
	regs.r[6] = window.handle();

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get external top left toolbar
 */
Window Window::etl_toolbar() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 8;
	regs.r[1] = _handle;
	regs.r[2] = 19;

	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return tbx::Window(regs.r[3]);
}

/**
 * Set the work area extent for the window.
 */
void Window::extent(const BBox &new_extent)
{
	// Run Toolbox_ObjectMiscOp
	swix_check(_swix(0x44ec6, _INR(0,3), 0, _handle, 15, &(new_extent.min.x)));
}

/**
 * Get the current work area extent for the window
 */
BBox Window::extent() const
{
	BBox ex;
	// Run Toolbox_ObjectMiscOp
	swix_check(_swix(0x44ec6, _INR(0,3), 0, _handle, 16, &(ex.min.x)));

	return ex;
}

/**
 * Force the area to be redrawn.
 *
 * @param work_area area to redraw in work area coordinates
 */
void Window::force_redraw(const BBox &work_area)
{
	// Run Toolbox_ObjectMiscOp
	swix_check(_swix(0x44ec6, _INR(0,3), 0, _handle, 17, &(work_area.min.x)));
}

/**
 * Update the window immediately using any registered redrawers.
 *
 * The redrawing routines should not remove themselves from the
 * window during this call.
 *
 * @param bounds area to update in work area co-ordinates
 * @param background redrawer to paint the background or 0 for none.
 */
void Window::update(const BBox &bounds, RedrawListener *background = 0)
{
	int more;
	IdBlock id_block(*this);
	PollBlock poll_block;
	EventRouter::WindowEventListenerItem *first_item = event_router()->find_window_event_listener(_handle, 1);

	if (first_item || background)
	{
		poll_block.word[0] = window_handle();
		poll_block.word[1] = bounds.min.x;
		poll_block.word[2] = bounds.min.y;
		poll_block.word[3] = bounds.max.x;
		poll_block.word[4] = bounds.max.y;

		swix_check(_swix(Wimp_UpdateWindow, _IN(1)|_OUT(0), &poll_block, &more));
		while (more)
		{
			RedrawEvent e(id_block, poll_block);
			if (background) background->redraw(e);
			EventRouter::WindowEventListenerItem *redraw = first_item;
			while(redraw)
			{
				static_cast<RedrawListener *>(redraw->listener)->redraw(e);
				redraw = redraw->next;
			}
			if (_swix(Wimp_GetRectangle, _IN(1)|_OUT(0), &poll_block, &more) != 0) more = 0;
		}
	}
}

/**
 * Update the window immediately using the given redrawers.
 *
 * @param bounds area to update in work area co-ordinates
 * @param redrawers array of redrawers to use to update the window
 * @param redrawer_count number of redrawers in the array.
 */
void Window::update(const BBox &bounds, RedrawListener **redrawers, int redrawer_count)
{
	int more;
	IdBlock id_block(*this);
	PollBlock poll_block;

	if (redrawers && redrawer_count)
	{
		poll_block.word[0] = window_handle();
		poll_block.word[1] = bounds.min.x;
		poll_block.word[2] = bounds.min.y;
		poll_block.word[3] = bounds.max.x;
		poll_block.word[4] = bounds.max.y;

		swix_check(_swix(Wimp_UpdateWindow, _IN(1)|_OUT(0), &poll_block, &more));
		while (more)
		{
			RedrawEvent e(id_block, poll_block);
			for (int j = 0; j < redrawer_count; j++)
			{
				redrawers[j]->redraw(e);
			}
			if (_swix(Wimp_GetRectangle, _IN(1)|_OUT(0), &poll_block, &more) != 0) more = 0;
		}
	}
}


/**
 * Get the gadget with the given component id.
 *
 * Note: The id is not validated.
 */
Gadget Window::gadget(ComponentId component_id)
{
	return Gadget(_handle, component_id);
}

/**
 * Get the window from the wimp window handle
 *
 * @param window_handle the wimp window handle
 * @returns A window object for the handle. If the handle was not found
 *          an uninitialised (null) window will be returned.
 */
Window Window::from_handle(WindowHandle window_handle)
{
	ObjectId handle = NULL_ObjectId;

	// Use Window_WimpToToolbox to get object ids
	_swix(0x82884, _INR(0,2)|_OUT(0), 0, window_handle, -1, &handle);

	return Window(handle);
}

/**
 * Get a gadget from a given wimp window/icon handle pair.
 *
 * @param window_handle the wimp window handle
 * @param icon_handle the wimp icon handle
 * @param window pointer to Window to update with the details of the window
 *        or 0 if window details are not required. If the window was not
 *        found the Window will be set to be uninitialised (null).
 * @returns a Gadget for the given handles. If not found the an uninitialised
 *         (null) gadget is returned.
 */
Gadget Window::from_handle(WindowHandle window_handle, IconHandle icon_handle, Window *window /* = 0*/)
{
	ObjectId handle = NULL_ObjectId;
	ComponentId id = NULL_ComponentId;

	// Use Window_WimpToToolbox to get object ids
	_swix(0x82884, _INR(0,2)|_OUTR(0,1), 0, window_handle, icon_handle,
			&handle, &id);

	if (window) window->_handle = handle;

	return Gadget(handle, id);
}

/**
 * Gets the current state of the window.
 *
 * Usually this call isn't needed as the main information
 * it returns is given during redraw events anyway.
 *
 * It can be useful if the state is required outside of
 * a redraw event or if the window flags are required.
 *
 * @param state window state structure to update with the state
 * @throws OsError unable to retrieve state for window.
 */
void Window::get_state(WindowState &state) const
{
	state._window_handle = window_handle();
	swix_check(_swix(Wimp_GetWindowState, _IN(1), &(state._window_handle)));
}

void Window::get_info(WindowInfo &info) const
{
	info._window_handle = window_handle();
	swix_check(_swix(Wimp_GetWindowInfo, _IN(1), ((int)(&(info._window_handle)))|1));
}

/**
 * Open window in a new location.
 *
 * This should only be called once a window has been shown.
 * Resize, scroll and/or change the window stack position.
 *
 * Note that all coordinates are rounded down to whole numbers of pixels
 *
 * @param New state required for the window
 * @throws OsError if unable to open the window
 */
void Window::open_window(const WindowState &state)
{
	swix_check(_swix(Wimp_OpenWindow, _IN(1), &(state._window_handle)));
}

/**
 * Scroll window to given coordinates
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to scroll window
 */
void Window::scroll(int x, int y)
{
	WindowState state;
	get_state(state);
	state.visible_area().scroll().x = x;
	state.visible_area().scroll().y = y;
	open_window(state);
}

/**
 * Scroll window to given coordinates
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to scroll window
 */
void Window::scroll(const Point &pos)
{
	WindowState state;
	get_state(state);
	state.visible_area().scroll() = pos;
	open_window(state);
}

/**
 * Returns the current scroll coordinates in the window
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to retrieve them
 */
Point Window::scroll() const
{
	WindowState state;
	get_state(state);
	return state.visible_area().scroll();
}

/**
 * Set the size of the window
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to set the new size
 */
void Window::size(int width, int height)
{
	WindowState state;
	get_state(state);
	state.visible_area().bounds().size(width, height);
	open_window(state);
}
/**
 * Set the size of the window
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to set the new size
 */
void Window::size(const Size &size)
{
	WindowState state;
	get_state(state);
	state.visible_area().bounds().size(size);
	open_window(state);
}

/**
 * Returns the size of the window
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to get the size
 */
Size Window::size() const
{
	WindowState state;
	get_state(state);
	return state.visible_area().bounds().size();
}

/**
 * Set the position of the top left of the visible
 * area of the Window on the desktop
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to reposition the window
 */
void Window::top_left(int x, int y)
{
	WindowState state;
	get_state(state);
	BBox &bounds = state.visible_area().bounds();
	bounds.move(x - bounds.min.x, y - bounds.max.y);
	open_window(state);
}

/**
 * Set the position of the top left of the visible
 * area of the Window on the desktop
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to reposition the window
 */
void Window::top_left(const Point &pos)
{
	WindowState state;
	get_state(state);
	BBox &bounds = state.visible_area().bounds();
	bounds.move(pos.x - bounds.min.x, pos.y - bounds.max.y);
	open_window(state);
}

/**
 * Get the position of the top left of the visible
 * area of the Window on the desktop
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to reposition the window
 */
Point Window::top_left() const
{
	WindowState state;
	get_state(state);
	const BBox &bounds = state.visible_area().bounds();
	return Point (bounds.min.x, bounds.max.y);
}

/**
 * Set position and size of the visible area of window on the desktop
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to reposition the window
 */
void Window::bounds(const BBox &bounds)
{
	WindowState state;
	get_state(state);
	state.visible_area().bounds() = bounds;
	open_window(state);
}

/**
 * Get position and size of the visible area of window on the desktop
 *
 * This should only be called once a window has been shown.
 *
 * @throws OsError if unable to reposition the window
 */
BBox Window::bounds() const
{
	WindowState state;
	get_state(state);
	return state.visible_area().bounds();
}

/**
 * Fast copy of a part of the work area to somewhere else.
 *
 * Uses VDU primitives where possible and invalidates other areas.
 *
 * @param bounds - rectangle to copy in work area coordinates
 * @param to new location in work area coordinates
 */
void Window::block_copy(const BBox &bounds, const Point &to)
{
	_kernel_swi_regs regs;
	regs.r[0] = window_handle();
	regs.r[1] = bounds.min.x;
	regs.r[2] = bounds.min.y;
	regs.r[3] = bounds.max.x;
	regs.r[4] = bounds.max.y;
	regs.r[5] = to.x;
	regs.r[6] = to.y;

	swix_check(_kernel_swi(Wimp_BlockCopy, &regs, &regs));
}

/**
 * Add a listener for when the window is about to be shown.
 *
 * The default about to be shown event must be enabled in
 * the toolbox resources for the listeners to be called.
 */
void Window::add_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	add_listener(0x82880, listener, about_to_be_shown_router);
}

/**
 * Remove a listener for when a window is about to be shown.
 */
void Window::remove_about_to_be_shown_listener(AboutToBeShownListener *listener)
{
	remove_listener(0x82880, listener);
}

/**
 * Add A listener for when the window has been hidden.
 *
 * The default has been hidden event must be enabled in
 * the toolbox resources for the listeners to be called.
 */
void Window::add_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	add_listener(0x82890, listener, has_been_hidden_router);
}

/**
 * Remove a listener for when a window has been hidden
 */
void Window::remove_has_been_hidden_listener(HasBeenHiddenListener *listener)
{
	remove_listener(0x82890, listener);
}

/**
 * Add a listener to be called when the window needs redrawing.
 *
 * This listener will only be called if the auto redraw flag on
 * the window has been unset.
 *
 * The listeners are called in the order they have been added
 */
void Window::add_redraw_listener(RedrawListener *listener)
{
	event_router()->add_window_event_listener(_handle, 1, listener);
}

/**
 * Remove a listener for redraw events.
 */
void Window::remove_redraw_listener(RedrawListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 1, listener);
}

/**
 * Add a listener to be called when the window is opened.
 *
 * This listener is only called it the auto open flag in
 * the window resource is unset.
 *
 * An open window event is generated whenever this visible
 * area, scroll position or stacking order of a window
 * is changed. This includes when the Window is first show.
 *
 * The tbx library automatically calls the Wimp_OpenWindow
 * swi to realise the change on the screen, so it should not
 * be called from the listener.
 */
void Window::add_open_window_listener(OpenWindowListener *listener)
{
	event_router()->add_window_event_listener(_handle, 2, listener);
}

/**
 * Remove a listener for window opened events.
 */
void Window::remove_open_window_listener(OpenWindowListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 2, listener);
}

/**
 * Add a listener to be called when the window has received a
 * request to close.
 *
 * This event is only generated when the auto close flag in
 * the resource is unset.
 */
void Window::add_close_window_listener(CloseWindowListener *listener)
{
	event_router()->add_window_event_listener(_handle, 3, listener);
}

/**
 * Remove a listener for close window events
 */
void Window::remove_close_window_listener(CloseWindowListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 3, listener);
}

/**
 * Add a listener for the pointer leaving the visible work area of
 * the window.
 */
void Window::add_pointer_leaving_listener(PointerLeavingListener *listener)
{
	event_router()->add_window_event_listener(_handle, 4, listener);
}

/**
 * Remove a listener for pointer leaving events
 */
void Window::remove_pointer_leaving_listener(PointerLeavingListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 4, listener);
}

/**
 * Add a listener of the pointer entering the visible work area of the window
 */
void Window::add_pointer_entering_listener(PointerEnteringListener *listener)
{
	event_router()->add_window_event_listener(_handle, 5, listener);
}

/**
 * Remove a listener for pointer entering events
 */
void Window::remove_pointer_entering_listener(PointerEnteringListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 5, listener);
}


/**
 * Add a listener for mouse click events.
 *
 * This listener will only be called if an approriate button type
 * has been set in the Windows resources.
 */
void Window::add_mouse_click_listener(MouseClickListener *listener)
{
	event_router()->add_window_event_listener(_handle, 6, listener);
}

/**
 * Remove a listener for mouse click events.
 */
void Window::remove_mouse_click_listener(MouseClickListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 6, listener);
}

/**
 * Add a listener for key pressed events.
 */
void Window::add_key_listener(KeyListener *listener)
{
	event_router()->add_window_event_listener(_handle, 8, listener);
}

/**
 * Remove a listener for key pressed events.
 */
void Window::remove_key_listener(KeyListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 8, listener);
}

/**
 * Add a listener for when the window loses the caret
 */
void Window::add_lose_caret_listener(LoseCaretListener *listener)
{
	event_router()->add_window_event_listener(_handle, 11, listener);
}

/**
 * Remove a listener for when the window loses the caret
 */
void Window::remove_lose_caret_listener(LoseCaretListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 11, listener);
}

/**
 * Add a listener for when the window gains the caret
 */
void Window::add_gain_caret_listener(GainCaretListener *listener)
{
	event_router()->add_window_event_listener(_handle, 12, listener);
}

/**
 * Remove a listener for when the window gains the caret
 */
void Window::remove_gain_caret_listener(GainCaretListener *listener)
{
	event_router()->remove_window_event_listener(_handle, 12, listener);
}

/**
 * Add a file loader.
 *
 * @param loader the loader to add
 * @param the file type for the loader or -2 (the default) for
 *        any type.
 */
void Window::add_loader(Loader *loader, int file_type /*=-2*/)
{
	LoaderManager *manager = LoaderManager::instance();
	if (manager == 0) manager = new LoaderManager();
	manager->add_loader(_handle, NULL_ComponentId, file_type, loader);
}

/**
 * Remove a file loader.
 *
 * @param loader the loader to remove
 * @param the file type for the loader or -2 (the default) for
 *        any type.
 */
void Window::remove_loader(Loader *loader, int file_type /*=-2*/)
{
	LoaderManager *manager = LoaderManager::instance();
	if (manager != 0)
		manager->remove_loader(_handle, NULL_ComponentId, file_type, loader);
}

/**
 * Start dragging of rubber rotating dashes box confined
 * to inside the window.
 *
 * @param start point to start the drag in screen coordinates
 * @param handler handler called when drag is finished
 */
void Window::drag_rubber_box_local(const Point &start, DragHandler *handler)
{
	WindowState state;
	get_state(state);

	int drag[10];
	drag[0] = window_handle();
	drag[1] = 6;
	drag[2] = start.x;
	drag[3] = start.y;
	drag[4] = start.x;
	drag[5] = start.y;
	drag[6] = state.visible_area().bounds().min.x;
	drag[7] = state.visible_area().bounds().min.y;
	drag[8] = state.visible_area().bounds().max.x;
	drag[9] = state.visible_area().bounds().max.y;

	swix_check(_swix(Wimp_DragBox, _IN(1), drag));
	event_router()->set_drag_handler(handler);
}

/**
 * Cancel current drag operation
 */
void Window::cancel_drag()
{
	swix_check(_swix(Wimp_DragBox, _IN(1), 0));
	event_router()->cancel_drag();
}

