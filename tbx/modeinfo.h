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

#ifndef  tbx_MODEINFO_H_
#define  tbx_MODEINFO_H_

#include "kernel.h"
#include "swis.h"
#include "point.h"
#include "size.h"

namespace tbx
{

/**
 * Class to return information on a screen mode
 */
class ModeInfo
{
  public:
     ModeInfo()              {_mode = -1;};
     ModeInfo(int mode)      {_mode = mode;};
     void set_mode(int mode = -1) {_mode = mode;};

     int get_mode() const {return _mode;};

     inline int colours() const;
	 inline Point eig() const;
	 inline Size screen_unit() const;
     inline Size pixel_size() const;
     inline Size screen_size() const;

  protected:
     int _mode;
};

inline int ModeInfo::colours() const
{
   int colours;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 3, &colours);
   if (colours == 63) colours = 255;
   return (colours+1);
}

inline Point ModeInfo::eig() const
{
   int x,y;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 4, &x);
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 5, &y);
   return Point(x,y);
}

inline Size ModeInfo::screen_unit() const
{
   int x,y;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 4, &x);
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 5, &y);
   return Size((1<<x),(1<<y));
}

inline Size ModeInfo::pixel_size() const
{
   int x,y;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 11, &x);
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 12, &y);
   return Size(x+1,y+1);
}

inline Size ModeInfo::screen_size() const
{
   int eig, x,y;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 4, &eig);
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 11, &x);
   x = (x + 1) << eig;
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 5, &eig);
   _swix(OS_ReadModeVariable,_IN(0) | _IN(1) | _OUT(2), _mode, 12, &y);
   y = (y + 1) << eig;

   return Size(x,y);
}
};

#endif
