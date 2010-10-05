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

#include "graphics.h"
#include "swixcheck.h"

#include "swis.h"

using namespace tbx;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Create the graphics class with the default plot
 * action of overwrite.
 */
Graphics::Graphics()
{
   _plot_action = ACTION_OVERWRITE;
}


/**
 * Sets the plot action.
 *
 * The plot action. The plot action doesn't take effect until
 * the next colour change.
 */
void Graphics::set_plot_action(PlotAction action)
{
	_plot_action = action;
}

/**
 * Execute the OS_Plot swi.
 *
 * For details of the plot codes see the RISC OS PRM.
 */
void Graphics::plot(int code, int x, int y)
{
	_swix(OS_Plot, _INR(0,2), code, x, y);
}

/**
 * Set foreground graphics colour to the nearest match
 * to the given colour for the current screen mode.
 *
 * It will use an ECF pattern to get a better match.
 */
void Graphics::set_colour(Colour colour)
{
	swix_check(_swix(ColourTrans_SetGCOL, _IN(0)|_INR(3,4),
			(unsigned)colour,
			(1 << 8), // use ECF
			_plot_action));
}

/**
 * Set background graphics colour to the nearest match
 * to the given colour for the current screen mode.
 *
 * It will use an ECF pattern to get a better match.
 */
void Graphics::set_background(Colour colour)
{
	swix_check(_swix(ColourTrans_SetGCOL, _IN(0)|_INR(3,4),
			(unsigned)colour,
			(1 << 7) | (1 << 8), // set background and use ECF
			_plot_action));
}

/**
 * Set the graphics foreground colour to the given WIMP colour
 */
void Graphics::set_wimp_colour(WimpColour colour)
{
	swix_check(_swix(Wimp_SetColour, _IN(0),
			((int)colour)
			| (_plot_action << 4)
			));
}

/**
 * Set the graphics background colour to the given WIMP colour
 */
void Graphics::set_wimp_background(WimpColour colour)
{
	swix_check(_swix(Wimp_SetColour, _IN(0),
			((int)colour)
			| (_plot_action << 4)
			| (1 << 7) // Background flag
			));

}

/**
 * Draw a line between the given to points
 */
void Graphics::line(int fx, int fy, int tx, int ty)
{
	move(fx,fy);
	line_to(tx,ty);
}

/**
 * Draw a rectangle
 */
void Graphics::rect(int xmin, int ymin, int xmax, int ymax)
{
	move(xmin,ymin);
	line_not_end_to(xmax, ymin);
	line_not_end_to(xmax, ymax);
	line_not_end_to(xmin, ymax);
	line_not_end_to(xmin, ymin);
}

/**
 * Draw a triangle
 */
void Graphics::triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	move(x1,y1);
	line_not_end_to(x2, y2);
	line_not_end_to(x3, y3);
	line_not_end_to(x1, y1);
}

/**
 * Draw a filled triangle with the given vertices
 */
void Graphics::fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	move(x1,y1);
	move(x2,y2);
	fill_triangle_to(x3,y3);
}

