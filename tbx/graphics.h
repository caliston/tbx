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

#ifndef TBX_GRAPHICS_H
#define TBX_GRAPHICS_H

#include "colour.h"
#include "font.h"
#include "bbox.h"

namespace tbx
{
	/**
	 * Class to draw to graphics to the screen
	 */
	class Graphics
	{
	public:
		Graphics();

		void plot(int code, int x, int y);

		// Colours
		void set_colour(Colour colour);
		void set_background(Colour colour);

		void set_wimp_colour(WimpColour colour);
		void set_wimp_background(WimpColour colour);

		// plot action
		enum PlotAction {ACTION_OVERWRITE, ACTION_OR, ACTION_AND, ACTION_XOR,
			ACTION_INVERT, ACTION_NONE, ACTION_AND_NOT, ACTION_OR_NOT};

		void set_plot_action(PlotAction action);

		// Drawing
		inline void move(int x, int y);
		inline void point(int x, int y);
		void line(int fx, int fy, int tx, int ty);
		inline void line_to(int x, int y);
		inline void line_not_end_to(int x, int y);
		void rect(int xmin, int ymin, int xmax, int ymax);
		void triangle(int x1, int y1, int x2, int y2, int x3, int y3);
		inline void fill_rect(int xmin, int ymin, int xmax, int ymax);
		void fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3);
		inline void fill_triangle_to(int x, int y);

		// Drawing using points and boxes
	    void move(const Point &point) {plot(4 ,point.x,point.y);}
	    void point(const Point &point) {plot(69 ,point.x,point.y);}
		void line_to(const Point &to_point) {plot(5 ,to_point.x,to_point.y);}
	    void line_not_end_to(const Point &to_point) {plot(13 ,to_point.x,to_point.y);}
		void line(const Point &from_point, const Point &to_point) {move(from_point);line_to(to_point);}
		void rect(const BBox &r) {rect(r.min.x,r.min.y, r.max.x, r.max.y);}
		void fill_rect(const BBox &rect) {fill_rect(rect.min.x, rect.min.y, rect.max.x, rect.max.y);}
	    void fill_triangle_to(const Point &to_point) {plot(85 ,to_point.x,to_point.y);}

	protected:
		int _plot_action;
	};

	inline void Graphics::move(int x, int y) {plot(4 ,x, y);}
	inline void Graphics::point(int x, int y) {plot(69 ,x, y);}
	inline void Graphics::line_to(int x, int y) {plot(5 ,x, y);}
	inline void Graphics::line_not_end_to(int x, int y) {plot(13 ,x, y);}
	inline void Graphics::fill_rect(int xmin, int ymin, int xmax, int ymax) {move(xmin,ymin);plot(101,xmax,ymax);}
	inline void Graphics::fill_triangle_to(int x, int y) {plot(85 ,x, y);};

}

#endif
