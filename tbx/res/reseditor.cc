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
 * reseditor.cc
 *
 *  Created on: 29 Apr 2010
 *      Author: alanb
 */

#include "reseditor.h"
#include <fstream>
#include <memory>
#include <stdexcept>

namespace tbx {

namespace res {

/**
 * Construct an empty resource file
 */
ResEditor::ResEditor()
{
	_header = new char[sizeof(ResFileHeader)];
	header()->file_id = RESF_MARKER;
	header()->version = 101; // 1.01
	header()->object_offset = -1; // No objects
}

ResEditor::~ResEditor()
{
	delete _header;
}

/**
 * Remove all objects from the editor
 */
void ResEditor::clear()
{
	_objects.clear();
}

/**
 * Load resources from a file
 *
 * @param filename - name of file for load
 * @returns true if load succeeded
 */
bool ResEditor::load(std::string filename)
{
	_objects.clear();

	std::ifstream file(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!file) return false;

	ResFileHeader h;
	file.read((char *)&h, sizeof(ResFileHeader));
	if (!file) return false;

	 char *new_header = new char[h.object_offset == -1 ? 12 : h.object_offset];
	std::memcpy(new_header, &h, sizeof(ResFileHeader));

	if (h.object_offset > (int)sizeof(ResFileHeader))
	{
		file.read(new_header+12, h.object_offset - sizeof(ResFileHeader));
	}
	delete _header;
	_header = new_header;


	while (file)
	{
		ResObject *obj = ResObject::load(file);
		if (obj != 0)
		{
			_objects.push_back(*obj);
			delete obj;
		}
	}

	return true;
}

/**
 * Save resources to the name file
 *
 * @param filename  name of file to save to
 * @returns true if save was successful
 */
bool ResEditor::save(std::string filename)
{
	std::ofstream file(filename.c_str(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

	int obj_offset = header()->object_offset;
	if (_objects.empty()) obj_offset = -1;
	else if (obj_offset == -1) obj_offset = sizeof(ResFileHeader);
	header()->object_offset = obj_offset;

	file.write(_header, obj_offset);
	for (iterator i = _objects.begin(); i != _objects.end(); ++i)
	{
		ResObject &obj = (*i);
		if (!obj.save(file)) return false;
	}

	return file.good();
}


/**
 * Check if editor contains the named object
 *
 * @param name to check for
 * @returns true if object is in this editor
 */
bool ResEditor::contains(std::string name) const
{
	for (const_iterator i = _objects.begin(); i != _objects.end(); ++i)
	{
		if (name == (*i).name()) return true;
	}
	return false;
}

/**
 * Get object
 *
 * @param name name of the object
 * @returns ResObject with the given name
 * @throws std::invalid_argument if object with name doesn't exists
 */
ResObject &ResEditor::object(std::string name)
{
	for (iterator i = _objects.begin(); i != _objects.end(); ++i)
	{
		if (name == (*i).name()) return (*i);
	}
	throw std::invalid_argument("Object name '" + name +"' is not in this ResEditor");
}

/**
 * Get object
 *
 * @param name name of the object
 * @returns ResObject with the given name
 * @throws std::invalid_argument if object with name doesn't exists
 */
const ResObject &ResEditor::object(std::string name) const
{
	for (const_iterator i = _objects.begin(); i != _objects.end(); ++i)
	{
		if (name == (*i).name()) return (*i);
	}
	throw std::invalid_argument("Object name '" + name +"' is not in this ResEditor");
}



}
}

