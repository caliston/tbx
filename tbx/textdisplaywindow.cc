/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2012 Alan Buckley   All Rights Reserved.
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
/*
 * textdisplaywindow.cc
 *
 *  Created on: 2 Mar 2012
 *      Author: alanb
 */

#include "textdisplaywindow.h"
#include "window.h"
#include "gadget.h"
#include "font.h"
#include "modeinfo.h"
#include "button.h"

namespace tbx
{

/**
 * Construct a text display window with the given text
 *
 * @param template_name window template name
 * @param first_bottom_component ID of first component to move down (or -1 for none)
 * @param last_bottom_component ID of last component to move down (or -1 for none)
 * @param first_bottom_right_gadget ID of first component to move down and right (or -1 for none)
 * @param last_bottom_right_gadget ID of last component to move down and right (or -1 for none)
 * @param text The message for the window
 */
TextDisplayWindow::TextDisplayWindow(const std::string &template_name,
		ComponentId first_bottom_gadget, ComponentId last_bottom_gadget,
		ComponentId first_bottom_right_gadget, ComponentId last_bottom_right_gadget,
		const std::string &text) :
		_window(template_name),
		_first_bottom_gadget(first_bottom_gadget),
		_last_bottom_gadget(last_bottom_gadget),
		_first_bottom_right_gadget(first_bottom_right_gadget),
		_last_bottom_right_gadget(last_bottom_right_gadget),
		_text(text),
		_delete_on_hide(false)
{
}


TextDisplayWindow::~TextDisplayWindow()
{
}


/**
 * Set the title for the text display window
 *
 * @param new_title New caption for the text display window
 */
void TextDisplayWindow::title(const std::string &new_title)
{
    _window.title(new_title);
}

/**
 * Set up text display window to delete itself when it is hidden.
 *
 * The text display window should have been created using new.
 */
void TextDisplayWindow::delete_on_hide()
{
  _window.add_has_been_hidden_listener(this);
}

/**
 * Show the text display window
 *
 * The size of the message is calculated and the window resized and gadgets
 * moved accordingly at this point.
 */
void TextDisplayWindow::show()
{
	tbx::ShowFullSpec full_spec;
	calc_layout(full_spec);

    _window.show(full_spec);
}

/**
 * Show the text display window as a menu
 *
 * When shown as a menu clicking outside of the window will close it.
 *
 * The size of the message is calculated and the window resized and gadgets
 * moved accordingly at this point.
 */
void TextDisplayWindow::show_as_menu()
{
	tbx::ShowFullSpec full_spec;
	calc_layout(full_spec);

    _window.show_as_menu(full_spec);
}


/**
 * Calculate layout for the window
 *
 * @param full_spec updated so it can be passed to window::show()
 */
void TextDisplayWindow::calc_layout(tbx::ShowFullSpec &full_spec)
{
    tbx::Gadget text_gadget = _window.gadget(0);

    // Get measurements
    _text_bounds = text_gadget.bounds();
    tbx::BBox old_bounds = _text_bounds;
    tbx::WindowInfo winfo;
    _window.get_info(winfo);
    tbx::BBox wextent = winfo.work_area();

    int max_width = wextent.width() - (winfo.visible_area().bounds().width()  - _text_bounds.width());

    full_spec.visible_area = winfo.visible_area();

    if (calc_line_ends(max_width))
    {
        int extra_width = _text_bounds.width() - old_bounds.width();
        int extra_height = _text_bounds.height() - old_bounds.height();
        text_gadget.bounds(_text_bounds);
        full_spec.visible_area.bounds().max.x += extra_width;
        full_spec.visible_area.bounds().min.y -= extra_height;

        ComponentId comp_id;
        if (_first_bottom_gadget != -1)
        {
        	for (comp_id = _first_bottom_gadget; comp_id <= _last_bottom_gadget; ++comp_id)
        	{
        		tbx::Gadget g = _window.gadget(comp_id);
        		g.move_by(0, -extra_height);

        	}
        }

        if (_first_bottom_right_gadget != -1)
        {
        	for (comp_id = _first_bottom_right_gadget; comp_id <= _last_bottom_right_gadget; ++comp_id)
        	{
        		tbx::Gadget g = _window.gadget(comp_id);
        		g.move_by(extra_width, -extra_height);
        	}
        }
    }

    full_spec.wimp_window = tbx::ShowFullSpec::WINDOW_SHOW_TOP;
    // Centre window on screen
    tbx::ModeInfo mode_info;
    tbx::Size screen_size = mode_info.screen_size();
    full_spec.visible_area.bounds().move_to(
        (screen_size.width - full_spec.visible_area.bounds().width())/2,
        (screen_size.height - full_spec.visible_area.bounds().height())/2
        );
}

/**
 * Redraw the message text
 *
 * @param e Redraw information
 */
void TextDisplayWindow::redraw (const tbx::RedrawEvent &e)
{
    int x = e.visible_area().screen_x(_text_bounds.min.x)+2;
    int y = e.visible_area().screen_y(_text_bounds.max.y)-32;

    tbx::WimpFont font;
    font.set_colours(tbx::WimpColour::black, tbx::WimpColour::no_colour);
    const char *text = _text.c_str();
    int start = 0;

    for (unsigned int row = 0; row < _line_end.size(); row++)
	{
		int end = _line_end[row];
		if (text[start] == '\n') start++;
		else if (text[start] == ' ' && start > 0 && text[start-1] != '\n') start++;
		if (start < end)
		{
			font.paint(x, y, text + start, end-start);
		}
		start = end;
		y -= 40;
	}
}

/**
 * Calculate the line end positions in the string.
 *
 * The message bounds will be increased up to max width and as
 * high as necessary to fit the text.
 *
 * @param max_width - maximum width for expansion
 * @returns true if message bounds were expanded
 */
bool TextDisplayWindow::calc_line_ends(int max_width)
{
   int width = _text_bounds.width(); // Minimum width set by template
   int height = 8;
   tbx::Font font;
   font.desktop_font();
   const char *text = _text.c_str();
   int pos = 0;
   int start;
   int line_width;

   while (pos < (int)_text.size())
   {
        height += 40;
		start = pos;
		pos = font.find_split_os(text + start, -1, max_width, ' ') + start;
		if (pos == start)
		{
			pos = font.find_index_xy_os(text + start, -1, max_width, 0) + start;
			if (pos == start) pos++;
		}
		_line_end.push_back(pos);
		if (pos > start)
		{
		   line_width = font.string_width_os(text+start, pos - start);
		   if (line_width > width) width = line_width;
		}
		if (text[pos] == '\n') pos++;
	}

    // Update message box bounds if necessary
    bool updated = false;
    if (width > _text_bounds.width())
    {
        _text_bounds.max.x = _text_bounds.min.x + width;
        updated = true;
    }
    if (height > _text_bounds.height())
    {
        _text_bounds.min.y = _text_bounds.max.y - height;
        updated = true;
    }

    return updated;
}

/**
 * Self destruct when message window is hidden
 *
 * Set delete_on_hide before showing the window.
 */
void TextDisplayWindow::has_been_hidden (const tbx::EventInfo &hidden_event)
{
   hidden_event.id_block().self_object().delete_object();
   delete this;
}


} /* namespace tbx */
