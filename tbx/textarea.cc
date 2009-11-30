/*
 * textarea.cc
 *
 *  Created on: 30-Mar-2009
 *      Author: alanb
 */

#include "textarea.h"
#include "swixcheck.h"
#include "kernel.h"


namespace tbx {

void TextArea::insert_text(int where, const std::string &text)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401c;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = where;
	regs.r[5] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void TextArea::replace_text(int start, int end, const std::string &text)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401d;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = start;
	regs.r[5] = end;
	regs.r[6] = reinterpret_cast<int>(const_cast<char *>(text.c_str() ));
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

}

void TextArea::get_selecton(int &start, int &end) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0; // Get location
	regs.r[2] = 0x401E;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
	start = regs.r[4];
	end = regs.r[5];
}

void TextArea::set_selection(int start, int end)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x401F;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = start;
	regs.r[5] = end;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

int TextArea::selection_length() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 1; // Get text
	regs.r[2] = 0x401E;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = 0;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[5];
}

std::string TextArea::selection_text() const
{
    _kernel_swi_regs regs;
    regs.r[0] = 1; // Get text
    regs.r[1] = _handle;
    regs.r[2] = 0x401E;
    regs.r[3] = _id;
    regs.r[4] = 0;
    regs.r[5] = 0;

    // Run Toolbox_ObjectMiscOp to get the size of the buffer
    swix_check(_kernel_swi(0x44ec6, &regs, &regs));

    std::string value;
    int len = regs.r[5];
    if (len)
    {
       std::auto_ptr<char> m_buffer(new char[len]);
       regs.r[4] = reinterpret_cast<int>(m_buffer.get());
       regs.r[5] = len;

       // Run Toolbox_ObjectMiscOp to get the buffer
       swix_check(_kernel_swi(0x44ec6, &regs, &regs));

       value = m_buffer.get();
    }

    return value;
}


/**
 * Set the font used for the scroll list items
 *
 * @param name The name of the font
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */
void TextArea::font(const std::string &name, int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4020;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = reinterpret_cast<int>(const_cast<char *>(name.c_str() ));
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Set the scroll list to use the system font to display the text
 *
 * @param width font width in 16ths of a point
 * @param height font height in 16ths of a point
 */

void TextArea::system_font(int width, int height)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4020;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = width;
	regs.r[6] = height;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}


void TextArea::set_colour(WimpColour foreground, WimpColour background)
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[2] = 0x4021;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = foreground;
	regs.r[4] = background;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void TextArea::set_colour(Colour foreground, Colour background)
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4021;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = foreground;
	regs.r[4] = background;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

void TextArea::get_colour(Colour &foreground, Colour &background) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 0;
	regs.r[2] = 0x4022;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
	foreground = regs.r[0];
	background = regs.r[1];
}


}
