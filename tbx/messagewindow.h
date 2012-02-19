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

#ifndef TBX_MESSAGEWINDOW_H
#define TBX_MESSAGEWINDOW_H

#include <string>
#include <vector>

#include "redrawlistener.h"
#include "hasbeenhiddenlistener.h"
#include "window.h"

namespace tbx
{

/**
 * Class to show a message in a window in the centre of the screen,
 * resizing the window to fit the message if necessary.
 *
 * To use this window you must have a template called "Message" in
 * your resources with the following characteristics.
 *
 * The visible area shown is the minimum size of the window.
 * The extent of the window sets the maximum width and height of the
 * window.
 *
 * Gadgets:
 *    0 - Button with the needs help flag set. This is used as a guide
 *        to position the message and is resized if necessary.
 *    1 - Action button to close the window
 *    2 - Button to show a sprite.
 *    3 - Button with R2 validation to provide a rule between the text
 *        and the button.
 *
 * A window you can copy to provide this is provided in TbxRes in the !Tbx
 * directory.
 *
 * If the depth or width of the window is changed due to the size of the
 * text the button is moved to stay at the same relative position to the
 * bottom right of the window and the rule is moved down to stay at the
 * same relative position to the bottom of the window.
 */
class MessageWindow :
      public tbx::RedrawListener,
      public tbx::HasBeenHiddenListener
{
   private:
      tbx::Window _window;
      std::string _msg;
      std::vector<int> _line_end;
      tbx::BBox _msg_bounds;
      bool _delete_on_hide;

      virtual void redraw (const tbx::RedrawEvent &e);
      virtual void has_been_hidden (const tbx::EventInfo &hidden_event);
      bool calc_line_ends(int max_width);

   public:
      MessageWindow(const std::string &msg);

      void sprite(const std::string &sprite_name);
      void title(const std::string &title);
      /**
       * Get underlying window used for the message window
       */
      tbx::Window window() {return _window;}

      void delete_on_hide();

      void show();
};

void show_message(const std::string &msg, const std::string &title = "", const std::string &sprite = "");

};

#endif
