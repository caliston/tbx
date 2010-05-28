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
 * resobject.cpp
 *
 *  Created on: 29 Apr 2010
 *      Author: alanb
 */

#include "resobject.h"
#include <cstring>
#include <stdexcept>
#include "../tbxexcept.h"

namespace tbx {

namespace res {

/**
 * Copy constructor
 */
ResRelocationTable::ResRelocationTable(const ResRelocationTable &other)
{
	_size = other._size;
	if (_size)
	{
		_relocs = new ResRelocation[_size];
		std::memcpy(_relocs, other._relocs, sizeof(ResRelocation) * _size);
	} else
	{
		_relocs = 0;
	}
}

/**
 * Assigment
 */
ResRelocationTable &ResRelocationTable::operator=(const ResRelocationTable &other)
{
	delete [] _relocs;
	_size = other._size;
	if (_size)
	{
		_relocs = new ResRelocation[_size];
		std::memcpy(_relocs, other._relocs, sizeof(ResRelocation) * _size);
	} else
	{
		_relocs = 0;
	}

	return *this;
}

/**
 * Add an offset to the relocation table
 */
void ResRelocationTable::add(int offset, ResRelocation::Type type)
{
	ResRelocation *new_relocs = new ResRelocation[_size + 1];
	std::memcpy(new_relocs, _relocs, sizeof(ResRelocation) * _size);
	new_relocs[_size].offset= offset;
	new_relocs[_size].type = type;
	_size++;
}

/**
 * Erase offset from table
 */
void ResRelocationTable::erase(int offset)
{
	int index = 0;
	while (index < _size && _relocs[index].offset != offset) index++;
	if (index < _size)
	{
		if (_size == 1)
		{
			delete [] _relocs;
			_relocs = 0;
		} else
		{
			ResRelocation *new_relocs = new ResRelocation[_size -1];
			if (index > 0) std::memcpy(new_relocs, _relocs, index * sizeof(ResRelocation));
			if (index < _size - 1)
			{
				std::memcpy(new_relocs + index, _relocs + index + 1,
						(_size - index - 1) * sizeof(ResRelocation));
			}
		}
		_size--;
	}
}

/**
 * Clear table
 */
void ResRelocationTable::clear()
{
	delete [] _relocs;
	_relocs = 0;
	_size = 0;
}


/**
 * fix_pointers after string table has moved or had bytes inserted or deleted.
 */
void ResRelocationTable::fix_text_pointers(bool string_table, char *body, const char *new_strings, const char *old_strings, const char *from, int by)
{
	ResRelocation::Type fix_type = (string_table) ?
			ResRelocation::STRING_REF : ResRelocation::MESSAGE_REF;
	int base_diff = (int)(new_strings - old_strings);
	for (int j = 0; j < _size; j++)
	{
		if (_relocs[j].type == fix_type)
		{
			int *p =(int *)(body + _relocs[j].offset);
			if (from && ((char *)(*p)) > from) (*p)+= by;
			(*p) += base_diff;
		}
	}
}

/**
 * fix_pointers after string table has moved or had bytes inserted or deleted.
 */
void ResRelocationTable::fix_all_pointers(char *new_body, const char *old_body, const char *new_strings, const char *old_strings, const char *new_messages, const char *old_messages)
{
	int body_diff = (int)(new_body - old_body);
	int str_diff = (int)(new_strings - old_strings);
	int msg_diff = (int)(new_messages - old_messages);

	for (int j = 0; j < _size; j++)
	{
		switch(_relocs[j].type)
		{
		case ResRelocation::MESSAGE_REF:
			{
				int *p =(int *)(new_body + _relocs[j].offset);
				(*p) += msg_diff;
			}
			break;
		case ResRelocation::STRING_REF:
			{
				int *p =(int *)(new_body + _relocs[j].offset);
				(*p) += str_diff;
			}
			break;
		case  ResRelocation::OBJECT_REF:
			{
				int *p =(int *)(new_body + _relocs[j].offset);
				(*p) += body_diff;
			}
			break;

		case ResRelocation::SPRITE_AREA_REF:
			//TODO:
			break;
		}
	}
}

/**
 * Convert all the offsets in an objects body to pointers
 */
void ResRelocationTable::offsets_to_pointers(char *body, char *strings, char *msgs)
{
	for (int j = 0; j < _size; j++)
	{
		switch(_relocs[j].type)
		{
		case ResRelocation::MESSAGE_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = ((unsigned int)msgs) + *p;
			}
			break;
		case ResRelocation::STRING_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = ((unsigned int)strings) + *p;
			}
			break;
		case ResRelocation::OBJECT_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = ((unsigned int)body) + *p;
			}
			break;

		case ResRelocation::SPRITE_AREA_REF:
			//TODO:
			break;
		}
	}
}

/**
 * Convert all the pointers in an objects body to offsets
 */
void ResRelocationTable::pointers_to_offsets(char *body, char *strings, char *msgs)
{
	for (int j = 0; j < _size; j++)
	{
		switch(_relocs[j].type)
		{
		case ResRelocation::MESSAGE_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = *p - (unsigned int)msgs;
			}
			break;
		case ResRelocation::STRING_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = *p - (unsigned int)strings;
			}
			break;
		case ResRelocation::OBJECT_REF:
			{
				unsigned int *p =(unsigned int *)(body + _relocs[j].offset);
				*p = *p - (unsigned int)body;
			}
			break;
		case ResRelocation::SPRITE_AREA_REF:
			//TODO:
			break;
		}
	}
}


/**
 * Construct the data handler for a Resource object.
 *
 * Takes owner ship of all pointers passed in.
 */
ResData::ResData(ResDataHeader header, char *all_strings, int num_reloc, ResRelocation *relocs)
{
	_strings = 0;
	_messages = 0;

	if (header.string_table_offset == -1)
	{
		_strings_size = 0;
		if (header.messages_table_offset == -1)
		{
			_messages_size = 0;
		} else
		{
			_messages = all_strings;
			_messages_size = header.relocations_table_offset - header.messages_table_offset;
		}
	} else if (header.messages_table_offset == -1)
	{
		_strings = all_strings;
		_strings_size = header.relocations_table_offset - header.string_table_offset;
	} else
	{
		_messages_size = header.relocations_table_offset - header.messages_table_offset;
		_strings_size = header.relocations_table_offset - header.string_table_offset;

		// Currently we just leave messages in _strings rather than reallocate
		_strings = all_strings;
		_messages = new char[_messages_size];
		std::memcpy(_messages, all_strings + _strings_size, _messages_size);
	}
	_reloc_table._size = num_reloc;
	_reloc_table._relocs = relocs;
}

/**
 * Copy constructor
 */
ResData::ResData(const ResData &other)
{
	if (other._reloc_table._size == 0)
	{
		_messages_size = _strings_size = 0;
		_strings = 0;
		_messages = 0;
	} else
	{
		_strings_size = other._strings_size;
		_messages_size = other._messages_size;
		if (_strings_size == 0) _strings = 0;
		else
		{
			_strings = new char[_strings_size];
			std::memcpy(_strings, other._strings, _strings_size);
		}
		if (_messages_size == 0) _messages = 0;
		else
		{
			_messages = new char[_messages_size];
			std::memcpy(_messages, other._messages, _messages_size);
		}
		_reloc_table = other._reloc_table;
	}
}

/**
 * Construct an empty resource data object.
 */
ResData::ResData()
{
	_strings = 0;
	_strings_size = 0;
	_messages = 0;
	_messages_size = 0;
}

ResData::~ResData()
{
	delete [] _strings;
	delete [] _messages;
}

/**
 * Returns the text length for a text pointer at the given offset.
 */
int ResData::text_len(char *body, int offset) const
{
	int *p = (int *)(body + offset);
	if (*p == 0 || *p == -1) return 0;
	return std::strlen((char *)*p)+1;
}

/**
 * Updates a string that is held at the specified byte offset in an object
 *
 * @param body - pointer to the base of an objects body
 * @param offset - offset into body in number of bytes
 * @param new_text - new text (or null for some fields).
 * @param new_len length of new text
 * @param is_string true if its a string, false if it's a message
 */
void ResData::text(char *body, int offset, const char *new_text, int new_len, bool is_string)
{
	const char *old_text = text(body, offset);
	int old_len = 0;
	if (old_text != 0 && (int)old_text != -1) old_len = std::strlen(old_text) + 1;

	if (new_len == 0)
	{
		if (old_len == 0)
		{
			number(body, offset, ((new_text != 0) ? -1 : 0));
		} else
		{
			_reloc_table.erase(offset);
			remove_chars(body, is_string, old_text, old_len);
		}
	} else
	{
		char *p;
		new_len += 1; // Add space for terminating null
		if (old_len == new_len)
		{
			p = const_cast<char *>(old_text);
		} else if (old_len > new_len)
		{
			p = remove_chars(body, is_string, old_text, old_len - new_len);
		} else if (old_len == 0)
		{
			p = insert_chars(body, is_string,
					is_string ? (_strings + _strings_size) : (_messages + _messages_size),
					new_len);
			_reloc_table.add(offset, (is_string) ? ResRelocation::STRING_REF : ResRelocation::MESSAGE_REF);
		} else
		{
			p = insert_chars(body, is_string, old_text, new_len - old_len);
		}
		std::strcpy(p, new_text);
	}
}

/**
 * Remove characters from the string tables, updating the body pointers
 * if necessary.
 *
 * returns the location of the deletion in the new table
 */
char *ResData::remove_chars(char *body, bool string_table, const char *where, int num)
{
	char *old_strings = (string_table) ? _strings : _messages;
	int old_size = (string_table) ? _strings_size : _messages_size;
	int new_size = old_size - num;
	char *new_strings = 0;

	if (new_size > 0)
	{
		new_strings = new char[_strings_size - num];
		if (where > old_strings) std::memcpy(new_strings, old_strings, where - old_strings);
		if (where < old_strings + old_size)
		{
			std::memcpy(new_strings + (where - old_strings),
					where + num,
					(old_size - (where - _strings) - num));
		}
	}
	_reloc_table.fix_text_pointers(string_table, body, new_strings, old_strings, where, -num);

	char *new_where = new_strings + (where - old_strings);
	if (string_table)
	{
		delete [] _strings;
		_strings = new_strings;
		_strings_size = new_size;
	} else
	{
		delete [] _messages;
		_messages = new_strings;
		_messages_size = new_size;
	}

	return new_where;
}

/**
 * Insert characters in table
 *
 * @return location characters were inserted in the new table
 */
char *ResData::insert_chars(char *body, bool string_table, const char *where, int num)
{
	char *old_strings = (string_table) ? _strings : _messages;
	int old_size = (string_table) ? _strings_size : _messages_size;
	int new_size = old_size + num;
	char *new_strings = new char[new_size];
	if (where > old_strings) std::memcpy(new_strings, old_strings, where - old_strings);
	if (where < old_strings + old_size)
	{
		std::memcpy(new_strings + (where - old_strings) + num,
				where, (old_size - (where - old_strings)));
	}

	_reloc_table.fix_text_pointers(string_table, body, new_strings, old_strings, where, num);

	char *new_where = new_strings + (where - old_strings);
	if (string_table)
	{
		delete [] _strings;
		_strings = new_strings;
		_strings_size = new_size;
	} else
	{
		delete [] _messages;
		_messages = new_strings;
		_messages_size = new_size;
	}
	_strings = new_strings;

	return new_where;
}

/**
 * Copy relocations and strings into this table updating body with new
 * string pointers.
 */
void ResData::copy_used(char *body, const char *old_body, const ResRelocationTable &copy_table)
{
	delete _strings;
	delete _messages;
	_strings = 0;
	_strings_size = 0;
	_messages = 0;
	_messages_size = 0;
	_reloc_table.clear();
	if (copy_table._size != 0)
	{
		int new_relocs = 0;
		int new_strings_size = 0;
		int new_messages_size = 0;
		int len;

		// Count relocs used and calculate size of string table
		for (int j = 0; j < copy_table._size; j++)
		{
			switch(copy_table._relocs[j].type)
			{
			case ResRelocation::MESSAGE_REF:
				len = text_len(body, copy_table._relocs[j].offset);
				if (len)
				{
					new_messages_size += len;
					new_relocs++;
				}
				break;
			case ResRelocation::STRING_REF:
				len = text_len(body, copy_table._relocs[j].offset);
				if (len)
				{
					new_strings_size += len;
					new_relocs++;
				}
				break;
			case ResRelocation::OBJECT_REF:
				new_relocs++;
				break;
			case ResRelocation::SPRITE_AREA_REF:
				new_relocs++;
				break;
			}
		}

		if (new_relocs)
		{
			_reloc_table._size = new_relocs;
			_reloc_table._relocs = new ResRelocation[new_relocs];
		}
		if (new_strings_size)
		{
			_strings = new char[new_strings_size];
			_strings_size = new_strings_size;
		}

		if (new_messages_size)
		{
			_messages = new char[new_messages_size];
			_messages_size = new_messages_size;
		}

		char *pstr = _strings;
		char *pmsg = _messages;
		int offset;
		int ridx = 0;

		for (int j = 0; j < copy_table._size; j++)
		{
			offset = copy_table._relocs[j].offset;
			switch(copy_table._relocs[j].type)
			{
			case ResRelocation::MESSAGE_REF:
				len = text_len(body, offset);
				if (len)
				{
					std::strcpy(pmsg, text(body, offset));
					number(body, offset, (int)pmsg);
					pmsg += len;
					_reloc_table._relocs[ridx].type = ResRelocation::MESSAGE_REF;
					_reloc_table._relocs[ridx].offset = offset;
					ridx++;
				}
				break;
			case ResRelocation::STRING_REF:
				len = text_len(body, offset);
				if (len)
				{
					std::strcpy(pstr, text(body, offset));
					number(body, offset, (int)pstr);
					pstr += len;
					_reloc_table._relocs[ridx].type = ResRelocation::STRING_REF;
					_reloc_table._relocs[ridx].offset = offset;
					ridx++;
				}
				break;
			case ResRelocation::OBJECT_REF:
				_reloc_table._relocs[ridx].type = ResRelocation::OBJECT_REF;
				_reloc_table._relocs[ridx].offset = offset;
				{
					int *pint = (int *)(body + offset);
					*pint = (*pint - (int)old_body) + (int)body;
				}
				ridx++;
				break;
			case ResRelocation::SPRITE_AREA_REF:
				_reloc_table._relocs[ridx].type = ResRelocation::SPRITE_AREA_REF;
				_reloc_table._relocs[ridx].offset = offset;
				ridx++;
				break;
			}
		}
	}
}


/**
 * Construct an implementation as a pointer to the given object
 *
 * object_header is not owned by this object so should remain accessible
 * until the implementation object is deleted
 */
ResObjectImpl::ResObjectImpl(void *object_header) :
		_ref_count(-1),
		_header((char *)object_header),
		_data(0),
		_type_reloc_table(0)
{
}

/**
 * Construct implementation from an object and its data
 *
 * Takes ownership of the passed data
 */
ResObjectImpl::ResObjectImpl(void *object_header, ResData *data) :
		_ref_count(1),
		_header((char *)object_header),
		_data(data),
		_type_reloc_table(0)
{

}

/**
 * Make copy of other implementation
 */
ResObjectImpl::ResObjectImpl(const ResObjectImpl &other)
{
	if (other._ref_count == -1)
	{
		_ref_count = -1;
		_header = other._header;
		_data = 0;
	} else
	{
		_ref_count = 1;
		int hdr_and_body_size = other.header()->body_size + (other.header()->body - other._header);
		_header = new char[hdr_and_body_size];
		std::memcpy(_header, other._header, hdr_and_body_size);
		header()->body = _header + (other.header()->body - other._header);
		if (other._data)
		{
			_data = new ResData(*(other._data));
			_data->_reloc_table.fix_all_pointers(header()->body, other.header()->body,
						_data->_strings, other._data->_strings,
						_data->_messages, other._data->_messages);
		}
		else _data = 0;
	}
	_type_reloc_table = other._type_reloc_table;
}

/**
 * Make copy of other implementation
 *
 * @param other Other item to copy
 * @param make a copy of a read-only item. Requires other to have a type_reloc_table
 * @param copy_read_only - make a copy of read-only ResObjects as well if possible
 * @throws std::invalid_argument if other doesn't have a type_reloc_table and
 * is read only
 */
ResObjectImpl::ResObjectImpl(const ResObjectImpl &other, bool copy_read_only)
{
	if (other._ref_count == -1 && copy_read_only == false)
	{
		_ref_count = -1;
		_header = other._header;
		_data = 0;
	} else
	{
		_ref_count = 1;
		int hdr_and_body_size = other.header()->body_size + (other.header()->body - other._header);
		_header = new char[hdr_and_body_size];
		std::memcpy(_header, other._header, hdr_and_body_size);
		header()->body = _header + (other.header()->body - other._header);
		if (other._data)
		{
			_data = new ResData(*(other._data));
			_data->_reloc_table.fix_all_pointers(header()->body, other.header()->body,
					_data->_strings, other._data->_strings,
					_data->_messages, other._data->_messages);

		} else if (other._ref_count == -1)
		{
			// Copy read only body using type_reloc_table
			if (other._type_reloc_table == 0)
			{
				std::string msg("Unable to make ResObject '");
				msg += header()->name;
				// Can only do this if we have a type_reloc_table
				delete [] _header;
				_header = 0;
				msg += "' Writeable. It is read-only without a type relocation table.";
				throw std::invalid_argument(msg);
			} else
			{
				_data = new ResData();
				_data->copy_used(header()->body, other.header()->body, *(other._type_reloc_table));
			}
		} else _data = 0;
	}
	_type_reloc_table = other._type_reloc_table;
}

/**
 * Destructor deletes non read-only resources
 */
ResObjectImpl::~ResObjectImpl()
{
	if (_ref_count != -1) delete [] _header;
	delete _data;
}

/**
 * Add reference
 */
void ResObjectImpl::add_ref()
{
	if (_ref_count == -1) throw std::invalid_argument("ResObjectImpl: Adding reference to a read_only object");
	_ref_count++;
}

/**
 * Release object, deleting if no longer used by anything
 */
void ResObjectImpl::release()
{
	if (_ref_count == -1 || --_ref_count == 0) delete this;
}

/**
* Construct from raw object header
*
* The object owns the passed pointers and will delete them
* when it is deleted.
*
* @param header - pointer to object header
* @param data - pointer to resource data (or 0 if no data)
*/
ResObject::ResObject(void *object_header, ResData *data)
{
	_impl = new ResObjectImpl(object_header, data);
}

/**
 * Copy constructor
 */
ResObject::ResObject(const ResObject &other)
{
   if (other._impl->read_only()) _impl = new ResObjectImpl(*(other._impl));
   else
   {
	   _impl = other._impl;
	   _impl->add_ref();
   }
}

/**
 * Destructor
 */
ResObject::~ResObject()
{
	if (_impl) _impl->release();
}

/**
 * Assignment
 */
ResObject &ResObject::operator=(const ResObject &other)
{
	ResObjectImpl *impl = _impl;
	if (other._impl->read_only()) _impl = new ResObjectImpl(*(other._impl));
	else
	{
		_impl = other._impl;
		_impl->add_ref();
	}
	impl->release();

	return *this;
}

/**
 * Load a resource object from a stream
 *
 * @param file binary stream to read the object from
 * @returns pointer to loaded object or 0 if load failed
 */
ResObject *ResObject::load(std::istream &file)
{
	ResDataHeader data_header;
	ResObjectHeader object_header;

	file.read((char *)&data_header, sizeof(ResDataHeader));
	if (file.fail()) return 0;
	file.read((char *)&object_header, sizeof(ResObjectHeader));
	if (file.fail()) return 0;

	// Read object body
	char *data = new char[(int)object_header.body + object_header.body_size];
	std::memcpy(data, &object_header, sizeof(ResObjectHeader));
	file.read(data + sizeof(ResObjectHeader), (int)object_header.body + object_header.body_size - sizeof(ResObjectHeader));

	// Make body offset to real pointer
	reinterpret_cast<ResObjectHeader *>(data)->body += (int)data;

	ResData *res_data = 0;

	if (data_header.relocations_table_offset != -1)
	{
		int strings_size = object_header.total_size - (object_header.body - data) - object_header.body_size;

		// Read in strings
		char *strings = 0;
		if (strings_size)
		{
			strings = new char[strings_size];
			file.read(strings, strings_size);
		}

		int numRelocs = 0;
		ResRelocation *relocs = 0;
		file.read((char *)&numRelocs,4);

		if (file)
		{
			relocs = new ResRelocation[numRelocs];
			file.read((char *)relocs, numRelocs * sizeof(ResRelocation));
		}
		if (file)
		{
			// ResData owns strings and relocs after the following line
			res_data = new ResData(data_header, strings, numRelocs, relocs);
		} else
		{
			delete [] data;
			delete [] strings;
			delete [] relocs;
			return 0;
		}
	}

	ResObject *obj = new ResObject(data, res_data);
	if (res_data) res_data->_reloc_table.offsets_to_pointers(obj->header()->body, res_data->_strings, res_data->_messages);

	return obj;
}

/**
 * Save a resource object to a stream
 *
 * @param file binary stream to save object to
 * @returns true if save successful
 */
bool ResObject::save(std::ostream &file)
{
	ResDataHeader data_header;
	ResData *data = _impl->_data;
	int body_offset = header()->body - _impl->_header;
	data_header.messages_table_offset = -1;
	data_header.string_table_offset = -1;
	if (data == 0 || data->_reloc_table._size == 0)
	{
		data_header.relocations_table_offset = -1;
	} else
	{
		int table_pos = body_offset + header()->body_size;
		if (data->_strings_size)
		{
			data_header.string_table_offset = table_pos;
			table_pos += data->_strings_size;
		}
		if (data->_messages_size)
		{
			data_header.messages_table_offset = table_pos;
			table_pos += data->_messages_size;
		}
		data_header.relocations_table_offset = table_pos;
		header()->total_size = table_pos;

		// Change to offsets for save
		data->_reloc_table.pointers_to_offsets(header()->body, data->_strings, data->_messages);
	}
	// Resource header
	file.write((char *)&data_header, sizeof(ResDataHeader));

	// Object header
	header()->body = (char *)body_offset; // Make offset for save
	file.write(_impl->_header, body_offset + header()->body_size);
	header()->body = _impl->_header + body_offset; // Restore body pointer

	// Object data tables
	if (data)
	{
		if (data->_strings_size) file.write(data->_strings, data->_strings_size);
		if (data->_messages_size) file.write(data->_messages, data->_messages_size);
		if (data->_reloc_table._size)
		{
			file.write((char *)&(data->_reloc_table._size), 4);
			file.write((char *)&(data->_reloc_table._relocs), sizeof(ResRelocation) * data->_reloc_table._size);

			// Put offsets back to pointers
			data->_reloc_table.offsets_to_pointers(header()->body, data->_strings, data->_messages);
		}
	}

	return file.good();
}

/**
 * Make this ResObject writeable.
 *
 * @throws std::invalid_argument if object is read only without a type relocation table.
 */
void ResObject::make_writeable()
{
	if (_impl->_ref_count == 1) return; // Nothing to do

	ResObjectImpl *impl = _impl;
	_impl = new ResObjectImpl(*impl, true); // Make a copy
	impl->release();
}

/**
 * Set the name of the object
 */
void ResObject::name(std::string name)
{
	make_writeable();
	name.copy(header()->name, 12);
	int size = name.size();
	if (size > 11) size = 11;
	header()->name[size] = 0;
}

/**
 * Check if this objects class id is as specified.
 *
 * @param class_id the toolbox class id to check against
 * @throws ObjectClassError if the class id does not match this objects class id
 */
void ResObject::check_class_id(int class_id) const
{
	if (header()->class_id != class_id) throw ObjectClassError();
}


/**
 * Return a string at the given offset
 */
const char *ResObject::string(int offset) const
{
	return (char *)(*((int *)(header()->body, offset)));
}

/**
 * Set a string at the given offset
 */
void ResObject::string(int offset, const char *value)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, ((value == 0) ? 0 : std::strlen(value)), true);
}
/**
 * Set a string at the given offset
 */
void ResObject::string(int offset, std::string value)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, true);
}

/**
 * Assign a string where the length is at offset+4
 *
 * @param offset - offset into body of string
 * @param value - new value for the string
 * @param length - new length or -1 to use existing length.
 * The length is always adjusted so it is big enough for the value.
 */
void ResObject::string_with_length(int offset, const char *value, int length /*= -1*/)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, true);
	int text_len = std::strlen(value) + 1;
	if (length == -1) length = number(offset+4);
	if (length < text_len) length = text_len;
	number(offset+4, length);
}

/**
 * Get the message string at the given offset
 */
const char *ResObject::message(int offset) const
{
	return (char *)(*((int *)(header()->body, offset)));
}

/**
 * Set a message at the given offset
 */
void ResObject::message(int offset, const char *value)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, ((value == 0) ? 0 : std::strlen(value)), false);
}

/**
 * Set a message at the given offset
 */
void ResObject::message(int offset, std::string value)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, false);
}

/**
 * Assign a message where the length is at offset+4
 *
 * @param offset - offset into body of message
 * @param value - new value for the message
 * @param length - new length or -1 to use existing length.
 * The length is always adjusted so it is big enough for the value.
 */
void ResObject::message_with_length(int offset, const char *value, int length/* = -1*/)
{
	make_writeable();
	if (_impl->_data == 0) _impl->_data = new ResData();
	_impl->_data->text(header()->body, offset, value, false);
	int text_len = std::strlen(value) + 1;
	if (length == -1) length = number(offset+4);
	if (length < text_len) length = text_len;
	number(offset+4, length);
}

/**
 * Get a number from the specified offset
 */
int ResObject::number(int offset) const
{
	return *((int *)(header()->body + offset));
}

/**
 * Set a number at the specified offset
 */
void ResObject::number(int offset, int value)
{
	*((int *)(header()->body + offset)) = value;
}

}
}
