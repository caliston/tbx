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



#include "sprite.h"
#include "modeinfo.h"
#include "path.h"

#include "swis.h"
#include "swixcheck.h"

#include <sstream>

using namespace tbx;

/**
 * Plot sprite to screen.
 *
 * This is the Image::plot override so it just adds
 * the SPA_USE_MASK code to the normal plot of
 * the derived class.
 */

void Sprite::plot(const Point &pos, int code) const
{
	plot_screen(pos, code | SPA_USE_MASK);
}

/********************************************************/
/*                                                      */
/*   Function: size()                                  */
/*                                                      */
/*   Purpose:  get os size of sprite                    */
/*                                                      */
/*   Returns:  size of sprite in os units               */
/*                                                      */
/********************************************************/

Size Sprite::size() const
{
   Size sz(0,0);
   int mode;

   if (info(&sz, &mode))
   {
      ModeInfo mi(mode);
      sz.width = sz.width << mi.eig().x;
      sz.height = sz.height << mi.eig().y;
   }

   return sz;
}

/**
 * Return width of sprite in OS units
 */
int Sprite::width() const
{
    Size sz;
	int x = 0;
    int mode;

    if (info(&sz, &mode))
    {
		ModeInfo mi(mode);
		x = sz.width << mi.eig().x;
    }

	return x;
}

/**
 * Return height of sprite in OS Units
 */
int Sprite::height() const
{
    Size sz;
    int mode;
	int y = 0;

	if (info(&sz, &mode))
	{
		ModeInfo mi(mode);
		y = sz.height << mi.eig().y;
	}

	return y;
}


/**
 *   Construct an unassigned sprite
 *
 */
UserSprite::UserSprite() : _area(0), _offset(0)
{
}

/**
 * Private constructor used by sprite area to return a sprite
 */
UserSprite::UserSprite(SpriteArea *area, int offset)
{
	_area = area;
	_offset = offset;
}

/**
 * Construct a user sprite from a sprite area and name
 *
 * @throws SpriteException if name is not in the sprite area.
 */
UserSprite::UserSprite(SpriteArea *area, const std::string &name)
{
   *this = area->get_sprite(name);
   if (!is_valid())
   {
      std::string message("Unable to find sprite '");
      message += name;
      message += "'";
      throw SpriteException(name);
   }
}

/**
 * Destructor - not this does not delete the sprite from the
 * sprite area as this class is just a reference to the sprite.
 */
UserSprite::~UserSprite()
{
}

/**
 * Renames the sprite.
 *
 * Returns true if successful
 */
bool UserSprite::rename(const std::string &name)
{
        return _area->rename(*this, name);
}

/**
 * Plot this sprite at the given position with no scaling
 * or colour conversions.
 *
*/

void UserSprite::plot_raw(const Point &pos, int code /* = 8 */) const
{
  _kernel_swi_regs in;

  in.r[0] = 34 + 512;
  in.r[1] = (int)(_area->pointer());
  in.r[2] = (int)pointer();
  in.r[3] = pos.x;
  in.r[4] = pos.y;
  in.r[5] = code;

  swix_check(_kernel_swi(OS_SpriteOp, &in, &in));
}

/**
 * Plot this sprite scaled using the given colour
 * translation.
 *
 * \arg x os x position for plot
 * \arg y os y position for plot
 * \arg sf scale factors (0 = don't scale)
 * \arg table colour translation table (0 = don't translate colours)
 */
void UserSprite::plot_scaled(const Point &pos, const ScaleFactors *sf, const TranslationTable *tbl, int code /*=8*/) const
{
  _kernel_swi_regs in;

  in.r[0] = 52 + 512;
  in.r[1] = (int)(_area->pointer());
  in.r[2] = (int)pointer();
  in.r[3] = pos.x;
  in.r[4] = pos.y;
  in.r[5] = code;
  in.r[6] = (int)sf;
  in.r[7] = (int)(tbl ? tbl->data() : 0);

  if (_offset) swix_check(_kernel_swi(OS_SpriteOp, &in, &in));
}

/**
 * Plot sprite to screen.
 *
 * Calculates the translation table and scale factors for
 * the current screen and plots the sprite
 */

void UserSprite::plot_screen(const Point &pos, int code /* = 8*/) const
{
    TranslationTable table;
    ScaleFactors scale;

    table.create(this);
    get_wimp_scale(scale);

    plot_scaled(pos, &scale, &table);
}


/********************************************************/
/*                                                      */
/*   Function: get_name                                 */
/*                                                      */
/*   Purpose:  get name of sprite                       */
/*                                                      */
/*   Note:   name must be 13 characters long            */
/*                                                      */
/********************************************************/

std::string UserSprite::name() const
{
    char name[13];
	strncpy(name, (char *)(pointer() + 1), 12);
	name[12] = 0;
	return std::string(name);
}

/********************************************************/
/*                                                      */
/*   Function: get_info                                 */
/*                                                      */
/*   Purpose:  get details on sprite                    */
/*                                                      */
/********************************************************/

bool UserSprite::info(Size *pixel_size, int *mode /* = NULL */ , bool *mask /* = NULL */) const
{
	bool ok = false;

	_kernel_swi_regs in, out;

	in.r[0] = 40 + 512;
	in.r[1] = (int)(_area->pointer());
	in.r[2] = (int)pointer();

	if (_kernel_swi(OS_SpriteOp, &in, &out) == NULL)
	{
		ok = true;
		if (pixel_size)
		{
			pixel_size->width = out.r[3];
			pixel_size->height = out.r[4];
		}
		if (mask) *mask = (out.r[5] != 0);
		if (mode) *mode = out.r[6];
	}

	if (!ok)
	{
		if (pixel_size) *pixel_size = Size();
		if (mask) *mask = false;
		if (mode) *mode = -1;
	}

	return ok;
}

/***************************************************/
/*                                                */
/*   Function: get_palette                        */
/*                                                */
/*   Purpose:  Get palette (non-flashing colours  */
/*             only) for this sprite              */
/*                                                */
/*   Returns:  TRUE is sprite has a palette       */
/*                                                */
/**************************************************/


bool UserSprite::get_palette(ColourPalette &pal) const
{
   int *pos = pointer();

   if (!pos || pos[8] == 0x2C) return false;

   ModeInfo mi(pos[10]);
   Colour *col = (Colour *)(pos + 11);

   pal.resize(mi.colours());
   for (int j = 0; j < mi.colours(); j++)
      pal.entry(j, col[j * 2]);

   return true;
}

//@{
//   Get scaling required to plot this sprite in the wimp
//@}
void UserSprite::get_wimp_scale(ScaleFactors &factor) const
{
	_kernel_swi_regs regs;

	regs.r[0] = 512; // Sprite area specified
	regs.r[1] = (int)_area->pointer();
	regs.r[2] = (int)pointer();
	regs.r[6] = (int)(factor.as_array());
	regs.r[7] = 0;
	_kernel_swi(0x400ED, &regs, &regs);
}

/**
 * Construct an uninitialise sprite area.
 */
SpriteArea::SpriteArea()
{
   _area = 0;
   _owns_area = true;
}

/**
 * Construct a SpriteArea from a RISC OS sprite area pointer.
 *
 * @param data RISC OS sprite area pointer
 * @param ownsarea true if the SpriteArea now looks after the area
 */
SpriteArea::SpriteArea(OsSpriteAreaPtr data, bool ownsarea /* = false*/)
{
   _area = data;
   _owns_area = ownsarea;
}

/**
 * Construct an empty sprite area with the initial capacity
 *
 * @param size size of area in bytes.
 */
SpriteArea::SpriteArea(int size)
{
	_owns_area = false;
	_area = 0;
	initialise(size);
	_owns_area = true;
}

/**
 * Destructor deletes the RISC OS sprite area if it owns it.
 *
 */
SpriteArea::~SpriteArea()
{
   if (_owns_area) delete [] _area;
}

/**
 * Constructs a sprite area that is a copy
 * of the given area.
 */
SpriteArea::SpriteArea(const SpriteArea &other)
{
   _owns_area = true;
   _area = 0;
   if (other._area != 0)
   {
      int size = other._area[0];
      _area = new int[(size>>2) + 1];
      memcpy(_area, other._area, size);
   }
}

/**
 * Deletes current area and replaces it with a copy of
 * the given area.
 */
SpriteArea::SpriteArea &SpriteArea::operator=(const SpriteArea &other)
{
    if (_owns_area) delete [] _area;
    _area = 0;
    _owns_area = true;
   if (other._area != 0)
   {
      int size = other._area[0];
      _area = new int[(size>>2) + 1];
      memcpy(_area, other._area, size);
   }

   return *this;
}

/**
 * Get a sprite from this sprite area by name.
 *
 * Check is_valid on returned sprite to check it was found.
 *
 * @return UserSprite. If the name is not found sprite returned is not valid.
 */
UserSprite SpriteArea::get_sprite(const std::string &name)
{
	_kernel_swi_regs in, out;

	if (_area)
	{
		in.r[0] = 24 + 256;
		in.r[1] = (int)_area;
		in.r[2] = (int)name.c_str();

		if (_kernel_swi(OS_SpriteOp, &in, &out) == NULL)
		{
			return UserSprite(this, out.r[2] - (int)_area);
		}
	}

    return UserSprite();
}

//{
//    Create a new sprite.
//
//@param name name of sprite (max length 12)
//@param width Width of sprite in OS units
//@param height Height of sprite in OS units
//@param mode - Use SpriteFormat enum or value returned from SpriteMode
//@param palette - true to create with a palette
//@returns Pointer to sprite if successful or 0
//@}

UserSprite SpriteArea::create_sprite(const std::string &name, int width, int height, int mode, bool palette /* = false */)
{
	ModeInfo mi(mode);
	width  >>= mi.eig().x;
	height >>= mi.eig().y;
	return create_sprite_pixels(name, width, height, mode, palette);
}

//{
//    Create a new sprite, size measured in pixels
//
//@param name name of sprite (max length 12)
//@param width Width of sprite in pixels
//@param height Height of sprite in pixels
//@param mode - Use SpriteFormat enum or value returned from SpriteMode
//@param palette - true to create with a palette
//@returns Pointer to sprite if successful or 0
//@}

UserSprite SpriteArea::create_sprite_pixels(const std::string &name, int width, int height, int mode, bool palette /* = false */)
{
  _kernel_swi_regs in, out;
  int sprite_size = calculate_memory(width, height, mode, palette);
  bool okToCreate = false;

  if (_area)
  {
	  okToCreate = (sprite_size <= free_space() || resize(_area[0] + sprite_size));
  } else
	  okToCreate = initialise(sprite_size+16);

  if (okToCreate)
  {
	  in.r[0] = 15 + 256;
	  in.r[1] = (int)_area;
	  in.r[2] = (int)name.c_str();
	  in.r[3] = palette;
	  in.r[4] = width;
	  in.r[5] = height;
	  in.r[6] = mode;

	  if (_kernel_swi(OS_SpriteOp, &in, &out) == NULL)
	  {
		  return get_sprite(name);
	  }
  }

  return UserSprite();
}

//@{
//  Calculate the memory required for a sprite.
//
//@}

int SpriteArea::calculate_memory(int width, int height, int mode, bool withPalette)
{
	int bitsPerPixel = get_bits_per_pixel(mode);
	int bitsPerLine  = width * bitsPerPixel;
	int bytesPerLine = (width * bitsPerPixel) >> 3;
	if (bitsPerLine & 8) bytesPerLine++;

	int wordsPerLine = bytesPerLine >> 2;
	int spriteImageSize = 0;

	if (bytesPerLine & 3) wordsPerLine++;
	spriteImageSize = (wordsPerLine * height) << 2;

	int spriteSize = 44; // Header size

	spriteSize += spriteImageSize;

	if (withPalette && bitsPerPixel <= 8)
	{
		spriteSize += (1 << bitsPerPixel) * 4;
	}

	return spriteSize;
}

int SpriteArea::calculate_mask_size(int width, int height, int mode)
{
	int bitsPerPixel = get_bits_per_pixel(mode);

	// 32K and 16M sprites only use 1bpp masks
	if (bitsPerPixel > 8) bitsPerPixel = 1;

	int bitsPerLine  = width * bitsPerPixel;
	int bytesPerLine = (width * bitsPerPixel) >> 3;
	if (bitsPerLine & 8) bytesPerLine++;

	int wordsPerLine = bytesPerLine >> 2;

	if (bytesPerLine & 3) wordsPerLine++;

	return (wordsPerLine * height) << 2;
}


int SpriteArea::get_bits_per_pixel(int mode)
{
	int bitsPerPixel = 32;

	if (mode & 1)
	{
		switch(SpriteColours(mode >> 27))
		{
		case SC_Colour2: bitsPerPixel = 1; break;
		case SC_Colour4: bitsPerPixel = 2; break;
		case SC_Colour16: bitsPerPixel = 4; break;
		case SC_Colour256: bitsPerPixel = 8; break;
		case SC_Colour32K: bitsPerPixel = 16; break;
		case SC_Colour16M: bitsPerPixel = 32; break;
		}
	} else
	{
		switch(SpriteFormat(mode))
		{
			case SF_Colour2dpi90x45:
			case SF_Colour2dpi45:
			case SF_Colour2dpi90:
				bitsPerPixel = 1;
				break;

			case SF_Colour4dpi45:
			case SF_Colour4dpi90x45:
			case SF_Colour4dpi90:
				bitsPerPixel = 2;
				break;

			case SF_Colour16dpi45:
			case SF_Colour16dpi90x45:
			case SF_Colour16dpi90:
				bitsPerPixel = 4;
				break;

			case SF_Colour256dpi45:
			case SF_Colour256dpi90x45:
			case SF_Colour256dpi90:
				bitsPerPixel = 8;
				break;
		}
	}

	return bitsPerPixel;
}

// Get count of sprites in area

int SpriteArea::sprite_count() const
{
  _kernel_swi_regs regs;
  int count = 0;

  if (_area)
  {
     regs.r[0] = 8 + 512;
     regs.r[1] = (int)_area;

     swix_check(_kernel_swi(OS_SpriteOp, &regs, &regs));
	 count =  regs.r[3];
  }

   return count;
}

// Initialise a new user sprite area

bool SpriteArea::initialise(int size)
{
    if (_owns_area) delete [] _area;
    _area = new int[(size>>2) + 1];
    _area[0] = size;
    _area[2] = 16; // Offset to first sprite

    _kernel_swi_regs regs;

    regs.r[0] = 9 + 256;
    regs.r[1] = (int)_area;

    if (_kernel_swi(OS_SpriteOp, &regs, &regs) == NULL)
	{
		return true;
	} else
	{
		delete [] _area;
		_area = 0;
		return false;
	}
}

bool SpriteArea::load(const std::string &file_name)
{
	_kernel_swi_regs regs;

	// get size of file
	regs.r[0] = 5;
	regs.r[1] = (int)file_name.c_str();
	if (_kernel_swi(OS_File, &regs,&regs) == NULL)
	{
		int fileSize = regs.r[4];
		fileSize += 16;
		if (fileSize < size() || initialise(fileSize))
		{
			regs.r[0] = 10 + 512;
			regs.r[1] = (int)_area;
			regs.r[2] = (int)file_name.c_str();
			if (_kernel_swi(OS_SpriteOp, &regs, &regs) == NULL)
			{
				return true;
			}
		}
	}
	return false;
}


// Note: any pointers will no longer be valid

bool SpriteArea::merge(const std::string &file_name)
{
   _kernel_swi_regs regs;

   // get size of file
   regs.r[0] = 17;
   regs.r[1] = (int)file_name.c_str();
   if (_kernel_swi(OS_File, &regs,&regs) == NULL)
   {
	   int fileSize = regs.r[4];

	   if (fileSize < free_space() || resize(size() + fileSize))
	   {
		   regs.r[0] = 11 + 512;
		   regs.r[1] = (int)_area;
		   regs.r[2] = (int)file_name.c_str();
		   if (_kernel_swi(OS_SpriteOp, &regs, &regs) == NULL)
		   {
			   return true;
		   }
	   }
   }

   return false;
}

bool SpriteArea::save(const std::string &file_name) const
{
   bool saved = false;

   if (_area)
   {
      _kernel_swi_regs regs;
      regs.r[0] = 12 + 512;
      regs.r[1] = (int)_area;
      regs.r[2] = (int)file_name.c_str();
      saved = (_kernel_swi(OS_SpriteOp, &regs, &regs) == NULL);
   }

   return saved;
}

int SpriteArea::size() const
{
   return (_area) ? _area[0] : 0;
}

int SpriteArea::free_space() const
{
    return (_area) ? (_area[0] - _area[3]) : 0;
}

bool SpriteArea::resize(int newSize)
{
   bool ok = false;

   if (_area)
   {
	   // Can only resize if we own it
	   if (_owns_area)
	   {
	      int *temp = _area;
		  _owns_area = false;
		  ok = initialise(newSize);
		  _owns_area = true;
		  if (ok)
		  {
			  memcpy(_area+1, temp+1, temp[3] - 4);
			  delete [] temp;
		  } else
			  _area = temp;
	   }

   } else
   {
	   ok = initialise(newSize+16);
	   _owns_area = true;
   }

   return ok;
}


bool SpriteArea::rename(UserSprite &s, const std::string &name)
{
	if (!_area) return false;

    _kernel_swi_regs in, out;
	in.r[0] = 26 + 512;
	in.r[1] = (int)(pointer());
	in.r[2] = (int)s.pointer();
	in.r[3] = (int)name.c_str();

	if (_kernel_swi(OS_SpriteOp, &in, &out) == NULL)
	{
		return true;
	}

	return false;
}

bool SpriteArea::erase(UserSprite &s)
{
    if (!_area) return false;

    if (_swix(OS_SpriteOp, _INR(0,2), 25 + 512, _area, s.pointer()) == NULL)
    {
       return true;
    }

    return false;
}

/**
 * Erase the a sprite from the sprite area.
 *
 * @param name Name of sprite to erase
 * @returns true if sprite was erased.
 */
bool SpriteArea::erase(const std::string name)
{
    if (!_area) return false;

    if (_swix(OS_SpriteOp, _INR(0,2), 25 + 256, _area, name.c_str()) == NULL)
    {
       return true;
    }

    return false;
}


// Create translation table to current mode

bool TranslationTable::create(int mode, const ColourPalette *pal)
{
    bool ok = false;
    initialise(mode);

    if (_table)
    {
       _kernel_swi_regs regs;

       regs.r[0] = mode;
       regs.r[1] = (int)(pal ? pal->address() : 0 );
       regs.r[2] = -1;
       regs.r[3] = -1;
       regs.r[4] = (int)_table;
       regs.r[5] = 0;

       ok = (_kernel_swi(ColourTrans_SelectTable, &regs, &regs) == 0);
    }

    return ok;
}

bool TranslationTable::create(const UserSprite *s)
{
   int numColours = initialise(s->mode());
   bool ok = false;

   if (_table)
   {
      _kernel_swi_regs regs;
      if (s->has_palette() || numColours > 16)
      {
         regs.r[0] = (int)(s->get_sprite_area()->pointer());
         regs.r[1] = (int)(s->pointer());
         regs.r[2] = -1;
         regs.r[3] = -1;
         regs.r[4] = (int)_table;
         // The following parameter is new to RiscOS 3.1
         // bit 0 - set if pointer to sprite directly, clear if pointer to name
         // bit 1 - set to use current palette for mode, clear for default palette
         regs.r[5] = 3;
         ok = (_kernel_swi(ColourTrans_SelectTable, &regs, &regs) == 0);
      } else
      {
         regs.r[0] = 512;
         regs.r[1] = (int)(s->get_sprite_area()->pointer());
         regs.r[2] = (int)(s->pointer());
         regs.r[6] = 0;
         regs.r[7] = (int)_table;
         ok = (_kernel_swi(Wimp_ReadPixTrans, &regs, &regs) == 0);
      }
   }

   return ok;
}

bool TranslationTable::create(const WimpSprite *s)
{
   int num_colours = initialise(s->mode());
   bool ok = false;

   if (_table)
   {
      _kernel_swi_regs regs;
      if (num_colours > 16 || s->has_palette())
      {
		  _kernel_swi(Wimp_BaseOfSprites, &regs, &regs);
		  int rom_base = regs.r[0];
		  regs.r[0] = regs.r[1]; // RMA base
		  regs.r[1] = (int)(s->name().c_str());
		  regs.r[2] = -1;
		  regs.r[3] = -1;
		  regs.r[4] = (int)_table;
		  // The following parameter is new to RiscOS 3.1
		  // bit 0 - set if pointer to sprite directly, clear if pointer to name
		  // bit 1 - set to use current palette for mode, clear for default palette
		  regs.r[5] = 2;
		  ok = (_kernel_swi(ColourTrans_SelectTable, &regs, &regs) == 0);
		  if (!ok)
		  {
			  // Try ROM sprites if not in RMA
			  regs.r[0] = rom_base;
			  ok = (_kernel_swi(ColourTrans_SelectTable, &regs, &regs) == 0);
		  }
      } else
      {
          regs.r[0] = 256;
          regs.r[1] = 1; // WIMP area
          regs.r[2] = (int)(s->name().c_str());
          regs.r[6] = 0;
          regs.r[7] = (int)_table;
          ok = (_kernel_swi(Wimp_ReadPixTrans, &regs, &regs) == 0);
      }
   }

   return ok;
}


bool TranslationTable::create(UserSprite *source, UserSprite *target)
{
	_kernel_swi_regs regs;
	bool ok = false;

	regs.r[0] = (int)source->get_sprite_area()->pointer();
	regs.r[1] = (int)source->pointer();
	regs.r[2] = target->mode();
	if (target->has_palette())
	{
		ColourPalette cp;
		if (target->get_palette(cp))
		{
			regs.r[3] = (int)(cp.address());
		} else regs.r[3] = 0;
	} else
		regs.r[3] = 0;

	regs.r[4] = 0; // Get size of buffer
	regs.r[5] = 1; // r[1] is pointer to sprite

	// ColourTrans_SelectTable
	if (_kernel_swi(0x40740, &regs, &regs) == 0)
	{
		_size = regs.r[4];
		if (_table) delete [] _table;
		_table = new unsigned char[ _size ];
		regs.r[4] = (int)_table;
		if (_kernel_swi(0x40740, &regs, &regs) == 0) ok = true;
	}

	return ok;
}

//******************************************************
// Function: initialise
// Purpose:  Allocate memory for translation table
// Returns:  Number of colours in the mode
//******************************************************

int TranslationTable::initialise(int mode)
{
    ModeInfo sourceMode(mode);
    int numColours = sourceMode.colours();
    int newSize = numColours;
	if (newSize > 256) newSize = 256;

    if (newSize != _size)
    {
       if (_table) delete [] _table;
       _table = new unsigned char[ newSize ];
       if (_table) _size = newSize;
       else _size = 0;
    }

    return numColours;
}


ColourPalette::~ColourPalette()
{
  delete _palette;
}

ColourPalette::ColourPalette(int size /* = 0 */)
{
   if (_size)
   {
      _palette = new Colour[size];
      _size = size;
   } else
   {
      _palette = 0;
      _size = 0;
   }
}

void ColourPalette::resize(int newsize)
{
   if (_palette) delete [] _palette;

   _palette = new Colour[newsize];
   _size = newsize;
}

/**
 * Class to capture screen output to a sprite
 */

SpriteCapture::SpriteCapture(UserSprite *sprite, bool start_capture /*= false*/, bool to_mask /* =false*/)
{
   _sprite = sprite;

   int save_size;
   _swix(OS_SpriteOp, _INR(0,2)|_OUT(3), 62+512,
             sprite->get_sprite_area()->pointer(),
             sprite->pointer(),
             &save_size);
   _save_area = new char[save_size];
   _capturing = false;

   _save_regs[0] = (to_mask) ? 61 : 60;

   if (start_capture) capture();
}

/**
 * Sprite capture destructor. Automatically releases the current capture.
 */
SpriteCapture::~SpriteCapture()
{
    if (_capturing) release();
}

/**
 * Capture screen output to sprite or sprites mask.
 *
 * returns true if capture successful or already captured.
 */
bool SpriteCapture::capture()
{
    if (_capturing) return true;

    _capturing = (_swix(OS_SpriteOp, _INR(0,3)|_OUTR(2,3),
         _save_regs[0], // Output to mask or sprite op
         _sprite->get_sprite_area()->pointer(),
         _sprite->pointer(),
         _save_area,
         &_save_regs[1],
         &_save_regs[2],
         &_save_regs[3]
         ) == 0);

      return _capturing;
}

/**
 * Release screen output from sprite.
 *
 * returns true if successful or capture already released or not started.
 */

bool SpriteCapture::release()
{
   if (!_capturing) return true;

   _capturing = (_swix(OS_SpriteOp, _INR(0,3),
                  _save_regs[0], _save_regs[1], _save_regs[2], _save_regs[3]
               ) != 0);

   return !_capturing;
}


/**
 * Construct a wimp sprite to represent the given
 * file_type.
 *
 * Applications will be given the generic file type.
 *
 * If the sprite is not in the current wimp sprite pool
 * unknown will be used.
 *
 * @param file type to construct sprite for
 */
WimpSprite::WimpSprite(int file_type)
{
   if (file_type == tbx::FILE_TYPE_APPLICATION)
   {
	   _name = "application";
   } else if (file_type == tbx::FILE_TYPE_DIRECTORY)
   {
	   _name = "directory";
   } else
   {
	   std::ostringstream ss;
	   ss << "file_" << std::hex << file_type;
	   _name = ss.str();
	   if (!exist()) _name = "unknown";
   }
}

/**
 * Construct a wimp sprite to represent the given
 * file_type.
 *
 * Applications will be check for the name sprite first.
 *
 * If the sprite is not in the current wimp sprite pool
 * unknown will be used.
 *
 * @param file type to construct sprite for
 * @param leafname leaf name of file (used for applications)
 */
WimpSprite::WimpSprite(int file_type, std::string leafname)
{
	if (file_type == tbx::FILE_TYPE_APPLICATION)
	{
		_name = leafname;
	   if (!exist()) _name = "application";
	} else if (file_type == tbx::FILE_TYPE_DIRECTORY)
	{
		   _name = "directory";
	} else
	{
		   std::ostringstream ss;
		   ss << "file_" << std::hex << file_type;
		   _name = ss.str();
		   if (!exist()) _name = "unknown";
	}
}

/**
 * Check if this sprite exists in the Wimp sprite area.
 */
bool WimpSprite::exist() const
{
	return (_swix(Wimp_SpriteOp, _INR(0,2), 24, 0, (int)_name.c_str()) == 0);
}

/**
 * Plot this sprite at the given position with no scaling
 * or colour conversions.
 *
*/

void WimpSprite::plot_raw(const Point &pos, int code /* = 8 */) const
{
  _kernel_swi_regs in;

  in.r[0] = 34;
  in.r[1] = 0;
  in.r[2] = (int)_name.c_str();
  in.r[3] = pos.x;
  in.r[4] = pos.y;
  in.r[5] = code;

  _kernel_swi(Wimp_SpriteOp, &in, &in);
}

/**
 * Plot this sprite scaled using the given colour
 * translation.
 *
 * \arg x os x position for plot
 * \arg y os y position for plot
 * \arg sf scale factors (0 = don't scale)
 * \arg table colour translation table (0 = don't translate colours)
 */
void WimpSprite::plot_scaled(const Point &pos, const ScaleFactors *sf, const TranslationTable *tbl, int code /*=8*/) const
{
  _kernel_swi_regs in;

  in.r[0] = 52;
  in.r[1] = 0;
  in.r[2] = (int)_name.c_str();
  in.r[3] = pos.x;
  in.r[4] = pos.y;
  in.r[5] = code;
  in.r[6] = (int)sf;
  in.r[7] = (int)(tbl ? tbl->data() : 0);

  swix_check(_kernel_swi(Wimp_SpriteOp, &in, &in));
}

/**
 * Plot sprite to screen.
 *
 * Calculates the translation table and scale factors for
 * the current screen and plots the sprite
 */

void WimpSprite::plot_screen(const Point &pos, int code /* = 8*/) const
{
    TranslationTable table;
    ScaleFactors scale;

    table.create(this);
    get_wimp_scale(scale);

    plot_scaled(pos, &scale, &table, code);
}

//@{
//   Get scaling required to plot this sprite in the wimp
//@}
void WimpSprite::get_wimp_scale(ScaleFactors &factor) const
{
	_kernel_swi_regs regs;

	regs.r[0] = 256; // Sprite area specified
	regs.r[1] = 1;   // Sprite in wimp sprite area
	regs.r[2] = (int)_name.c_str();
	regs.r[6] = (int)(factor.as_array());
	regs.r[7] = 0;
	_kernel_swi(Wimp_ReadPixTrans, &regs, &regs);
}

/**
 * Check if WimpSprite is using a palette
 */
bool WimpSprite::has_palette() const
{
    _kernel_swi_regs regs;
    regs.r[0] = 37;
    regs.r[1] = 1;
    regs.r[2] = (int)(_name.c_str());
    regs.r[3] = -1;
    _kernel_swi(Wimp_SpriteOp, &regs, &regs);
    return (regs.r[4] != 0); // palette pointer
}

/********************************************************/
/*                                                      */
/*   Function: get_info                                 */
/*                                                      */
/*   Purpose:  get details on sprite                    */
/*                                                      */
/********************************************************/

bool WimpSprite::info(Size *pixelSize, int *mode /* = NULL */ , bool *mask /* = NULL */) const
{
	bool ok = false;

	_kernel_swi_regs in, out;

	in.r[0] = 40;
	in.r[1] = 0;
	in.r[2] = (int)_name.c_str();

	if (_kernel_swi(Wimp_SpriteOp, &in, &out) == NULL)
	{
		ok = true;
		if (pixelSize)
		{
			pixelSize->width = out.r[3];
			pixelSize->height= out.r[4];
		}
		if (mask) *mask = (out.r[5] != 0);
		if (mode) *mode = out.r[6];
	}

	if (!ok)
	{
		if (pixelSize) *pixelSize = Size();
		if (mask) *mask = false;
		if (mode) *mode = -1;
	}

	return ok;
}

