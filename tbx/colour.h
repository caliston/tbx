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


#ifndef tbx_colour_H
#define tbx_colour_H

namespace tbx
{
class Colour
{
  union {
    unsigned _colour;
    struct { char r, g, b, x; } _k;
  };

public:
  Colour(): _colour(black) {}
  Colour(unsigned c): _colour(c) {}
  Colour(int red, int green, int blue) {_k.r=red;_k.g=green;_k.b=blue;_k.x=0;}


  void red(int r) { _k.r = r; }
  void green(int g) { _k.g = g; }
  void blue(int b) { _k.b = b; }


  int red() const { return _k.r; }
  int green() const { return _k.g; }
  int blue() const { return _k.b; }

  operator unsigned() const { return _colour; }
  Colour &operator=(unsigned c) { _colour = c; return *this; }

  enum { black = 0x00000000, white = 0xFFFFFF00,
	 no_colour = 0xFFFFFFFF };
};

class WimpColour
{
  int _colour;

public:
  WimpColour(): _colour(black) {}
  WimpColour(int c): _colour(c) {}

  operator int() const { return _colour; }
  WimpColour &operator=(int c) { _colour = c; return *this; }

  enum { white = 0, grey0 = 0, grey1 = 1, grey2 = 2, grey3 = 3,
	 grey4 = 4, grey5 = 5, grey6 = 6, grey7 = 7, black = 7,
	 dark_blue = 8, yellow = 9, light_green = 10, red = 11, cream = 12,
	 dark_green = 13, orange = 14, light_blue = 15, no_colour = -1,
	 minimum = 0, maximum = 15 };
};

};

#endif
