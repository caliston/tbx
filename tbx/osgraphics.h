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

#ifndef TBX_OSGRAPHICS_H
#define TBX_OSGRAPHICS_H

#include "graphics.h"

namespace tbx
{
	/**
	 * Class to draw to graphics to the screen using standard OS routines
	 * with no translation or scaling
	 */
	class OSGraphics : public Graphics
	{
	public:
		OSGraphics();

		virtual void plot(int code, int x, int y);
		// plot action
		enum PlotAction {ACTION_OVERWRITE, ACTION_OR, ACTION_AND, ACTION_XOR,
			ACTION_INVERT, ACTION_NONE, ACTION_AND_NOT, ACTION_OR_NOT};

		void plot_action(PlotAction action);

		// Graphic interface

		// coordinate conversion
		virtual int os_x(int logical_x) const {return logical_x;}
		virtual int os_y(int logical_y) const {return logical_y;}
		virtual int logical_x(int os_x) const {return os_x;}
		virtual int logical_y(int os_y) const {return os_y;}

		virtual Point os(const Point &pt) {return pt;}
		virtual Point logical(const Point &pt) {return pt;}
		virtual BBox os(const BBox &b) {return b;}
		virtual BBox logical(const BBox &b) {return b;}

		virtual void foreground(Colour colour);
		virtual void background(Colour colour);

		virtual void wimp_foreground(WimpColour colour);
		virtual void wimp_background(WimpColour colour);

		// Drawing
		virtual void move(int x, int y);
		virtual void point(int x, int y);
		virtual void line(int fx, int fy, int tx, int ty);
		virtual void line(int x, int y);
		virtual void rectangle(int xmin, int ymin, int xmax, int ymax);
		virtual void fill_rectangle(int xmin, int ymin, int xmax, int ymax);
		virtual void circle(int centre_x, int centre_y, int radius);
		virtual void circle_fill(int centre_x, int centre_y, int radius);
		virtual void arc(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y);
		virtual void segment(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y);
		virtual void sector(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y);
		virtual void ellipse(int centre_x, int centre_y, int intersect_x, int intersect_y, int high_x, int high_y);
		virtual void ellipse_fill(int centre_x, int centre_y, int intersect_x, int intersect_y, int high_x, int high_y);

		virtual void path(const Point *points, int num);
		virtual void polygon(const Point *points, int num);
		virtual void fill_polygon(const Point *points, int num);

		virtual void text(int x, int y, const std::string &text);
		virtual void text(int x, int y, const std::string &text, const Font &font);

		// Sprites
		virtual void sprite(int x, int y, const Sprite &sprite);

	protected:
		int _plot_action;
	};
}

#endif
