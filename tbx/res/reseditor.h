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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * reseditor.h
 *
 *  Created on: 29 Apr 2010
 *      Author: alanb
 */

#ifndef TBX_RESEDITOR_H_
#define TBX_RESEDITOR_H_

#include "resobject.h"
#include <vector>

namespace tbx {

namespace res {

const int RESF_MARKER = 0x46534552;// 'RESF'

/**
 * Structure representing the header of a resource file
 */
struct ResFileHeader
{
	unsigned int file_id; /* Magic number should be RESF_MARKER */
	unsigned int version; /* Version number * 100 */
	int object_offset;    /* Offset to first object in the file or -1 for no objects */
};

/**
 * Class to allow creation, loading, editing and saving of a
 * toolbox resource file.
 */
class ResEditor
{
	char *_header;
	std::vector<ResObject> _objects;

public:
	ResEditor();
	virtual ~ResEditor();

	/**
	 * Return header details of the file
	 */
	const ResFileHeader *header() const {return reinterpret_cast<ResFileHeader *>(_header);}

	void clear();

	/**
	 * Return number of objects
	 */
	unsigned int count() const {return _objects.size();}
	bool contains(std::string name) const;
	ResObject &object(std::string name);
	const ResObject &object(std::string name) const;
	void remove(std::string name);

	/**
	 * Add an object to the resource file.
	 *
	 * @param obj The object to add
	 */
	void add(ResObject obj) {_objects.push_back(obj);}


	typedef std::vector<ResObject>::iterator iterator;
	typedef std::vector<ResObject>::const_iterator const_iterator;

	iterator begin() {return _objects.begin();}
	iterator end()   {return _objects.end();}

	const_iterator begin() const {return _objects.begin();}
	const_iterator end()   const {return _objects.end();}

	bool load(std::string filename);
	bool save(std::string filename);

private:
	// Only editor can change header
	ResFileHeader *header() {return reinterpret_cast<ResFileHeader *>(_header);}
};

}

}

#endif /* TBX_RESEDITOR_H_ */
