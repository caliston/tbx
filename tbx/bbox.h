/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008 Alan Buckley   All Rights Reserved.
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


#ifndef TBX_BBOX_H_
#define TBX_BBOX_H_

#include "point.h"
#include "size.h"

namespace tbx
{

/**
 * Class to represent a two dimensional bounding box
 */
class BBox
{
public:
	/**
	 * Uninitialised bounding box
	 */
	BBox() {};

	/**
	 * Construct from the x and y coordinates
	 */
	BBox(int xmin, int ymin, int xmax, int ymax) : min(xmin,ymin), max(xmax,ymax) {}

	/**
	 * Construct from another bounding box
	 */
	BBox(const BBox &other) : min(other.min), max(other.max) {}

	/**
	 * Construct from the minimum and maximum points of the box
	 */
	BBox(const Point &imin, const Point &imax) : min(imin), max(imax) {}

	/**
	 * Minimum coordinate of the bounding box
	 */
	Point min;
	/**
	 * Maximum coordinate of the bounding box
	 */
	Point max;

	/**
	 * Assign value from another bounding box
	 */
	BBox &operator=(const BBox &other) {min=other.min; max=other.max; return *this;}

	/**
	 * Check if two bounding boxes are equal
	 */
	bool operator==(const BBox &other) const {return min==other.min && max == other.max;}

	/**
	 * Check if two bounding boxes are not equal
	 */
	bool operator!=(const BBox &other) const {return min!=other.min || max != other.max;}

	/**
	 * Return the width of the bounding box.
	 */
	int width() const {return max.x - min.x;}

	/**
	 * Return the height of the bounding box.
	 */
	int height() const {return max.y - min.y;}

	/**
	 * Return the width and height of the bounding box as a Size
	 */
	Size size() const {return max - min;}

	/**
	 * Resize the bounding box
	 */
	void size(int width, int height) {max.x = min.x + width; max.y = min.y + height;}
	/**
	 * Resize the bounding box
	 */
	void size(const Size &size) {max.x = min.x + size.width; max.y = min.y + size.height;}

	/**
	 * Move whole box by given amounts
	 */
	void move(int bx, int by) {min.x+=bx; max.x+=bx; min.y+=by; max.y+=by;}

	/**
	 * Move box right
	 */
	void move_right(int bx) {min.x+=bx; max.x+=bx;}

	/**
	 * Move box left
	 */
	void move_left(int bx) {min.x-=bx; max.x-=bx;}

	/**
	 * Move box up
	 */
	void move_up(int by) {min.y+=by; max.y+=by;}

	/**
	 * Move box down
	 */
	void move_down(int by) {min.y-=by; max.y-=by;}

	/**
	 * Increase bounds to cover given box
	 */
	void cover(const BBox &to_cover)
	{
		if (min.x > to_cover.min.x) min.x = to_cover.min.x;
		if (min.y > to_cover.min.y) min.y = to_cover.min.y;
		if (max.x < to_cover.max.x) max.x = to_cover.max.x;
		if (max.y < to_cover.max.y) max.y = to_cover.max.y;
	}

	/**
	 * Inflate box by given amount on each side
	 */
	void inflate(int amount)
	{
		min.x -= amount; max.x += amount;
		min.y -= amount; max.y += amount;
	}

	/**
	 * Check if point is in rectangle.
	 */
	bool contains(const Point &pt) const
	{
		return (pt >= min && pt < max);
	}

	/**
	 * Check if point is in rectangle.
	 */
	bool contains(int x, int y) const
	{
		return (x >= min.x && x < max.x && y >= min.y && y < max.y);
	}

	/**
	 * Check if bounds intesect with another rect
	 */
	bool intersects(const BBox &other) const
	{
		return (max > other.min && min < other.max);
	}

	/**
	 * Ensure min.x <= max.x and min.y <= max.y
	 */
	void normalise()
	{
		if (min.x > max.x) {int t = min.x; min.x = max.x; max.x = t;}
		if (min.y > max.y) {int t = min.y; min.y = max.y; max.y = t;}
	}

};
}
#endif /* BBOX_H_ */
