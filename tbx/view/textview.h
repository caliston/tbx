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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * TextView.h
 *
 *  Created on: 31-Jul-2009
 *      Author: alanb
 */

#ifndef TBX_TEXTVIEW_H_
#define TBX_TEXTVIEW_H_

#include "../window.h"
#include "../redrawlistener.h"
#include "../openwindowlistener.h"
#include "../margin.h"
#include <vector>

namespace tbx
{
namespace view
{
/**
 * Class to display text in a window
 */
class TextView :
	public tbx::RedrawListener,
	tbx::OpenWindowListener
{
private:
	tbx::Window _window;
	tbx::Margin _margin;
	bool _wrap;
	char *_text;
	unsigned int _size;
	std::vector<unsigned int> _line_end;
	unsigned int _width;

public:
	TextView(tbx::Window window, bool wrap = false);
	virtual ~TextView();

	void update_window_extent();
	void refresh();

	tbx::Window &window() {return _window;}

	const tbx::Margin &margin() const {return _margin;}
	void margin(const tbx::Margin &margin);

	const char *text() const {return _text;}
	void text(const char *text);
	void text(const std::string &text);

    unsigned int size() const {return _size;}

    bool load_file(const std::string &file_name);

    bool wrap() const {return _wrap;}
    void wrap(bool w);

	// Redraw listener override
	virtual void redraw(const tbx::RedrawEvent &event);
	virtual void open_window(tbx::OpenWindowEvent &event);

private:
	void update_window_extent(const BBox &visible_bounds);
	void recalc_layout();
	void recalc_layout(const BBox &visible_bounds);
};

}
}
#endif /* TBX_TEXTVIEW_H_ */
