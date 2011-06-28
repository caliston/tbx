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

/*
 * jpeg.cc
 *
 *  Created on: 2 Nov 2010
 *      Author: alanb
 */

#include "jpeg.h"
#include "kernel.h"
#include <fstream>

namespace tbx {

/**
 * Construct an unloaded JPEG image
 */
JPEG::JPEG(void)
{
	_image = 0;
	_size = 0;
	_flags = 0;
	_width = 0;
	_height = 0;
	_x_density = 0;
	_y_density = 0;
	_extra_workspace = 0;
	_plot_flags = 3;
}

/**
 * Destroy image data if loaded
 */
JPEG::~JPEG(void)
{
	delete [] _image;
}

/**
 * Copy constructor
 */
JPEG::JPEG(const JPEG &other)
{
	if (other._image != 0)
	{
		_image = new unsigned char[other._size];
		std::memcpy(_image, other._image, other._size);
	} else _image = 0;

	_size = other._size;
	_flags = other._flags;
	_width = other._width;
	_height = other._height;
	_x_density = other._x_density;
	_y_density = other._y_density;
	_extra_workspace = other._extra_workspace;
	_plot_flags = other._plot_flags;
}

/**
 * Assignment operator
 */
JPEG &JPEG::operator=(const JPEG &other)
{
	if (_image) delete [] _image;

	if (other._image != 0)
	{
		_image = new unsigned char[other._size];
		std::memcpy(_image, other._image, other._size);
	} else _image = 0;

	_size = other._size;
	_flags = other._flags;
	_width = other._width;
	_height = other._height;
	_x_density = other._x_density;
	_y_density = other._y_density;
	_extra_workspace = other._extra_workspace;
	_plot_flags = other._plot_flags;

	return *this;
}

/**
 * Load JPEG from file
 *
 * @param file_name name of file to load from
 * @returns true if load succeeded
 */
bool JPEG::load(const std::string &file_name)
{
	std::ifstream file(file_name.c_str());
	bool loaded = false;

	if (file.is_open())
	{
		file.seekg(0, std::ios_base::end);
		int size = file.tellg();
		if (size > 0)
		{
			if (_image) delete[] _image;
			_image = 0;
			_size = 0;
			file.seekg(0, std::ios_base::beg);
			_image = new unsigned char[size];
			_size = size;
			file.read((char *)_image, _size);

			_kernel_swi_regs regs;

			regs.r[0] = 1; /* return dimensions */
			regs.r[1] = reinterpret_cast<int>(_image);
			regs.r[2] = _size;
			// JPEG_Info switch call
			if (_kernel_swi(0x49980, &regs, &regs) == 0)
			{
				_flags = regs.r[0];
				_width = regs.r[2];
				_height = regs.r[3];
				_x_density = regs.r[4];
				_y_density = regs.r[5];
				_extra_workspace = regs.r[6];
				loaded = true;
			} else
			{
				delete [] _image;
				_image = 0;
			}
		}
	}

	return loaded;
}

/**
 * Plot jpeg to screen
 *
 * @param x x coordinate in os units
 * @param y y coordinate in os units
 */
void JPEG::plot(int x, int y) const
{
	if (_image == 0) return;

	_kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(_image);
	regs.r[1] = x;
	regs.r[2] = y;
	regs.r[3] = 0; // No scale factors
	regs.r[4] = _size;
	regs.r[5] = _plot_flags;

	// JPEG_PlotScaled
	_kernel_swi(0x49982, &regs, &regs);
}

/**
 * Plot jpeg to screen
 *
 * @param pos point to plot the jpeg in in os units
 */
void JPEG::plot(const Point &pos) const
{
	if (_image == 0) return;

	_kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(_image);
	regs.r[1] = pos.x;
	regs.r[2] = pos.y;
	regs.r[3] = 0; // No scale factors
	regs.r[4] = _size;
	regs.r[5] = _plot_flags;

	// JPEG_PlotScaled
	_kernel_swi(0x49982, &regs, &regs);
}


/**
 * Plot the jpeg with the given scale factors
 *
 * @param x x coordinate in os units
 * @param y y coordinate in os units
 * @param sf ScaleFactors to scale image
 */
void JPEG::plot(int x, int y, const ScaleFactors &sf)
{
	if (_image == 0) return;

	_kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(_image);
	regs.r[1] = x;
	regs.r[2] = y;
	regs.r[3] = reinterpret_cast<int>(sf.as_array()); // Scale factors
	regs.r[4] = _size;
	regs.r[5] = _plot_flags;

	// JPEG_PlotScaled
	_kernel_swi(0x49982, &regs, &regs);
}

/**
 * Plot jpeg image in given box
 *
 * @param bbox box to plot image into.
 */
void JPEG::plot(const BBox &bbox)
{
	if (_image == 0) return;

	_kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(_image);
	regs.r[1] = 1 | (_plot_flags << 1); // Plot to co-ordinate block
	regs.r[2] = reinterpret_cast<int>(&(bbox.min.x));
	regs.r[3] = _size;

	// JPEG_PlotTransformed
	_kernel_swi(0x49984, &regs, &regs);
}

/*TODO:
void JPEG::plot(const DrawTransform &dt)
{
	_kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(_image);
	regs.r[1] = 0 | (_plot_flags << 1); // Plot to draw transform
	regs.r[2] = reinterpret_cast<int>(&(dt.a));
	regs.r[3] = _size;

	// JPEG_PlotTransformed
	_kernel_swi(0x49984, &regs, &regs);
}
*/

/**
 * Set flag to control dithering when plotting
 *
 * @param dither true to dither image
 */
void JPEG::dithered(bool dither)
{
	if (dither) _plot_flags |=1 ;
	else plot_flags &=~1;
}

/**
 * Set flag to control error defusion when plotting
 *
 * @param error_defused true to use error defusion when plotting
 */
void JPEG::error_defused(bool error_defused)
{
	if (error_defused) _plot_flags |=2;
	else plot_flags &=~2;
}

}
