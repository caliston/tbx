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

#ifndef TBX_PROPERTYSET_H_
#define TBX_PROPERTYSET_H_

#include <string>
#include <map>
#include <iostream>

namespace tbx {

//@{
// Maintain a simple set of string properties with
// their values.
//
// The property names are case sensitive
//@}

class PropertySet {
public:

	//@{
	// Check if property is a member of this property set
	//@}
	bool exists(std::string name) const;

	//@{
	// Set the value of a string property.
	//@}
	void set(std::string name, std::string value);

	//@{
	// Get the value of a property as a string
	//
	//@return value of string property returns the default value if property does not exist
	//@}
	std::string get(std::string name, const char *def ="") const;

	//@{
	// Set the value of an integer property
	//@}
	void set(std::string name, int value);
	int get(std::string name, int def) const;

	//@{
	// Set the value of a bool property
	//@}
	void set(std::string name, bool value);
	bool get(std::string name, bool def) const;

	//@{
	// erases a property from the set.
	//
	//@return true if property existed in the set
	//@}
	bool erase(std::string name);

	//@{
	// Set an indexed property
	//
	// Short cut that just appends the index to the property name
	// before using it in the above routines.
	//@}
	void set_indexed(std::string name, int index, std::string value);
	std::string get_indexed(std::string name, int index, const char *def = "") const;
	void set_indexed(std::string name, int index, int value);
	int get_indexed(std::string name, int index, int def) const;
	void set_indexed(std::string name, int index, bool value);
	bool get_indexed(std::string name, int index, bool def) const;
	bool exists_indexed(std::string name, int index);
	bool erase_indexed(std::string name, int index);

	//@{
	// Writes the property list to a stream
	//@}
	bool write(std::ostream &os) const;

	//@{
	// Reads the properties from a stream
	//
	// Property list is assumed to continue until the end of the stream
	//
	// Note: Any string property values will have leading and
	// trailing spaces removed.
	//@}
	bool read(std::istream &is);

	//@{
	// Remove all properties from the property set
	//@}
	void clear();
	bool empty();

	bool save(std::string filename) const;
	bool load(std::string filename);

protected:
   std::map<std::string,std::string> _properties;
};

}

#endif /* TBX_PROPERTYSET_H_ */
