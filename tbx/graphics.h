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
#include "image.h"
#include <string>

namespace tbx
{
	/**
	 * Interface to drawing graphics to the screen.
	 *
	 * This provides a standard interface that can be used with all
	 * the graphics classes.
	 */
	class Graphics
	{
	public:
		Graphics() {}
		virtual ~Graphics() {}

		// coordinate conversion
		virtual int os_x(int logical_x) const = 0;
		virtual int os_y(int logical_y) const = 0;
		virtual int logical_x(int os_x) const = 0;
		virtual int logical_y(int os_y) const = 0;

		virtual Point os(const Point &pt) {Point o;o.x=os_x(pt.x);o.y=os_y(pt.y);return o;}
		virtual Point logical(const Point &pt) {Point o;o.x=logical_x(pt.x);o.y=logical_y(pt.y);return o;}
		virtual BBox os(const BBox &b) {BBox o;o.min = os(b.min);o.max = os(b.max);return o;}
		virtual BBox logical(const BBox &b) {BBox o;o.min = logical(b.min);o.max = logical(b.max);return o;}

		// Colours
		virtual void foreground(Colour colour) = 0;
		virtual void background(Colour colour) = 0;

		virtual void wimp_foreground(WimpColour colour) = 0;
		virtual void wimp_background(WimpColour colour) = 0;

		// Drawing
		virtual void move(int x, int y) = 0;
		virtual void point(int x, int y) = 0;
		virtual void line(int tx, int ty) = 0;
		virtual void line(int fx, int fy, int tx, int ty) {move(fx,ty);line(tx,ty);}
		virtual void rectangle(int xmin, int ymin, int xmax, int ymax) = 0;
		virtual void fill_rectangle(int xmin, int ymin, int xmax, int ymax) = 0;
		virtual void path(const Point *points, int num) = 0;
		virtual void polygon(const Point *points, int num) = 0;
		virtual void fill_polygon(const Point *points, int num) = 0;
		virtual void circle(int centre_x, int centre_y, int radius) = 0;
		virtual void fill_circle(int centre_x, int centre_y, int radius) = 0;
		virtual void arc(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y) = 0;
		virtual void segment(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y) = 0;
		virtual void sector(int centre_x, int centre_y, int start_x, int start_y, int end_x, int end_y) = 0;
		virtual void ellipse(int centre_x, int centre_y, int intersect_x, int intersect_y, int high_x, int high_y) = 0;
		virtual void fill_ellipse(int centre_x, int centre_y, int intersect_x, int intersect_y, int high_x, int high_y) = 0;

		// Drawing using points and boxes
	    virtual void move(const Point &point) {move(point.x,point.y);}
	    virtual void point(const Point &pt) {point(pt.x,pt.y);}
		virtual void line(const Point &to_point) {line(to_point.x,to_point.y);}
		virtual void line(const Point &from_point, const Point &to_point) {move(from_point);line(to_point);}
		virtual void rectangle(const Point &min_point, const Point &max_point) {rectangle(min_point.x, min_point.y, max_point.x, max_point.y);}
		virtual void rectangle(const BBox &r) {rectangle(r.min.x,r.min.y, r.max.x, r.max.y);}
		virtual void fill_rectangle(const Point &min_point, const Point &max_point) {fill_rectangle(min_point.x, min_point.y, max_point.x, max_point.y);}
		virtual void fill_rectangle(const BBox &r) {fill_rectangle(r.min.x,r.min.y, r.max.x, r.max.y);}
		virtual void circle(const Point &centre_point, int radius) {circle(centre_point.x, centre_point.y, radius);}
		virtual void fill_circle(const Point &centre_point, int radius) {fill_circle(centre_point.x, centre_point.y, radius);}
		virtual void arc(const Point &centre_point, const Point &start_point, const Point &end_point) {arc(centre_point.x, centre_point.y, start_point.x, start_point.y, end_point.x, end_point.y);}
		virtual void segment(const Point &centre_point, const Point &start_point, const Point &end_point) {segment(centre_point.x, centre_point.y, start_point.x, start_point.y, end_point.x, end_point.y);}
		virtual void sector(const Point &centre_point, const Point &start_point, const Point &end_point) {sector(centre_point.x, centre_point.y, start_point.x, start_point.y, end_point.x, end_point.y);}
		virtual void ellipse(const Point &centre_point, const Point &intersect_point, const Point &high_point) {ellipse(centre_point.x, centre_point.y, intersect_point.x, intersect_point.y, high_point.x, high_point.y);}
		virtual void fill_ellipse(const Point &centre_point, const Point &intersect_point, const Point &high_point) {fill_ellipse(centre_point.x, centre_point.y, intersect_point.x, intersect_point.y, high_point.x, high_point.y);}

		// Text
		virtual void text(int x, int y, const std::string &text) = 0;
		virtual void text(int x, int y, const std::string &text, const Font &font) = 0;
		virtual void text_colours(Colour foreground, Colour background) = 0;
		virtual void text_colours(Font &font, Colour foreground, Colour background) = 0;
		virtual void text(const Point &pt, const std::string &str) {text(pt.x,pt.y,str);}
		virtual void text(const Point &pt, const std::string &str, const Font &font) {text(pt.x,pt.y,str,font);}

		// Image
		virtual void image(int x, int y, const Image &image) = 0;
		virtual void image(const Point &pt, const Image &im) {image(pt.x, pt.y, im);}
	};
}

#endif
