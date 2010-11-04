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
 * jpeg.h
 *
 *  Created on: 2 Nov 2010
 *      Author: alanb
 */

#ifndef TBX_JPEG_H_
#define TBX_JPEG_H_

#include "image.h"
#include "bbox.h"
#include "scalefactors.h"
#include <string>

namespace tbx {

/**
 * Class to load and display JPEG images.
 *
 * This class is only supported on RISC OS 3.6 or later
 */
class JPEG : public Image
{
private:
	unsigned char *_image;
	int _size;
	int _flags;
	int _width;
	int _height;
	int _x_density;
	int _y_density;
	int _extra_workspace;
	int _plot_flags;
public:
	JPEG();
	JPEG(const JPEG &other);
	virtual ~JPEG();

	JPEG &operator=(const JPEG &other);

	// Image override
	virtual void plot(int x, int y) const;
	virtual void plot(const Point &pos) const;

	bool load(const std::string &file_name);

	void plot(int x, int y, const ScaleFactors &sf);
	void plot(const BBox &bbox);
//TODO:	bool plot(const DrawTransform &dt);

	void dithered(bool dither);
	bool dithered() const	{return ((_plot_flags & 1) != 0);}
	void error_defused(bool error_defused);
	bool error_defused() const	{return ((_plot_flags & 2) != 0);}

	bool is_valid() const		{return (_image != 0);}
	int width() const			{return _width;}
	int height() const			{return _height;}
	int x_density() const		{return _x_density;}
	int y_density() const		{return _y_density;}
	int extra_workspace() const	{return _extra_workspace;}

	bool greyscale()const					{return ((_flags & 1) != 0);}
	bool transform_plot_suppoted() const	{return ((_flags & 2) == 0);}
	bool density_simple_ratio() const		{return ((_flags & 4) != 0);}

//TODO:		static bool IsJPEGFile(const std::string &fileName);
//TODO:		static bool GetFileInfo(const std::string &fileName, int *width, int *height, int *xDensity, int *yDensity, int *workSpace, bool *greyscaleImage, bool *noTransformPlots, bool *pixelDensityIsSimpleRatio);

};

}

#endif /* TBX_JPEG_H_ */
