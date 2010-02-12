/*
 * scrolllist.cc
 *
 *  Created on: 25-Mar-2009
 *      Author: alanb
 */

#include "scrolllist.h"
#include "swixcheck.h"
#include "kernel.h"

namespace tbx {

void ScrollList::add_item(const std::string &text, int index /*= -1*/)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401c;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	regs.r[5] = 0;
	regs.r[6] = 0;
	regs.r[7] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Add an item with a sprite from the wimp sprite pool
 */
void ScrollList::add_item(const std::string &text, const WimpSprite &sprite, int index /*= -1*/)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401c;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	regs.r[5] = 0;
	regs.r[6] = reinterpret_cast<int>(const_cast<char *>(sprite.name().c_str() ));
	regs.r[7] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Add an item with a use sprite
 */
void ScrollList::add_item(const std::string &text, const UserSprite &sprite, int index /*= -1*/)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401c;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	regs.r[5] = reinterpret_cast<int>(sprite.get_sprite_area()->pointer());
	regs.r[6] = reinterpret_cast<int>(sprite.pointer());
	regs.r[7] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::delete_item(int index)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401d;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = index;
	regs.r[5] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Delete a range of items
 */
void ScrollList::delete_items(int start, int end)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401d;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = start;
	regs.r[5] = end;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Delete all the items
 */
void ScrollList::clear()
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401d;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = -1;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}


void ScrollList::select_unique_item(int index)
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[2] = 0x401E;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::select_item(int index)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401E;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::deselect_item(int index)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401F;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

int ScrollList::first_selected() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4020;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = -1;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[0];
}

int ScrollList::next_selected(int from) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4020;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = from;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[0];
}


void ScrollList::make_visible(int index)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4021;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::item_text(int index, const std::string &text)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4027;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	regs.r[5] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

std::string ScrollList::item_text(int index) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4025;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = 0;
	regs.r[6] = index;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

    std::string value;
    int len = regs.r[5];
    if (len)
    {
       std::auto_ptr<char> m_buffer(new char[len]);
       regs.r[4] = reinterpret_cast<int>(m_buffer.get());
       regs.r[5] = len;

       // Run Toolbox_ObjectMiscOp to get the buffer
       swix_check(_kernel_swi(0x44ec6, &regs, &regs));

       value = m_buffer.get();
    }

    return value;

}


int ScrollList::count_items() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4026;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[0];
}
/**
 * Set the font used for the scroll list items
 *
 * @param name The name of the font
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */
void ScrollList::font(const std::string &name, int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4024;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(name.c_str() ));
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Set the scroll list to use the system font to display the text
 *
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */

void ScrollList::system_font(int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4024;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}


void ScrollList::set_colour(WimpColour foreground, WimpColour background)
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[2] = 0x401F;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = foreground;
	regs.r[4] = background;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::set_colour(Colour foreground, Colour background)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401F;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = foreground;
	regs.r[4] = background;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void ScrollList::get_colour(Colour &foreground, Colour &background) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401F;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
	foreground = regs.r[0];
	background = regs.r[1];
}

static void scrolllist_selection_router(IdBlock &id_block, PollBlock &data, Listener *listener)
{
	ScrollList source(id_block.self_component());
 	 ScrollListSelectionEvent event(source, data.word[5], data.word[4]);
 	 static_cast<ScrollListSelectionListener *>(listener)->scrolllist_selection(event);
}


/**
 * Add listener for when the value of the number range changes
 *
 * The default value change event must have been enabled in the resources
 * for this number range.
 */
void ScrollList::add_selection_listener(ScrollListSelectionListener *listener)
{
	add_listener(0x140181, listener, scrolllist_selection_router);
}

/**
 * Remove listener for when the value of the number range changes
 */
void ScrollList::remove_selection_listener(ScrollListSelectionListener *listener)
{
	remove_listener(0x140181, listener);
}

}
