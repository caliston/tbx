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

#ifndef tbx_sprite_h
#define tbx_sprite_h

#include "colour.h"
//TODO: Image abstraction #include "image.h"
#include "point.h"
#include "size.h"

#include <string>
#include <map>

namespace tbx
{
	class SpriteArea;
	class Sprite;
	class UserSprite;
	class WimpSprite;

	const int SPRITE_NAMELEN = 13;

	/**
	 * A class to hold a list of Colours for the a colour palette.
	 *
	 * Typically this is returned from a sprite that has < 32 thousand colours
	 */
	class ColourPalette
	{
	   public:
		  ColourPalette(int size = 0);
		  ~ColourPalette();

		  void resize(int new_size);

		  /**
		   * Set the specified index to the given colour
		   */
		  void entry(int index, const Colour &col) {_palette[index] = col;};
		  /**
		   * Return the colour for the specified index.
		   */
		  Colour entry(int index) const {return _palette[index];};

		  /**
		   * Return the size of the palette
		   */
		  int size() const       {return _size;};
		  /**
		   * Get a pointer to the array of colours
		   */
		  const Colour *address() const {return &_palette[0];};

	   private:
		  int _size;
		  Colour *_palette;
	};


	/**
	 * Class for a sprite colour translation table.
	 *
	 * The table is used to convert the colours from a sprite to
	 * the screen or from one sprite to another
	 */
	class TranslationTable
	{
	   public:
		  /**
		   * Construct an empty translation table
		   */
		  TranslationTable() {_table = 0;_size = 0;};

		  bool create(int mode, const ColourPalette *pal = NULL);
		  bool create(const UserSprite *s);
		  bool create(UserSprite *source, UserSprite *target);
		  bool create(const WimpSprite *s);

		  /**
		   * Return a pointer to the translation table data
		   */
		  unsigned char *data() const {return _table;};

	   private:
		  int initialise(int mode);

	   private:
		  unsigned char *_table;
		  int _size;
	};

	/**
	 * Class for sprite ScaleFactors.
	 *
	 * ScaleFactors are used to scale a sprite using integer values
	 *
	 * The scaling is xmult()/xdiv() for the x direction
	 * and            ymult()/ydiv() for the y direction
	 */
	class ScaleFactors
	{
		int _xmult;
		int _ymult;
		int _xdiv;
		int _ydiv;
	public:
		/**
		 * Constructor that creates the unit scale factors.
		 * i.e. no scaling
		 */
		ScaleFactors() {_xmult=_ymult=_xdiv=_ydiv=1;}
		/**
		 * Constructor allowing the multipliers and dividers to be specified.
		 */
		ScaleFactors(int xm, int ym, int xd, int yd) {_xmult = xm; _ymult = ym; _xdiv = xd; _ydiv = yd;}
		/**
		 * Constructor for common multiplier/divider in x and y directions
		 */
		ScaleFactors(int mult, int div = 1) {_xmult = _ymult = mult; _xdiv = _ydiv = div;}

		/**
		 * Returns true if scale factors do not change a value
		 */
	    bool is_unit() const {return (_xmult == _xdiv && _ymult == _ydiv);};

		int xmult() const {return _xmult;}
		ScaleFactors &xmult(int x) {_xmult=x;return *this;}
		int ymult() const {return _ymult;}
		ScaleFactors &ymult(int y) {_ymult=y;return *this;}
		int xdiv() const {return _xdiv;}
		ScaleFactors &xdiv(int x) {_xdiv=x;return *this;}
		int ydiv() const {return _ydiv;}
		ScaleFactors &ydiv(int y) {_ydiv=y;return *this;}

		/**
		 * Return 4 integer array of scale factors
		 */
		int *as_array() {return &_xmult;}
	};


	/**
	 * Standard sprite formats (colour <= 256)
	 */
	enum SpriteFormat {
		SF_Colour2dpi90x45 = 0,
		SF_Colour4dpi45 = 1,
		SF_Colour2dpi45 = 4,
		SF_Colour4dpi90x45 = 8,
		SF_Colour16dpi45 = 9,
		SF_Colour16dpi90x45 = 12,
		SF_Colour256dpi45 = 13,
		SF_Colour256dpi90x45 = 15,
		SF_Colour2dpi90 = 18,
		SF_Colour4dpi90 = 19,
		SF_Colour16dpi90 = 20,
		SF_Colour256dpi90 = 21
	};

	/**
	 * Sprite colour constant
	 */
	enum SpriteColours {
		SC_Colour2   = 1,
		SC_Colour4   = 2,
		SC_Colour16  = 3,
		SC_Colour256 = 4,
		SC_Colour32K = 5,
		SC_Colour16M = 6
	};

	//@{
	//   Return new format sprite mode.
	//
	//   Can only be used in Risc OS 3.5 and later.
	//@param colours constant to specify number of colours
	//@param horzDpi - horizontal dots per inch
	//@param vertDpi - vertical dots per inch
	//@}

	inline int sprite_mode(SpriteColours colours, int horzDpi = 90, int vertDpi = 90)
	{
		return (colours << 27) | (horzDpi << 1) | (vertDpi << 14) | 1;
	}

	/**
	 * Exception throw for sprite method failures
	 */
	class SpriteException
	{
	   std::string _message;
	   public:
	       SpriteException(const std::string &m) {_message = m;}
	       const std::string &what() const {return _message;}
	};

	/**
	 * Type for pointer to underlying RISC OS Sprite area.
	 */
	typedef int *OsSpriteAreaPtr;

	/**
	 * Constant representing the WIMP sprite area
	 */
	const OsSpriteAreaPtr WIMP_SPRITEAREA = (OsSpriteAreaPtr)1;

	/**
	 * Type for pointer to underlyin RISC OS sprite
	 */
	typedef int *OsSpritePtr;

	/**
	 * A SpriteArea holds zero or more user sprites.
	 *
	 * New sprites can be created within it.
	 */
	class SpriteArea
	{
	   public:
		  SpriteArea();
		  ~SpriteArea();
		  SpriteArea(OsSpriteAreaPtr data, bool ownsarea = false);
		  SpriteArea(int size);

          SpriteArea(const SpriteArea &other);
          SpriteArea &operator=(const SpriteArea &other);

          /**
           * Returns true if area is valid.
           */
		  bool is_valid() const	{return (_area != 0);}

		  bool initialise(int size);
		  bool load(const std::string &file_name);
		  bool merge(const std::string &file_name);
		  bool save(const std::string &file_name) const;

		  int size() const;
		  int free_space() const;
		  bool resize(int new_size);

		  int sprite_count() const;
		  UserSprite get_sprite(const std::string &name);
		  UserSprite create_sprite(const std::string &name, int width, int height, int mode, bool palette = false);
		  UserSprite create_sprite_pixels(const std::string &name, int width, int height, int mode, bool palette = false);

		  /**
		   * Return pointer to underlying sprite are that can be used when
		   * using low-level area manipulation.
		   */
		  OsSpriteAreaPtr pointer() const {return _area;};

		  bool rename(UserSprite &sprite, const std::string &newname);
		  bool erase(UserSprite &sprite);
		  bool erase(const std::string name);

		  static int calculate_memory(int width, int height, int mode, bool withPalette);
		  static int calculate_mask_size(int width, int height, int mode);
		  static int get_bits_per_pixel(int mode);

	   private:
		  OsSpriteAreaPtr _area;
		  bool _owns_area;
	};

	/**
	 * Enumeration of the plot actions for sprites.
	 */
   enum sprite_plot_action {SPA_OVERWRITE, SPA_OR, SPA_AND, SPA_XOR,
			SPA_INVERT, SPA_NONE, SPA_AND_NOT, SPA_OR_NOT, SPA_USE_MASK};

	/**
	 * Common base class for the UserSprite and WimpSprite
	 * classes.
	 */
	class Sprite
	{
	public:
		virtual ~Sprite() {};

		// Image overrides
		virtual void plot(const Point &pos, int code) const;

	   // Common sprite functions
	   virtual void plot_raw(const Point &pos, int code = SPA_USE_MASK) const = 0;
	   virtual void plot_scaled(const Point &pos, const ScaleFactors *sf, const TranslationTable *tt = NULL, int code = SPA_USE_MASK) const = 0;
	   virtual void plot_screen(const Point &pos, int code = SPA_USE_MASK) const = 0;

	   virtual std::string name() const = 0;
	   virtual bool info(Size *pixel_size, int *mode  = NULL, bool *mask = NULL) const = 0;

	   // Functions using a virtual call to the derived class
		Size size() const;
		int width() const;
		int height() const;

		Size pixel_size() const {Size ps; info(&ps); return ps;};
		int mode()       const {int m; info(NULL, &m); return m;};
		bool has_mask()      const {bool m; info(NULL, NULL, &m); return m;};

	   virtual void get_wimp_scale(ScaleFactors &factor) const = 0;
	};

	/**
	 * Sprite from a user sprite area.
	 *
	 * A sprite reference is returned using
	 * the SpriteArea::get_sprite method.
	 *
	 * A runtime error or crash will occur is you try to use a sprite
	 * after the SpriteArea it was retrieved from has been deleted.
	 */
	class UserSprite : public Sprite
	{
	   protected:
		   UserSprite(SpriteArea *area, int offset);

       public:
           UserSprite();
           UserSprite(SpriteArea *area, const std::string &name);
           virtual ~UserSprite();

           /**
            * Check if two sprite classes refer to the same underlying sprite
            */
		   bool operator==(const UserSprite &other) {return _offset == other._offset && _area == other._area;}
           /**
            * Check if two sprite classes DO NOT refer to the same underlying sprite
            */
		   bool operator!=(const UserSprite &other) {return _offset != other._offset || _area != other._area;}

		   /**
		    * Check this sprite object is valid.
		    */
           bool is_valid() const {return _area != 0 && _offset != 0;}
		   bool rename(const std::string &name);

		   virtual void plot_raw(const Point &pos, int code = SPA_USE_MASK) const;
		   virtual void plot_scaled(const Point &pos, const ScaleFactors *sf, const TranslationTable *tt = NULL, int code = SPA_USE_MASK) const;
		   virtual void plot_screen(const Point &pos, int code = SPA_USE_MASK) const;

		   virtual std::string name() const;
		   virtual bool info(Size *pixel_size, int *mode  = NULL, bool *mask = NULL) const;

		   bool get_palette(ColourPalette &pal) const;

		   /**
		    * Return SpriteArea this sprite is from
		    */
		   SpriteArea *get_sprite_area()	const {return _area;};

		   /**
		    * Return offset of this sprite in the sprite area
		    */
		   int offset() const			{return _offset;}

		   /**
		    * Return pointer to underlying RISC OS sprite
		    */
		   OsSpritePtr pointer() const	{return (OsSpritePtr)((char *)(_area->pointer()) + _offset);};

		   /**
		    * Returns true if this sprite has a palette
		    */
		   bool has_palette() const {return (*(pointer() + 8) > 0x2C);};

		   virtual void get_wimp_scale(ScaleFactors &factor) const;

		   friend class SpriteArea;

	   protected:
		   SpriteArea *_area;
		   int _offset;
	};

    /**
     * Class to capture screen output to a sprite
     */
    class SpriteCapture
    {
        char *_save_area;
        int _save_regs[4];
        UserSprite *_sprite;
        bool _capturing;

    public:
        SpriteCapture(UserSprite *sprite, bool start_capture = false, bool to_mask = false);
        ~SpriteCapture();

        bool capture();
        bool release();
        bool is_capturing() {return _capturing;}
    };

	/**
	 * Class for handling sprites from the Wimp sprite pool.
	 *
	 * These sprites are read only so just have functions
	 * for getting information and plotting a sprite.
	 *
	 */
	class WimpSprite : public Sprite
	{
	public:
		/**
		 * Construct a wimp sprite.
		 *
		 * There is no check on the name given here.
		 * If it is incorrect the other sprite operations
		 * will fail silently.
		 *
		 * Call the exist function to check if a sprite is
		 * currently in the wimp sprite pool.
		 */
		WimpSprite(const std::string &sname) : _name(sname) {};
		WimpSprite(const char *sname) : _name(sname) {};
		WimpSprite(int file_type);
		WimpSprite(int file_type, std::string leafname);

		virtual ~WimpSprite() {};

		bool exist() const;

	   virtual void plot_raw(const Point &pos, int code = SPA_USE_MASK) const ;
	   virtual void plot_scaled(const Point &pos, const ScaleFactors *sf, const TranslationTable *tt = NULL, int code = SPA_USE_MASK) const;
	   virtual void plot_screen(const Point &pos, int code = SPA_USE_MASK) const;

	   virtual std::string name() const {return _name;}
	   virtual bool info(Size *pixel_size, int *mode  = NULL, bool *mask = NULL) const;

	   virtual void get_wimp_scale(ScaleFactors &factor) const;

	   bool has_palette() const;

	private:
		std::string _name;
	};
};

#endif
