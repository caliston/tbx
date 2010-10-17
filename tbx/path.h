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

#include <string>
#include <vector>
#include "kernel.h"

#ifndef TBX_PATH_H
#define TBX_PATH_H

namespace tbx
{
	/**
	 * Special file type returned for a directory
	 */
	const int FILE_TYPE_DIRECTORY = 0x1000;
	/**
	 * Special file type returned for an application directory.
	 * i.e. a directory where the first character is a '!'
	 */
	const int FILE_TYPE_APPLICATION = 0x2000;

	///////////////////////////////////////////////////////////
	//@{
	//   Class to handle the 5 byte times.
	//
	//  The 5 byte time is used for time/date stamps on files
	//  on RISCOS. It is the number of centi-seconds from
	//  Midnight on 1st Jan 1900.
	//@}
	//////////////////////////////////////////////////////////

	class UTCTime
	{
	public:
		UTCTime();
		UTCTime(long long csecs);
		UTCTime(unsigned int loadAddress, unsigned int execAddress);
		UTCTime(const UTCTime &other);

		UTCTime &operator=(const UTCTime &other);

		static UTCTime now();

		std::string text() const;
		std::string text(const std::string &format) const;

		unsigned int low_word() const {return (unsigned int)(_centiseconds & 0xFFFFFFFF);}
		unsigned char high_byte() const {return (unsigned char)((_centiseconds >> 32) & 0xFF);}

		long long centiseconds() const {return _centiseconds;}

		/**
		 * Pointer to start of time in memory.
		 * This is used for calls to the OS that pass a UTC.
		 */
		unsigned char *buffer() {return (unsigned char *)&_centiseconds;}
		unsigned char *buffer() const {return (unsigned char *)&_centiseconds;}

	protected:
		long long _centiseconds;
	};

	class Path;

	////////////////////////////////////////////////////////////////
	//@{
	//   Class to hold the catalog information for a file.
	//@}
	///////////////////////////////////////////////////////////////

	class PathInfo
	{
	public:
		PathInfo();
		PathInfo(const PathInfo &other);

		PathInfo &operator=(const PathInfo &other);
		bool operator==(const PathInfo &other);
		bool operator!=(const PathInfo &other);

		bool read(const Path &path);

		enum ObjectType {NOT_FOUND, FILE, DIRECTORY, IMAGE_FILE};

		//@- Returns the child/leaf name of the object the information if for
		const std::string &name() const	{return _name;}

		// Object type
		ObjectType object_type() const;
		bool exists() const			{return (_object_type != NOT_FOUND);}
		bool file() const			{return (_object_type == FILE);}
		bool directory() const	{return (_object_type == DIRECTORY);}
		bool image_file() const	{return (_object_type == IMAGE_FILE);}

		// File type format
		bool has_file_type() const;
		int file_type() const;
		UTCTime modified_time() const;

		// Load/Executable format
		bool has_load_address() const;
		unsigned int load_address() const;
		unsigned int exec_address() const;

		// All formats
		int length() const;
		int attributes() const;

		// Iterate through a folder
		class Iterator
		{
		protected:
			Iterator(const std::string &dirName, const char *wildCard);
			friend class PathInfo;

		public:
			Iterator();
			~Iterator();

			Iterator(const Iterator &other);
			Iterator &operator=(const Iterator &other);
			bool operator==(const Iterator &other);
			bool operator!=(const Iterator &other);

			Iterator &operator++();
			Iterator operator++(int);

			PathInfo &operator*();

			void next();

		// Variables
		protected:
			PathInfo *_info;

			class IterBlock
			{
			public:
				IterBlock(const std::string &dirName, const char *wildCard);
				~IterBlock()	{delete _dirName; delete _wildCard;}

				bool next();
				const char *next_record() const	{return _nextRecord;}
				bool info(PathInfo &info);

				void add_ref() {_ref++;}
				void release() {if (--_ref == 0) delete this;}

				// Variables
				int _ref;
				_kernel_swi_regs _regs;
				char *_dirName;
				char *_wildCard;
				enum {_readSize = 2048};
				char _readData[_readSize];
				int _toRead;
				char *_nextRecord;

			} *_iterBlock;
		};

		friend class Iterator::IterBlock;

		static PathInfo::Iterator begin(const Path &path, const std::string &wildCard);
		static PathInfo::Iterator begin(const Path &path);
		static PathInfo::Iterator end();

	protected:
		std::string _name;
		ObjectType _object_type;
		unsigned int _load_address;
		unsigned int _exec_address;
		int _length;
		int _attributes;
		int _file_type;
	};

	///////////////////////////////////////////////////////////////////
	//@{
	//   Class to manipulate RISC OS file and directory path names.
	//@}
	//////////////////////////////////////////////////////////////////

	class Path
	{
	public:
		Path();
		Path(const std::string &name);
		Path(const Path &other);
		Path(const Path &other, const std::string &child);

		virtual ~Path();

		// Assignment
		Path &operator=(const Path &other);
		Path &operator=(const std::string &name);
		Path &set(const Path &other, const std::string &child);

		// Attributes
		const std::string &name() const	{return _name;}

		operator const std::string&() const;
		operator const char *() const;

		Path child(const std::string &child) const;
		Path parent() const;

		Path &down(const std::string &child);
		Path &up();
		void leaf_name(const std::string &child);
		std::string leaf_name() const;

		PathInfo::ObjectType object_type() const;

		bool path_info(PathInfo &info) const;

		bool exists() const;
		bool file() const;
		bool directory() const;
		bool image_file() const;

		// File information
		int file_type() const;
		bool file_type(int type);
		static int file_type(const std::string &file_name);
		static bool file_type(const std::string &file_name, int type);

		UTCTime modified_time() const;
//TODO:		bool modified_time(const UTCTime &utcTime);

		// Creation
		bool create_file(int type) const;
		bool create_directory() const;

		// Deletion
		bool remove() const;

		// Simple renaming
		bool rename(const std::string &new_name);

		// Copying
		enum CopyOption {
			COPY_RECURSE=1, // Recursively copy
            COPY_ALLOW_PRINT = 0x100u, // Allow copy to printer
			COPY_NO_ATTRIBUTES = 0x200u, // Don't copy attributes
			COPY_STAMP = 0x400u,  // Reset date stamp
			COPY_STRUCTURE = 0x800u, // Copy directory structure but not files
			COPY_NEWER = 0x1000u, // Copy if newer then destination only
			COPY_LOOK = 0x4000u   // Check destination first
		};

		bool copy(const std::string &copyto, unsigned int options = 0);
		bool copy(const std::string &copyto, unsigned int options, void *buffer, unsigned int size);
		bool move(const std::string &copyto, unsigned int options = 0);
		bool move(const std::string &copyto, unsigned int options, void *buffer, unsigned int size);

		// Whole file loading/saving
		char *load_file(int *length = 0) const;
		bool save_file(const char *data, int length, int file_type) const;

		bool set_current_directory() const;

		//Operators


		class Iterator
		{
		protected:
			Iterator(const std::string &dirName, const char *wildCard);
			friend class Path;

		public:
			Iterator();
			~Iterator()	{if (_iterBlock) _iterBlock->release();}

			Iterator(const Iterator &other);
			Iterator &operator=(const Iterator &other);
			bool operator==(const Iterator &other);
			bool operator!=(const Iterator &other);

			Iterator &operator++();
			Iterator operator++(int);

			std::string &operator*()	{return _name;};

			void next();

		// Variables
			std::string _name;

			class IterBlock
			{
			public:
				IterBlock(const std::string &dirName, const char *wildCard);
				~IterBlock()	{delete _dirName; delete _wildCard;}

				bool next();
				const char *next_name() const	{return _nextName;}

				void add_ref() {_ref++;}
				void release() {if (--_ref == 0) delete this;}

				// Variables
				int _ref;

				_kernel_swi_regs _regs;
				char *_dirName;
				char *_wildCard;
				enum {_readSize = 2048};
				char _readData[_readSize];
				int _toRead;
				char *_nextName;

			} *_iterBlock;
		};

		Path::Iterator begin(const std::string &wildCard);
		Path::Iterator begin();
		Path::Iterator end();

	protected:
		std::string _name;
	};

};

#endif // TBX_PATH_H
