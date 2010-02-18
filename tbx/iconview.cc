/*
 * tbx RISC OS toolbox library
 *
 * Copyright (C) 2008-2010 Alan Buckley   All Rights Reserved.
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

#include "iconview.h"
#include "iconpainter.h"
#include "font.h"
#include "sprite.h"
#include "application.h"

#include "string.h"
#include <algorithm>

//#define CHECK_SIZES
#ifdef CHECK_SIZES
#include "graphics.h"
#endif

namespace tbx
{

/**
 * Compares two IconViewItem pointers and returns true if the
 * first is less then the second according to a case insensitive
 * comparison of the display names.
 *
 * This is used for normal sorting and display
 */
bool icon_view_compare_text_nocase(IconViewItem *i1, IconViewItem *i2)
{
	return (stricmp(i1->display_text().c_str(), i2->display_text().c_str()) < 0);
}

/**
 * Simple class used for find algorthims
 */
class FindIconViewItem : public IconViewItem
{
	std::string _value;
public:
	FindIconViewItem(std::string v) : _value(v) {}
	virtual std::string display_text() const {return _value;}
	virtual std::string display_sprite_name() const {return _value;}
};

const int IV_MARGIN = 8; // Margin on each side of cell
const int IV_GAP = 8;    // Gap between sprite and text

/**
 * Construct an unsorted icon view using the given window.
 *
 *
 * @param window The Window to use for the iconview
 * @param auto_delete true to delete all the items when the view is deleted (defaults to true).
 */
IconView::IconView(Window window, bool auto_delete /*=true*/) : FixedCellView(window, this),
	_auto_delete(auto_delete)
{
	init();
}

/**
 * Construct an unsorted icon view using the given window.
 *
 *
 * @param window_name The name of the window template to use for the iconview
 * @param auto_delete true to delete all the items when the view is deleted (defaults to true).
 */
IconView::IconView(std::string window_name, bool auto_delete /*= true*/) : FixedCellView(window_name, this),
_auto_delete(auto_delete)
{
	init();
}

// Common initialisation
void IconView::init()
{
	_cell_size.width = 160 + IV_MARGIN * 2;
	_cell_size.height = 68 /* sprite */
	                   + 32 /* text */
	                   + IV_GAP
	                   + IV_MARGIN * 2;

	calc_layout();

	window().add_mouse_click_listener(this);

	_first_selected = 0x7FFFFFFF;
	_last_selected = -1;

	_sorted = false;
}


/**
 * Destructor removes listeners from window.
 */
IconView::~IconView()
{
	window().remove_mouse_click_listener(this);
	if (_auto_delete)
	{
		for (Items::iterator i = _items.begin(); i != _items.end(); ++i)
		{
			delete (*i);
		}
	}
}

/**
 * Callback from FixedCellRenderer to render an individual cell
 */
void IconView::render_cell(const tbx::FixedCellRenderer::Info &info)
{
	IconViewItem* &item = _items[info.index];

	BBox content(info.bounds);
	content.inflate(-IV_MARGIN); // Reduce size for border

	if (item->use_client_sprite_area())
	{
		// Can't do sprite and text icon for user client area so
		// do two paints.
		IconPainter ip;
		ip.inverted(item->selected());
		ip.bounds() = content;
		ip.bounds().max.y = content.min.y + 32;
		ip.hcentred(true);

		// Paint text at bottom
		ip.text(item->display_text());
		ip.redraw(info.redraw);

		// Paint sprite at top
		ip.text_icon(false);
		ip.sprite(item->display_sprite_name());
		ip.use_client_sprite_area();
		ip.bounds().max.y = content.max.y;
		ip.bounds().min.y = content.max.y - 68;
		ip.redraw(info.redraw);
	} else
	{
		// WIMP sprite can be done with one Icon
		IconPainter ip;
		ip.text(item->display_text()).sprite(item->display_sprite_name());
		ip.text_and_sprite_centred();
		ip.inverted(item->selected());
		ip.bounds() = content;
		ip.redraw(info.redraw);
	}

#ifdef CHECK_SIZES
	Graphics g;

	BBox sbounds;
	info.redraw.visible_area().screen(content, sbounds);

	g.set_wimp_colour(WimpColour::grey2);
	g.rect(sbounds);

	BBox sbox;
	get_sprite_box(info.index, sbox);
	sbox.move(info.screen.x, info.screen.y);

	g.set_wimp_colour(WimpColour::red);
	g.rect(sbox);

	BBox tbox;
	get_text_box(info.index, tbox);
	tbox.move(info.screen.x, info.screen.y);

	g.set_wimp_colour(WimpColour::dark_green);
	g.rect(tbox);
#endif

}

/**
 * Callback from FixedCellRenderer to see if a screen point hits
 * the content of an item. i.e. on the sprite or or text.
 */
bool IconView::hit_test(int index, const tbx::Point &pos) const
{
	// Quick test
	if (pos.x < IV_MARGIN || pos.y < IV_MARGIN
		|| pos.x > _cell_size.width - IV_MARGIN
		|| pos.y > _cell_size.height - IV_MARGIN)
		return false;

	// Check sprite box
	BBox check;
	get_sprite_box(index, check);

	if (check.contains(pos)) return true;

	// Finally check text
	get_text_box(index, check);
	return check.contains(pos);
}

/**
 * Callback from FixedCellRenderer to see if a box intersects the
 * content of an item.
 */
bool IconView::intersects(int index, const tbx::BBox &box) const
{
	// Quick test intersects if mid point is in box
	if (box.contains(_cell_size.width/2, _cell_size.height/2)) return true;

	// Check sprite
	BBox check;
	get_sprite_box(index, check);
	if (check.intersects(box)) return true;

	// Check text
	get_text_box(index, check);
	return check.intersects(box);
}

/**
 * Gets sprite box relative to bottom left corner
 */
void IconView::get_sprite_box(int index, BBox &sbox) const
{
	Size sprite_size;

	if (_items[index]->use_client_sprite_area())
	{
		UserSprite us(app()->sprite_area(), _items[index]->display_sprite_name());
		sprite_size = us.size();
	} else
	{
		WimpSprite ws(_items[index]->display_sprite_name());
		sprite_size = ws.size();
	}

	sbox.min.x = (_cell_size.width - sprite_size.width) / 2;
	sbox.max.x = sbox.min.x + sprite_size.width;
	sbox.max.y = _cell_size.height - IV_MARGIN;
	sbox.min.y = sbox.max.y - sprite_size.height;
}

/**
 * Get text box relative to bottom left corner
 */
void IconView::get_text_box(int index, BBox &tbox) const
{
	WimpFont wf;
	Size text_size;
	text_size.width = wf.string_width_os(_items[index]->display_text());
	text_size.height = wf.height_os();

	tbox.min.x = (_cell_size.width - text_size.width) / 2;
	tbox.max.x = tbox.min.x + text_size.width;
	tbox.min.y = IV_MARGIN;
	tbox.max.y = tbox.min.y + text_size.height;
}


/**
 * Mouse has been clicked so automatically do selection, call
 * IconViewItem open etc.
 */
void IconView::mouse_click(tbx::MouseClickEvent &event)
{
	int index = hit_test(event.point());
	if (index != -1)
	{
		if (event.is_select())
		{
			if (!selected(index))
			{
				clear_selection();
				select(index, true);
			}
		} else if (event.is_adjust())
		{
			select(index, !selected(index));
		} else if (event.is_select())
		{
			// Double click opens item
			_items[index]->open(this, event.point(), false);
		} else if (event.is_adjust())
		{
			// Double click opens item
			_items[index]->open(this, event.point(), true);
		}
	} else
	{
		if (event.is_select_drag())
		{
			clear_selection();
			window().drag_rubber_box_local(event.point(), new Selector(this, false) );
		} else if (event.is_adjust_drag())
		{
			window().drag_rubber_box_local(event.point(), new Selector(this, true));
		} else if (event.is_select())
		{
			clear_selection();
		}
	}
}

/**
 * Drag has been finished so select items in final box
 * and self destruct
 */
void IconView::Selector::drag_finished(const BBox &final)
{
	if (!_adjust) _me->clear_selection();
	_me->for_each_in_box(final, this, true);
	delete this;
}

/**
 * Drag has been cancelled so self destruct
 */

void IconView::Selector::drag_cancelled()
{
	delete this;
}

/**
 * Call back for each cell in selection box
 */
void IconView::Selector::fixed_cell_callback(const std::pair<int, int> &range)
{
	if (_adjust && _me->_last_selected != -1)
	{
		_me->toggle_select(range.first, range.second, false);
	} else
	{
		// Relies on clear selection having been called before this
		_me->select(range.first, range.second, true, false);
	}
}

/**
 * Set if IconView is sorted or not.
 *
 * View is sorted by case insensitive icon name.
 *
 * Turning sort on - sorts and redisplays any existing data.
 */
void IconView::sorted(bool s)
{
	if (s != _sorted)
	{
		_sorted = s;
		if (s && _items.size())
		{
			std::sort(_items.begin(), _items.end(), icon_view_compare_text_nocase);
			WindowState state;
			window().get_state(state);
			window().force_redraw(state.visible_area().bounds());
		}
	}
}

/**
 * Add an item to the icon view. If it is sorted
 * it will be put in the sorted position, otherwise
 * it will be added to the end.
 */
void IconView::add(IconViewItem *item)
{
	if (_sorted)
	{
		Items::iterator i = std::lower_bound(_items.begin(), _items.end(), item, icon_view_compare_text_nocase);
		_items.insert(i, item);
		inserted(std::distance(i, _items.begin()),1);
	} else
	{
		_items.push_back(item);
		inserted(_items.size() - 1,1);
	}
}

/**
 * Insert the item at the given position.
 *
 * If the list is sorted the index parameter is ignored.
 * @param index for insert.
 * @param The item to insert.
 */
void IconView::insert(int index, IconViewItem *item)
{
	if (_sorted) add(item);
	else
	{
		_items.insert(_items.begin() + index, item);
		inserted(index,1);
	}
}

/**
 * Insert the item at the given screen coordinates.
 *
 * The item is moved to the nearest cell.
 *
 * The position is ignored if the icon view is sorted.
 *
 * @param screen_pos point in screen coordinates for the insert.
 * @param item item to insert.
 */
void IconView::insert(const Point &screen_pos, IconViewItem *item)
{
	insert(insert_index(screen_pos), item);
}

/**
 * Remove item at the given index.
 *
 * Does not delete the item.
 */
void IconView::remove(int index)
{
	_items.erase(_items.begin() + index);
	removed(index, 1);
}

/**
 * Erase the item at the given index.
 *
 * Removes from screen and deletes the item
 */
void IconView::erase(int index)
{
	delete _items[index];
	_items.erase(_items.begin() + index);
	removed(index, 1);
}

/**
 * Removed the specified icon from the list.
 *
 * Does not delete it
 */
void IconView::remove(IconViewItem *item)
{
	int index = find(item);
	if (index >= 0) remove(index);
}

/**
 * Remove the specified item from the list and delete it
 */
void IconView::erase(IconViewItem *item)
{
	int index = find(item);
	if (index >= 0) erase(index);
}

/**
 * Inform icon view the display characteristics of the
 * item have been changed.
 *
 * Call this function or the equivalent with the
 * index of the item after changing the item in such
 * a way that the display_text or sprite changes.
 */
void IconView::changed(IconViewItem *item)
{
	int index = find(item);
	if (index >= 0) changed(index, 1);
}

/**
 * Find the location of an icon view item.
 *
 * @returns found location or -1 if not found.
 */
int IconView::find(IconViewItem *item) const
{
	Items::const_iterator i;
	if (_sorted)
	{
		i = std::lower_bound(_items.begin(), _items.end(), item, icon_view_compare_text_nocase);
		if (i == _items.end() || (*i) != item) return -1;
	} else
	{
		i = std::find(_items.begin(), _items.end(), item);
		if (i == _items.end()) return -1;
	}

	return std::distance(_items.begin(), i);
}

/**
 * Find the given text in the icon view item.
 *
 *
 */
int IconView::find_text(const std::string &text) const
{
	FindIconViewItem fiv(text);
	return find(&fiv);
}

/**
 * Return true if item at index is selected
 */
bool IconView::selected(int index) const
{
	return _items[index]->selected();
}

/**
 * Select the item at the given index
 *
 * @param index to select
 * @param sel true to select, false to deselect
 * @param update update the iconview if selection changed
 */
void IconView::select(int index, bool sel, bool update /*= true*/)
{
	if (_items[index]->_selected != sel)
	{
		_items[index]->_selected = sel;
		if (sel)
		{
			if (index < _first_selected) _first_selected = index;
			if (index > _last_selected) _last_selected = index;
		} else
		{
			if (index == _first_selected)
			{
				if (_first_selected == _last_selected)
				{
					_first_selected = 0x7FFFFFFF;
					_last_selected = -1;
				} else
				{
					_first_selected++;
					while (!_items[_first_selected]->_selected) _first_selected++;
				}
			} else if (index == _last_selected)
			{
				_last_selected--;
				while (!_items[_last_selected]->_selected) _last_selected--;
			}
		}
		if (update) update_cell(index);
	}
}

/**
 * Selected/deselect all items in a range
 *
 * @param from index of first item
 * @param to index of last item
 * @param sel true to select, false to deselect
 * @param update true to update view afterwards
 */
void IconView::select(int from, int to, bool sel, bool update /* = true */)
{
	if (sel)
	{
		if (from < _first_selected) _first_selected = from;
		if (to > _last_selected) _last_selected = to;
		for (int i = from; i <= to; i++) _items[i]->_selected = true;
	} else if (to >= _first_selected && from <= _last_selected)
	{
		if (from <= _first_selected)
		{
			if (to >= _last_selected)
			{
				for (int i = _first_selected; i <= _last_selected; i++) _items[i]->_selected = false;
				_first_selected = 0x7FFFFFFF;
				_last_selected = -1;
			} else
			{
				for (int i = _first_selected; i <= to; i++) _items[i]->_selected = false;
				_first_selected = to + 1;
				while (!_items[_first_selected]->_selected) _first_selected++;
			}
		} else
		{
			if (to < _last_selected)
			{
				for (int i = from; i <= to; i++) _items[i]->_selected = false;
			} else
			{
				for (int i = from; i <= _last_selected; i++) _items[i]->_selected = false;
				_last_selected = from-1;
				while (!_items[_last_selected]->_selected) _last_selected--;
			}
		}
	} else
	{
		update = false; // Nothing to update
	}
}

/**
 * Toggle selection at index
 */
void IconView::toggle_select(int index, bool update /* = true */)
{
	select(index, !_items[index]->_selected, update);
}

/**
 * Toggle selection in a range
 */
void IconView::toggle_select(int from, int to, bool update  /* = true */)
{
	if (from > _last_selected || to < _first_selected) select(from, to, true, update);
	else
	{
		int min_toggle = from;
		int max_toggle = to;
		if (from < _first_selected)
		{
			min_toggle = _first_selected;
			_first_selected = from;
		}
		if (max_toggle > _last_selected)
		{
			max_toggle = _last_selected;
			_last_selected = to;
		}
		int i;

		for (i = from; i < min_toggle; i++) _items[i]->_selected = true;
		for (i = min_toggle; i <= max_toggle; i++) _items[i]->_selected = !_items[i]->_selected;
		for (i = max_toggle+1; i <= to; i++) _items[i]->_selected = true;

		// Check ends
		while (_first_selected <= _last_selected && !_items[_first_selected]->_selected) _first_selected++;
		if (_first_selected > _last_selected)
		{
			_first_selected = 0x7FFFFFFF;
			_last_selected = -1;
		} else
		{
			while (!_items[_last_selected]->_selected) _last_selected--;
		}
	}

	if (update) update_cells(from, to);
}

/**
 * Clear the selection
 */
void IconView::clear_selection(bool update /* = true*/)
{
	if (_first_selected > _last_selected) return;

	for (int i = _first_selected; i <= _last_selected; i++)
	{
		_items[i]->_selected = false;
	}

	int fs = _first_selected;
	int ls = _last_selected;

	_first_selected = 0x7FFFFFFF;
	_last_selected = -1;

	if (update) update_cells(fs, ls);
}

/**
 * Returns a count of the number of items selected
 */
int IconView::count_selected() const
{
	if (_first_selected > _last_selected) return 0;
	int cnt = 1;
	for (int i = _first_selected+1; i <= _last_selected; i++)
	{
		if (_items[i]->_selected) cnt++;
	}

	return cnt;
}

/**
 * Override from base class to keep selection information up to date
 */
void IconView::inserted(int where, int how_many)
{
	if (_last_selected != -1)
	{
		if (where <= _first_selected) _first_selected += how_many;
		if (where <= _last_selected) _last_selected += how_many;
	}
	FixedCellView::inserted(where, how_many);
}

/**
 * Override from base class to keep selection information up to date
 */
void IconView::removed(int where, int how_many)
{
	if (_last_selected != -1 && where <= _last_selected)
	{
		int last_removed = where + how_many - 1;
		if (last_removed < _first_selected)
		{
			_first_selected -= how_many;
			_last_selected -= how_many;
		} else if (where > _first_selected)
		{
			if (last_removed < _last_selected) _last_selected -= how_many;
			else
			{
				_last_selected = where;
				while (!_items[_last_selected]->_selected) _last_selected--;
			}
		} else if (last_removed >= _last_selected)
		{
			_first_selected = 0x7FFFFFFF;
			_last_selected = -1;
		} else
		{
			_last_selected -= how_many;
			_first_selected = where;
			while (!_items[_first_selected]->_selected) _first_selected++;
		}
	}

	FixedCellView::removed(where, how_many);
}

}
