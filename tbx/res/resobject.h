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
 * resobject.h
 *
 *  Created on: 29 Apr 2010
 *      Author: alanb
 */

#ifndef RESOBJECT_H_
#define RESOBJECT_H_

#include <string>
#include <iostream>

namespace tbx {

namespace res {

class ResEditor;

/**
 * Common header for all resource objects
 */
struct ResObjectHeader
{
    int class_id;			// Object class
    int flags;			// flags
    int version;		// version * 100
	char name[12];		// Null terminated size
	int total_size;     // Total size of object including tables
	char *body;			// Pointer to body
	int body_size;	    // Size of header and body only
};

/**
 * This structure proceeds a resource object in a file to give it's data
 */
struct ResDataHeader
{
	int string_table_offset;
	int messages_table_offset;
	int relocations_table_offset;
};

/**
 * Structure for one relocation record
 */
struct ResRelocation
{
	enum Type {STRING_REF, MESSAGE_REF, SPRITE_AREA_REF, OBJECT_REF};
	Type type;
	int offset;
};

/**
 * Structure to represent a relocation table
 */
struct ResRelocationTable
{
	int _size;
	ResRelocation *_relocs;

	ResRelocationTable() : _size(0), _relocs(0) {};
	ResRelocationTable(const ResRelocationTable &other);
	~ResRelocationTable() {delete [] _relocs;}

	ResRelocationTable &operator=(const ResRelocationTable &other);

	void add(int offset, ResRelocation::Type type);
	void erase(int offset);
	void clear();

	void offsets_to_pointers(char *body, char *strings, char *msgs);
	void pointers_to_offsets(char *body, char *strings, char *msgs);
	void fix_text_pointers(bool string_table, char *body, const char *new_strings, const char *old_strings, const char *from, int by);
	void fix_all_pointers(char *new_body, const char *old_body, const char *new_strings, const char *old_strings, const char *new_messages, const char *old_messages);
};

//Notes:
// Null string or message ref = -1

class ResObject;
class ResObjectImpl;

/**
 * Class to Manipulate an objects tables.
 *
 * This is a helper object and should not be used directly
 */
class ResData
{
	char *_strings;
	int _strings_size;
	char *_messages;
	int _messages_size;
	ResRelocationTable _reloc_table;

	friend class ResObject;
	friend class ResObjectImpl;

public:
	ResData(ResDataHeader header, char *strings, int num_reloc, ResRelocation *relocs);
	ResData(const ResData &other);
	ResData();
	~ResData();

	const char *text(char *body, int offset) const {return (char *)(*(int *)(body + offset));}
	int text_len(char *body, int offset) const;
	void text(char *body, int offset, const char *new_text, int len, bool is_string);
	void text(char *body, int offset, std::string msg, bool is_string)
	{
		text(body, offset, msg.c_str(), msg.size(), is_string);
	}

	int number(char *body, int offset) const
	{
		return *((int *)(body + offset));
	}
	void number(char *body, int offset, int value)
	{
		*((int *)(body+offset)) = value;
	}

	void copy_used(char *body, const char *old_body, const ResRelocationTable &copy_table);

	//TODO:	void object_offset(char *body, int offset);
	//TODO: void sprite_area(char *body, int offset);


private:
	char *remove_chars(char *body, bool string_table, const char *where, int num);
	char *insert_chars(char *body, bool string_table, const char *where, int num);
};



/**
 * Internal Resource Object implementation class
 *
 * Use ResObject and derived classes and not this one
 *
 * TODO: Put in internal namespace or protect
 */
class ResObjectImpl
{
	int _ref_count; // Reference count or -1 if read only object
	char *_header;
	ResData *_data;
	ResRelocationTable *_type_reloc_table;

	friend class ResObject;
	friend class ResData;

private:
	~ResObjectImpl();
	ResObjectHeader *header() {return (ResObjectHeader *)_header;}
	const ResObjectHeader *header() const {return (ResObjectHeader *)_header;}

public:
	ResObjectImpl(void *object_header);
	ResObjectImpl(void *object_header, ResData *data);
	ResObjectImpl(const ResObjectImpl &other);
	ResObjectImpl(const ResObjectImpl &other, bool copy_read_only);

	void add_ref();
	void release();

	/**
	 * Check if implentation is in a read only state
	 */
	bool read_only() const {return (_ref_count == -1);}
};

/**
 * Base class for a resource object that can be edited
 */
class ResObject
{
	ResObjectImpl *_impl;

	ResObject(void *object_header, ResData *data);
public:
	ResObject(const ResObject &other);
	virtual ~ResObject();

	ResObject &operator=(const ResObject &other);

	static ResObject *load(std::istream &file);
	bool save(std::ostream &file);

	/**
	 * Return a pointer to the header information for this object
	 */
	const ResObjectHeader *header() const {return reinterpret_cast<const ResObjectHeader *>(_impl->_header);}

	/**
	 * Return pointer to name of object
	 */
	const char *name() const {return header()->name;}

	/**
	 * Set the name of the object
	 */
	void name(std::string name);

	/**
	 * Return class id of object
	 */
    int class_id() const {return header()->class_id;}
    /**
     * Return version number * 100
     */
    int version() const {return header()->version;}

	void check_class_id(int class_id) const;


protected:
	/**
	 * Return a pointer to the header information for this object
	 */
	ResObjectHeader *header() {return reinterpret_cast<ResObjectHeader *>(_impl->_header);}

	/**
	 * Make object writeable, call before any change.
	 */
	void make_writeable();

	// Helpers to get and set data in record
	const char *string(int offset) const;
	void string(int offset, const char *value);
	void string(int offset, std::string value);
	void string_with_length(int offset, const char *value, int length = -1);
	const char *message(int offset) const;
	void message(int offset, const char *value);
	void message(int offset, std::string value);
	void message_with_length(int offset, const char *value, int length = -1);
	int number(int offset) const;
	void number(int offset, int value);
};


}

}

#endif /* RESOBJECT_H_ */
