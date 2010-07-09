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

#ifndef TBX_FONT_H
#define TBX_FONT_H

#include <string>
#include "colour.h"
#include "bbox.h"
#include "size.h"

namespace tbx
{
    /**
     * Class to handle painting and measuring text using an outline font
     */
	class Font
	{
	public:
		Font();
		Font(const std::string &font_name, int width, int height = -1);
		Font(const Font &other);
		virtual ~Font();

		// Attributes
		bool is_valid() const {return (_font_ref->handle != 0);}
		void set_colours(Colour fore, Colour back, int colourOffset = 14);
		Size size() const;
		int height() const;
		int width() const;
		Size size_os() const;
		int height_os() const;
		int width_os() const;

		// Operations
		bool find(const std::string &font_name, int width, int height = -1);
		bool desktop_font();

		enum PaintFlags {FPF_NONE = 0, FPF_JUSTIFY=1, FPF_RUBOUT= 2, FPF_OSUNITS=16};
		void paint(int x, int y, const std::string &text, int flags = Font::FPF_NONE);
		void paint(int x, int y, const char *text, int length = -1, int flags = Font::FPF_NONE);

		int string_width_mp(const std::string &text);
		int string_width_mp(const char *text, int length = -1);
		int string_width_os(const std::string &text);
		int string_width_os(const char *text, int length = -1);
		void get_char_bounds_mp(BBox &bounds, char c);
		void get_char_bounds_os(BBox &bounds, char c);

		int find_split_os(const char *text, int length, int width, int split_char =-1);
		int find_index_xy_os(const char *text, int length, int x, int y);

		BBox bounding_box() const;
		void get_bounding_box(BBox &bounds) const;

		// Operators
		Font &operator=(const Font &other);
		bool operator==(const Font &other);
		bool operator!=(const Font &other);

	private:
		class FontRef
		{
			int ref_count;
		public:
			FontRef(int handle);

			void add_ref() {ref_count++;}
			void release();

			int handle;
		protected:
			~FontRef();
		} *_font_ref;

		static FontRef *s_invalid_font_ref;
		static FontRef *s_desktop_font_ref;
	};

	// Conversion functions for points
    // Notes: assumes 180 os units per inch - also rounds down
	//        1 points is 1/72 of an inch

    inline int os_to_points(int os)              {return (os<<1) / 5;}
    inline int points_to_os(int points)         {return (5 * points) >> 1;}

    inline int os_to_points_16th(int os)          {return (os<<5) / 5;}
    inline int points_16th_to_os(int point16th)   {return (5 * point16th) >> 5;}

    inline int os_to_millipoints(int os)         {return 400 * os;}
    inline int millipoints_to_os(int millipoint) {return millipoint / 400;}

    /**
     * Class to display and measure text using the current
     * desktop font.
     */
    class WimpFont
    {
    public:
    	void set_font_colours(Colour foreground, Colour background);

    	int height_os() const {return 32;}
    	int string_width_os(const std::string &text, int num_chars = 0);

    	enum PaintFlags {WFPF_NONE, WFPF_VCENTRE = (1<<30), WFPF_RJUSTIFY = (1<<31)};
    	void paint(int x, int y, const std::string &text, int flags = WimpFont::WFPF_NONE);
    	void paint(int x, int y, const char *text, int length = -1, int flags = WimpFont::WFPF_NONE);
    };
};

#endif

