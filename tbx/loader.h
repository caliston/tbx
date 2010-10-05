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

#ifndef TBX_LOADER_H_
#define TBX_LOADER_H_

#include "object.h"
#include "gadget.h"
#include "point.h"

namespace tbx {

/**
 * Class with details of a file load operation
 */
class LoadEvent
{
private:
	Object _object;
	Gadget _gadget;
	Point _point;
	int _est_size;
	int _file_type;
	std::string _file_name;
	bool _from_filer;

public:
	LoadEvent(Object obj, Gadget gadget, int x, int y,
			  int est, int type, const char *file_name, bool from_filer);

	/**
	 * Object the load is going to occur on
	 */
	Object destination_object() {return _object;}

	/**
	 * Gadget for load (can be a null gadget if no gadget is involved).
	 */
	Gadget destination_gadget() {return _gadget;}

	/**
	 * Location for load (screen coordinates)
	 */
	const Point &destination_point() const  {return _point;}

	/**
	 * Estimated size of file.
	 */
	int estimated_size() const  {return _est_size;}

	/**
	 * File type of file
	 */
	int file_type() const {return _file_type;}

	/**
	 * File name of file.
	 *
	 * This will be either a proposed leaf name for the file when
	 * an application is checking if you can load the file or the
	 * full pathname when you are about to load the file.
	 */
	const std::string &file_name() const {return _file_name;}

	/**
	 * Check if the load is from the filer.
	 */
	bool from_filer() const {return _from_filer;}
};

/**
 * Class to handle file/data loading from the filer or
 * an external application.
 *
 * Add the loader to the object you wish to handle the
 * load and override the load_file method to load the
 * file.
 */
class Loader {
public:
	virtual ~Loader() {};

	/**
	 * Override to do the actual file load.
	 *
	 * The file name for the load is in event.file_name().
	 *
	 * @return true if file is loaded.
	 */
	virtual bool load_file(LoadEvent &event) = 0;

	/**
	 * Override to see if the loader can accept a file.
	 *
	 * Defaults to return true as in normal circumstances
	 * the file type is all that is important and that
	 * will be specified when adding the loader.
	 */
	virtual bool accept_file(LoadEvent &event) {return true;}
};

}

#endif /* LOADER_H_ */
