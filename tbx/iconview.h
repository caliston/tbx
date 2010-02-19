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


#ifndef TBX_ICONVIEW_H_
#define TBX_ICONVIEW_H_

#include "fixedcellview.h"
#include "window.h"
#include "mouseclicklistener.h"
#include "draghandler.h"

#include <string>
#include <vector>


namespace tbx {

class IconView;

/**
 * Interface for an items stored in an ItemView.
 *
 * Derive the objects to display from this class
 * and then add them to the IconView.
 *
 * The sprites used must be 68 OS units high
 *
 * If the item is changed in such a way that one
 * of the values returned in this interface changes
 * you must call IconView.update for the item to
 * refresh the display.
 */
class IconViewItem
{
private:
	bool _selected;
	friend class IconView;

public:
	IconViewItem() : _selected(false) {}
	virtual ~IconViewItem() {}

	/**
	 * Return the text to display in the icon view
	 */
	virtual std::string display_text() const = 0;

	/**
	 * Return the name of the sprite to display in the icon view.
	 */
	virtual std::string display_sprite_name() const = 0;

	/**
	 * Return true (the default) to use the client sprite area
	 * to retrieve the sprite to display;
	 */
	virtual bool use_client_sprite_area() const {return true;}

	/**
	 * Called by the item view when an item is double clicked upon.
	 *
	 * Default does nothing.
	 *
	 * @param view IconView displaying the item
	 * @param screen_pos Position in screen coordinates of the click
	 * @param adjust double click with adjust rather than select.
	 */
	virtual void open(IconView *view, const Point &screen_pos, bool adjust) {}

	/**
	 * Returns true if the item is currently selected.
	 *
	 * Use the IconView::select method to select an item.
	 */
	bool selected() const {return _selected;}
};

/**
 * Class to display icons with text underneath them.
 *
 * The items are added as classes derived from IconViewItem.
 *
 * Needs a Window with auto redraw off, auto open off
 * and extensible X and Y attributes.
 *
 * Also needs Button type 10 for selection to work.
 */
class IconView: public FixedCellView, FixedCellRenderer,  MouseClickListener
{
public:
	typedef std::vector<IconViewItem *> Items;

private:
	Items _items;
	tbx::Size _cell_size;
	int _first_selected;
	int _last_selected;
	bool _sorted;
	bool _auto_delete;
	bool _menu_selected;

public:
	IconView(Window window, bool auto_delete = true);
	IconView(std::string window_name, bool auto_delete = true);
	virtual ~IconView();

	// FixedCellRenderer callbacks
	virtual void render_cell(const tbx::FixedCellRenderer::Info &info);
	/**
	 * Callback from FixedCellRenderer to get the size of an individual cell
	 */
	virtual tbx::Size cell_size() const {return _cell_size;}
	virtual bool hit_test(int index, const tbx::Point &pos) const;
	virtual bool intersects(int index, const tbx::BBox &box) const;

	// Mouse click callbacks
	virtual void mouse_click(tbx::MouseClickEvent &event);

	// Make FixedCellView hit_test visible
	int hit_test(const Point &scr_pt) const {return FixedCellView::hit_test(scr_pt);}

	/**
	 * Return true if icon view is stored
	 */
	bool sorted() const {return _sorted;}

	void sorted(bool s);

	void add(IconViewItem *item);
	void insert(int index, IconViewItem *item);
	void insert(const Point &screen_pos, IconViewItem *item);
	void remove(int index);
	void erase(int index);
	void remove(IconViewItem *item);
	void erase(IconViewItem *item);
	void remove_all();
	void erase_all();
	void changed(IconViewItem *item);

	int find(IconViewItem *item) const;
	int find_text(const std::string &text) const;


	// Selection
	bool selected(int index) const;
	void select(int index, bool sel, bool update = true);
	void select(int from, int to, bool sel, bool update = true);
	void toggle_select(int index, bool update = true);
	void toggle_select(int from, int to, bool update = true);
	bool menu_selected() const;

	/**
	 * Returns true if any items are selected
	 */
	bool any_selected() const {return (_last_selected != -1);}
	/**
	 * Return true if no items are selected
	 */
	bool none_selected() const {return (_last_selected == -1);}
	/**
	 * Returns true if one and only one item is selected
	 */
	bool one_selected() const {return (_first_selected == _last_selected);}
	/**
	 * Return true if more than one is selected
	 */
	bool multiple_selected() const {return (_last_selected > _first_selected);}
	int count_selected() const;

	/**
	 * Returns first item selected.
	 * Undefined if no items are selected
	 */
	int first_selected() const {return _first_selected;}
	/**
	 * Returns last item selected.
	 * Undefined if no items are selected.
	 */
	int last_selected() const {return _last_selected;}

	void clear_selection(bool update = true);

	virtual void inserted(int where, int how_many);
	virtual void removed(int where, int how_many);
	void changed(int where, int how_many) {FixedCellView::changed(where, how_many);}

	IconViewItem *operator[](int i) {return _items[i];}
	IconViewItem *operator[](int i) const {return _items[i];}


	typedef Items::iterator iterator;
	typedef Items::const_iterator const_iterator;

	/**
	 * Return iterator to first IconViewItem in window
	 */
	iterator begin() {return _items.begin();}

	/**
	 * Return iterator to end of IconViewItem list
	 */
	iterator end() {return _items.end();}

	/**
	 * Return const iterator to first IconViewItem in window
	 */
	const_iterator begin() const {return _items.begin();}

	/**
	 * Return const iterator to end of IconViewItem list
	 */
	const_iterator end() const {return _items.end();}

	/**
	 * Return iterator to first selected item.
	 * returns end() if none selected.
	 * Note: Not all items between begin_selected and end_selected are
	 * necessarily selected.
	 */
	iterator begin_selected() {return (_last_selected == -1) ? _items.end() : _items.begin() + _first_selected;}

	/**
	 * Return iterator to item after last selected item.
	 */
	iterator end_selected() {return  (_last_selected == -1) ? _items.end() : _items.begin() + _last_selected + 1;}

	/**
	 * Return const iterator to first selected item.
	 * returns end() if none selected.
	 * Note: Not all items between begin_selected and end_selected are
	 * necessarily selected.
	 */
	const_iterator begin_selected() const {return (_last_selected == -1) ? _items.end() : _items.begin() + _first_selected;}
	/**
	 * Return const iterator to item after last selected item.
	 */
	const_iterator end_selected() const {return  (_last_selected == -1) ? _items.end() : _items.begin() + _last_selected + 1;}

private:
	void init();
	// Helpers
	void get_sprite_box(int index, tbx::BBox &sbox) const;
	void get_text_box(int index, tbx::BBox &tbox) const;

	/**
	 * Class to handle selecting items after a rubber box drag
	 */
	class Selector : public tbx::DragHandler, public tbx::FixedCellCallback
	{
		IconView *_me;
		bool _adjust;
	public:
		Selector(IconView *me, bool adjust) : _me(me), _adjust(adjust) {};

		virtual void drag_finished(const tbx::BBox &final);
		virtual void drag_cancelled();

		virtual void fixed_cell_callback(const std::pair<int, int> &range);
	};
};

}

#endif /* ICONVIEW_H_ */
