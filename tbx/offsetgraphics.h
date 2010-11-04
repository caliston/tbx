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

#ifndef TBX_OFFSETGRAPHICS_H
#define TBX_OFFSETGRAPHICS_H

#include "osgraphics.h"
#include "visiblearea.h"

namespace tbx
{
	/**
	 * Class to draw to graphics to the screen using standard OS routines
	 * off setting the coordinates given.
	 */
	class OffsetGraphics : public OSGraphics
	{
	private:
		int _offset_x;
		int _offset_y;
	public:
		OffsetGraphics() {_offset_x = 0; _offset_y = 0;}
		OffsetGraphics(int offset_x, int offset_y) {_offset_x = offset_x; _offset_y = offset_y;}
		OffsetGraphics(const VisibleArea &area) {_offset_x = area.screen_x(0); _offset_y = area.screen_y(0);}

		void offset_x(int new_x) {_offset_x = new_x;}
		int offset_x() const {return _offset_x;}
		void offset_y(int new_y) {_offset_y = new_y;}
		int offset_y() const {return _offset_y;}
		Point offset() const {return Point(_offset_x, _offset_y);}
		void offset(const Point &new_offset) {_offset_x = new_offset.x; _offset_y = new_offset.y;}

		// Override plot to take into account offset
		virtual void plot(int code, int x, int y) {OSGraphics::plot(code, x + _offset_x, y + _offset_y);}

		// Graphic interface

		// coordinate conversion
		virtual int os_x(int logical_x) const {return logical_x + _offset_x;}
		virtual int os_y(int logical_y) const {return logical_y + _offset_y;}
		virtual int logical_x(int os_x) const {return os_x - _offset_x;}
		virtual int logical_y(int os_y) const {return os_y - _offset_y;}
		virtual Point os(const Point &pt) {return Point(pt.x+_offset_x, pt.x+_offset_y);}
		virtual Point logical(const Point &pt) {return Point(pt.x-_offset_x, pt.x-_offset_y);}
		virtual BBox os(const BBox &b) {return BBox(b.min.x + _offset_x, b.min.y + _offset_y, b.max.x + _offset_x, b.max.y + _offset_y);}
		virtual BBox logical(const BBox &b) {return BBox(b.min.x - _offset_x, b.min.y - _offset_y, b.max.x - _offset_x, b.max.y - _offset_y);}

		virtual void text(int x, int y, const std::string &text) {OSGraphics::text(x + _offset_x, y + _offset_y, text);}
		virtual void text(int x, int y, const std::string &text, const Font &font) {OSGraphics::text(x + _offset_x, y + _offset_y, text, font);}

		// Images
		virtual void image(int x, int y, const Image &image) {OSGraphics::image(x + _offset_x, y + _offset_y, image);}
	};
}

#endif /* TBX_OFFSETGRAPHICS_H_ */
