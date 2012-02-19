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

#include "messagewindow.h"

#include "window.h"
#include "gadget.h"
#include "font.h"
#include "showfullspec.h"
#include "modeinfo.h"
#include "button.h"

namespace tbx
{

/**
 * Show a message in a window centred in the screen.
 *
 * This shows the window and returns immediately. It is appropriate to use
 * instead of report_error as it allows the current and all other applications
 * to multi-task while the mesasge is shown.
 *
 * To use this function a window template called "Message" must be in your
 * resources. A suitable window is provided in TbxRes in the !TBX directory.
 * For full details of this window see the MessageWindow class.
 *
 * @param msg The text of the message. This can be multi-line and the window
 *  will be resized to fit it.
 * @param title The title of the message box. Use "" (the default) to keep
 * the title from the Message resource.
 * @param sprite_name The name of the sprite for the window. Use "" (the
 * default) to keep the sprite name in the Message resource. Useful sprites for
 * this provided with RISC OS include "information", "warning" and "error".
 */
void show_message(const std::string &msg, const std::string &title /*=""*/, const std::string &sprite_name/*=""*/)
{
   MessageWindow *mw = new MessageWindow(msg);
   if (!sprite_name.empty()) mw->sprite(sprite_name);
   if (!title.empty()) mw->title(title);
   mw->delete_on_hide();
   mw->show();
}

/**
 * Construct a message window with the give message
 *
 * @param msg The message for the window
 */
MessageWindow::MessageWindow(const std::string &msg) :
    _window("Message"),
    _msg(msg),
    _delete_on_hide(false)
{
    _window.add_redraw_listener(this);
}

/**
 * Set the name of the sprite to show in the message box
 *
 * @param sprite_name name of sprite shown in the message box.
 */
void MessageWindow::sprite(const std::string &sprite_name)
{
    tbx::Button sprite_gadget = _window.gadget(2);
    sprite_gadget.value(sprite_name);
}

/**
 * Set the title for the message box
 *
 * @param new_title New caption for the message box
 */
void MessageWindow::title(const std::string &new_title)
{
    _window.title(new_title);
}

/**
 * Set up message box to delete itself when it is hidden.
 *
 * The message box should have been created using new.
 */
void MessageWindow::delete_on_hide()
{
  _window.add_has_been_hidden_listener(this);
}

/**
 * Show the message window.
 *
 * The size of the message is calculated and the window resized and gadgets
 * moved accordingly at this point.
 */
void MessageWindow::show()
{
    tbx::Gadget msg_gadget = _window.gadget(0);
    tbx::Gadget btn_gadget = _window.gadget(1);
    tbx::Gadget rule_gadget = _window.gadget(3);

    // Get measurements
    _msg_bounds= msg_gadget.bounds();
    tbx::BBox old_bounds = _msg_bounds;
    tbx::BBox btn_bounds = btn_gadget.bounds();
    tbx::BBox rule_bounds = rule_gadget.bounds();
    tbx::WindowInfo winfo;
    _window.get_info(winfo);
    tbx::BBox wextent = winfo.work_area();

    int max_width = wextent.width() - (winfo.visible_area().bounds().width()  - _msg_bounds.width());

    tbx::ShowFullSpec full_spec;
    full_spec.visible_area = winfo.visible_area();

    if (calc_line_ends(max_width))
    {
        int extra_width = _msg_bounds.width() - old_bounds.width();
        int extra_height = _msg_bounds.height() - old_bounds.height();
        msg_gadget.bounds(_msg_bounds);
        full_spec.visible_area.bounds().max.x += extra_width;
        full_spec.visible_area.bounds().min.y -= extra_height;

        // Move button so it keeps the same distance from the right hand side
        // and move down if text was too big
        btn_bounds.move_to(
           btn_bounds.min.x + extra_width,
           btn_bounds.min.y - extra_height);
        btn_gadget.bounds(btn_bounds);

        // Move rule down
        rule_bounds.move_to(
           rule_bounds.min.x,
           rule_bounds.min.y - extra_height);
        rule_gadget.bounds(rule_bounds);
    }

    full_spec.wimp_window = tbx::ShowFullSpec::WINDOW_SHOW_TOP;
    // Centre window on screen
    tbx::ModeInfo mode_info;
    tbx::Size screen_size = mode_info.screen_size();
    full_spec.visible_area.bounds().move_to(
        (screen_size.width - full_spec.visible_area.bounds().width())/2,
        (screen_size.height - full_spec.visible_area.bounds().height())/2
        );

    _window.show(full_spec);
}

/**
 * Redraw the message text
 *
 * @param e Redraw information
 */
void MessageWindow::redraw (const tbx::RedrawEvent &e)
{
    int x = e.visible_area().screen_x(_msg_bounds.min.x)+2;
    int y = e.visible_area().screen_y(_msg_bounds.max.y)-32;

    tbx::WimpFont font;
    font.set_colours(tbx::WimpColour::black, tbx::WimpColour::no_colour);
    const char *text = _msg.c_str();
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
bool MessageWindow::calc_line_ends(int max_width)
{
   int width = _msg_bounds.width(); // Minimum width set by template
   int height = 8;
   tbx::Font font;
   font.desktop_font();
   const char *text = _msg.c_str();
   int pos = 0;
   int start;
   int line_width;

   while (pos < (int)_msg.size())
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
    if (width > _msg_bounds.width())
    {
        _msg_bounds.max.x = _msg_bounds.min.x + width;
        updated = true;
    }
    if (height > _msg_bounds.height())
    {
        _msg_bounds.min.y = _msg_bounds.max.y - height;
        updated = true;
    }

    return updated;
}

/**
 * Self destruct when message window is hidden
 *
 * Set delete_on_hide before showing the window.
 */
void MessageWindow::has_been_hidden (const tbx::EventInfo &hidden_event)
{
   hidden_event.id_block().self_object().delete_object();
   delete this;
}

};
