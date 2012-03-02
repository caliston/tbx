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
 * textdisplaywindow.h
 *
 *  Created on: 2 Mar 2012
 *      Author: alanb
 */

#ifndef TBX_TEXTDISPLAYWINDOW_H_
#define TBX_TEXTDISPLAYWINDOW_H_

#include <string>
#include <vector>

#include "redrawlistener.h"
#include "hasbeenhiddenlistener.h"
#include "window.h"
#include "showfullspec.h"

namespace tbx {

/**
 * Class to show some text in a window in the centre of the screen,
 * resizing the window to fit the text if necessary.
 *
 * Gadgets that should be moved if the window is expanded can be specified
 *
 * To use this window you must have a template in
 * your resources with the following characteristics.
 *
 * The visible area shown is the minimum size of the window.
 * The extent of the window sets the maximum width and height of the
 * window.
 *
 * Gadgets:
 *    0 - Button with the needs help flag set. This is used as a guide
 *        to position the message and is resized if necessary.
 *
 * The "Message" and "Question" windows that are used by the derived classes
 * MessageWindow and QuestionWindow are provided in TbxRes in the !TBX
 * directory which you can copy to base your own window on.
 *
 * If the depth or width of the window is changed due to the size of the
 * text the specified gadgets are moved to stay at the same relative position
 * to the bottom right of the window or moved down to stay at the
 * same relative position to the bottom of the window.
 */
class TextDisplayWindow  :
	public tbx::RedrawListener,
	public tbx::HasBeenHiddenListener
{
protected:
	tbx::Window _window;
private:
	ComponentId _first_bottom_gadget;
	ComponentId _last_bottom_gadget;
	ComponentId _first_bottom_right_gadget;
	ComponentId _last_bottom_right_gadget;
	std::string _text;
	std::vector<int> _line_end;
	tbx::BBox _text_bounds;
	bool _delete_on_hide;

	virtual void redraw (const tbx::RedrawEvent &e);
	virtual void has_been_hidden (const tbx::EventInfo &hidden_event);

	void calc_layout(tbx::ShowFullSpec &full_spec);
	bool calc_line_ends(int max_width);

public:
	TextDisplayWindow(const std::string &template_name,
			ComponentId first_bottom_gadget, ComponentId last_bottom_gadget,
			ComponentId first_bottom_right_gadget, ComponentId last_bottom_right_gadget,
			const std::string &text);
	virtual ~TextDisplayWindow();

    void title(const std::string &title);
    /**
     * Get underlying window used for the message window
     */
    tbx::Window window() {return _window;}

    void delete_on_hide();

    void show();
    void show_as_menu();
};

} /* namespace tbx */
#endif /* TEXTDISPLAYWINDOW_H_ */
