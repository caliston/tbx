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
    TextDisplayWindow("Message", 3,3, 1,1, msg)
{
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

};
