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


#include "fixedcellview.h"

namespace tbx {

/**
 * Construct a fixed cell view for a window/renderer.
 *
 * Adds listeners it needs to the windows
 */
FixedCellView::FixedCellView(Window window, FixedCellRenderer *renderer) :
	_window(window), _renderer(renderer),
	_size(0),
	_cols_per_row(1)
{
	_window.add_redraw_listener(this);
	_window.add_open_window_listener(this);
}

/**
 * Construct a fixed cell view from a window template
 * and renderer.
 *
 * Adds listeners it needs to the windows
 */
FixedCellView::FixedCellView(std::string window_name, FixedCellRenderer *renderer) :
	_window(window_name), _renderer(renderer),
	_size(0),
	_cols_per_row(1)
{
	_window.add_redraw_listener(this);
	_window.add_open_window_listener(this);
}

/**
 * Calculate initial layout.
 *
 * Call if the window has been shown before this class was created.
 */
void FixedCellView::calc_layout()
{
	WindowState state;
	_window.get_state(state);
	recalc_layout(state.visible_area().bounds(), false);
}

/**
 * Destructor of fixed cell view
 *
 * removes any window listeners it requires
 */
FixedCellView::~FixedCellView()
{
	_window.remove_redraw_listener(this);
	_window.remove_open_window_listener(this);
}

/**
 * Recalculate/refresh the fixed cell view.
 *
 * This is automatically called if the margins are
 * changed.
 *
 * It should also be called if the cell renderer output
 * is modified.
 */

void FixedCellView::refresh()
{
	WindowState state;
	_window.get_state(state);

	if (state.open()) recalc_layout(state.visible_area().bounds(), true);
}

/**
 * Call after inserting items into the collection
 * the FixedCellView is showing.
 *
 */
void FixedCellView::inserted(int where, int how_many)
{
	_size += how_many;
	Size cell_size = _renderer->cell_size();
	int first_row = where / _cols_per_row;
	int last_row = _size / _cols_per_row + 1;
	BBox dirty(_margin.left,
		-last_row * cell_size.height - _margin.top,
		_cols_per_row * cell_size.width + _margin.left,
		-first_row * cell_size.height - _margin.top);
	_window.force_redraw(dirty);
}

/**
 * Call after removing items from the collection
 * the FixedCellView is showing.
 */
void FixedCellView::removed(int where, int how_many)
{
	Size cell_size = _renderer->cell_size();
	int first_row = where / _cols_per_row;
	int last_row = _size / _cols_per_row + 1;
	BBox dirty(_margin.left,
		-last_row * cell_size.height - _margin.top,
		_cols_per_row * cell_size.width + _margin.left,
		-first_row * cell_size.height - _margin.top);
	_size -= how_many;
	_window.force_redraw(dirty);
}

/**
 * Call after changing items in the collection
 * the FixedCellView is showing.
 */
void FixedCellView::changed(int where, int how_many)
{
	Size cell_size = _renderer->cell_size();
	int first_row = where / _cols_per_row;
	int last_row = _size / _cols_per_row + 1;
	BBox dirty(_margin.left,
		-last_row * cell_size.height - _margin.top,
		_cols_per_row * cell_size.width + _margin.left,
		-first_row * cell_size.height - _margin.top);
	_window.force_redraw(dirty);
}

/**
 * Window has been opened or resized, so re do layout
 */
void FixedCellView::open_window(OpenWindowEvent &event)
{
	recalc_layout(event.visible_area(), false);
}

/**
 * Recalculate the cell layout and update/refresh the window
 *
 * @param visible_area visible area of window on screen
 * @param repaint true to always refresh the window and its extent
 *                false refresh only if cols_per_row changes
 */
void FixedCellView::recalc_layout(const BBox &visible_area, bool repaint)
{
	Size cell_size = _renderer->cell_size();

	int cols_per_row = (visible_area.width() - _margin.left - _margin.right) / cell_size.width;
	if (cols_per_row < 1) cols_per_row = 1;

	if (cols_per_row != _cols_per_row)
	{
		// Need to re do the layout
		_cols_per_row = cols_per_row;
		repaint = true;
	}
	if (repaint)
	{
		int rows = (_size + cols_per_row - 1)/cols_per_row;
		int height = rows *cell_size.height + _margin.top + _margin.bottom;
		if (height < visible_area.height())
			height = visible_area.height();
		BBox extent(0,-height, visible_area.width(), 0);
		_window.extent(extent);
		_window.force_redraw(extent);
	}
}

/**
 * Redraw the window
 */
void FixedCellView::redraw(const RedrawEvent &event)
{
	BBox work_clip = event.visible_area().work(event.clip());
	Size cell_size = _renderer->cell_size();

	int first_row = (-work_clip.max.y + _margin.top) / cell_size.height;
	int last_row =  (-work_clip.min.y + _margin.top) / cell_size.height;

	if (first_row >= _size) return; // Nothing to draw
	if (last_row >= _size) last_row = _size - 1;

    int first_col = (work_clip.min.x - _margin.left) / cell_size.width;
    int last_col = (work_clip.max.x - _margin.left) / cell_size.width;

    if (first_col >= _size) return; // Nothing to redraw
    if (first_col >= _cols_per_row) return; // Nothing to redraw

    if (last_col >= _size) last_col = _size - 1;
    if (last_col >= _cols_per_row) last_col = _cols_per_row - 1;

    FixedCellRenderer::Info cell_info(event);

	cell_info.bounds.max.y = -first_row * cell_size.height - _margin.top;
	cell_info.screen.y = event.visible_area().screen_y(cell_info.bounds.max.y);
	int first_col_x = first_col * cell_size.width + _margin.left;
	int first_col_scr_x = event.visible_area().screen_x(first_col_x);

    for (int row = first_row; row <= last_row; row++)
    {
        cell_info.bounds.min.y = cell_info.bounds.max.y - cell_size.height;
        cell_info.bounds.min.x = first_col_x;
        cell_info.screen.y -= cell_size.height;
        cell_info.screen.x = first_col_scr_x;
        cell_info.index = row * _cols_per_row + first_col;

        for (int col = first_col; col <= last_col; col++)
        {
           if (cell_info.index < _size)
           {
              cell_info.bounds.max.x = cell_info.bounds.min.x + cell_size.width;

              _renderer->render_cell(cell_info);
              cell_info.bounds.min.x = cell_info.bounds.max.x;
              cell_info.screen.x += cell_size.width;
              cell_info.index++;
           }
        }
        cell_info.bounds.max.y = cell_info.bounds.min.y;
    }
}

/**
 * Find the index to insert an item based on the screen
 * coordinates given
 *
 * @param scr_pt position to find the index for
 * @return index for insert.
 */
int FixedCellView::insert_index(const Point &scr_pt) const
{
	WindowState state;
	_window.get_state(state);

	Point work = state.visible_area().work(scr_pt);
	Size cell_size = _renderer->cell_size();

	work.x -= _margin.left;
	work.y += _margin.top;

	int col = (work.x + cell_size.width/2) / cell_size.width;
	int row = (-work.y + cell_size.height/2) / cell_size.height;

	int index;
	if (row < 0) index = 0;
	else
	{
		if (col < 0) col = 0;
		else if (col >= _cols_per_row)
		{
			col = 0;
			row++;
		}
		index = row * _cols_per_row + col;
		if (index > _size) index = _size;
	}

	return index;
}

/**
 * Get item index for the screen location (if any)
 *
 * @param scr_pt position on screen (screen coordinates)
 * @return index of item under location or -1 if no item at location.
 */
int FixedCellView::screen_index(const Point &scr_pt) const
{
	WindowState state;
	_window.get_state(state);

	Point work = state.visible_area().work(scr_pt);
	Size cell_size = _renderer->cell_size();

	work.x -= _margin.left;
	work.y += _margin.top;

	int col = work.x / cell_size.width;
	int row = -work.y / cell_size.height;

	int index = -1;
	if (col >= 0 && col < _cols_per_row
		&& row >= 0 && row < row_count())
	{
	   index = row * _cols_per_row + col;
	   if (index >= _size) index = -1;
	}

	return index;
}

/**
 * Get the row and column to a screen location.
 *
 * This can be outside of the data area.
 *
 * @param scr_pt position on screen (screen coordinates)
 * @param column update with the column for the screen position or
 *        -1 if in left margin
 *        >= columns_per_row() if in right margin
 * @param row update with the row for the screen position or
 *        -1 if in top margin.
 *        >= row_count() if in bottom margin.
 * @returns index of item at the location or -1 if column and row are not on
 *          an item.
 */
int FixedCellView::screen_location(const Point &scr_pt, int &column, int &row) const
{
	WindowState state;
	_window.get_state(state);

	Point work = state.visible_area().work(scr_pt);
	Size cell_size = _renderer->cell_size();

	work.x -= _margin.left;
	work.y += _margin.top;

	column = work.x / cell_size.width;
	row = -work.y / cell_size.height;

	int index = -1;
	if (column >= 0 && column < _cols_per_row
		&& row >= 0 && row < row_count())
	{
	   index = row * _cols_per_row + column;
	   if (index >= _size) index = -1;
	}

	return index;
}

/**
 * Hit test the given screen point.
 *
 * Unlike screen_index this uses the cell renderer to
 * see if it has actually hit content rather than just
 * somewhere in the cell.
 *
 * returns: index of hit or -1 if none.
 */
int FixedCellView::hit_test(const Point &scr_pt) const
{
	WindowState state;
	_window.get_state(state);

	Point work = state.visible_area().work(scr_pt);
	Size cell_size = _renderer->cell_size();

	work.x -= _margin.left;
	work.y += _margin.top;

	int column = work.x / cell_size.width;
	int row = -work.y / cell_size.height;

	int index = -1;
	if (column >= 0 && column < _cols_per_row
		&& row >= 0 && row < row_count())
	{
	   index = row * _cols_per_row + column;
	   if (index >= _size) index = -1;
	   else
	   {
		   work.x -= column * cell_size.width;
		   work.y += (row + 1) * cell_size.height;
		   if (!_renderer->hit_test(index, work)) index = -1;
	   }
	}

	return index;
}

/**
 * Get the cell bounds for the specified index.
 *
 * @param index index to retrieve the bounds for
 * @param bounds bounding box to update (work area coordinates)
 *
 */
void FixedCellView::get_cell_bounds(int index, BBox &bounds)
{
	get_cell_bounds(index % _cols_per_row, index / _cols_per_row, bounds);
}

/**
 * Get the cell bounds for the specified column and row
 *
 * It is permitted to specifiy a column and row outside of the
 * used cells.
 *
 * @param column column to retrieve the bounds for
 * @param row row to retrieve the bounds for
 * @param bounds bounding box to update (work area coordinates)
 */
void FixedCellView::get_cell_bounds(int column, int row, BBox &bounds)
{
	Size cell_size = _renderer->cell_size();
	bounds.min.x = _margin.left + column * cell_size.width;
	bounds.max.x = bounds.min.x + cell_size.width;
	bounds.max.y = -_margin.top - row * cell_size.height;
	bounds.min.y = bounds.max.y - cell_size.height;
}

/**
 * Use UpdateWindow to update cell for index
 */
void FixedCellView::update_cell(int index)
{
	BBox bounds;
	get_cell_bounds(index, bounds);
	_window.update(bounds, 0);
}

/*
 * Use UpdateWindow to update all cells between and including
 * the current icon
 */
void FixedCellView::update_cells(int first_index, int last_index)
{
	if (first_index >= _size) return;
	if (last_index >= _size) last_index = _size - 1;

	BBox bounds;
	if (first_index == last_index)
	{
		get_cell_bounds(first_index, bounds);
	} else
	{
		int first_row = first_index / _cols_per_row;
		int last_row = last_index / _cols_per_row;

		if (first_row == last_row)
		{
			get_cell_bounds(first_index, bounds);
			bounds.max.x += (last_index - first_index) * _renderer->cell_size().width;
		} else
		{
			bounds.min.x = _margin.left;
			bounds.max.y = _margin.right;
			int cell_height = _renderer->cell_size().height;
			bounds.min.y = -_margin.top - (cell_height * (last_row + 1));
			bounds.max.y = -_margin.top - (cell_height * first_row);
		}
	}
	_window.update(bounds, 0);
}

/**
 * Call the given callback for each item that has content in the given box.
 *
 * @param box box for callback in screen coordinates
 * @param callback object with callback interface
 * @param update call update window for the smallest rectangle covering the items called.
 */
void FixedCellView::for_each_in_box(const BBox &box, FixedCellCallback *callback, bool update)
{
    WindowState state;
    _window.get_state(state);
    BBox work_box = state.visible_area().work(box);
    Size cell_size = _renderer->cell_size();

    work_box.normalise();

    work_box.move(-_margin.left, _margin.top);

    int first_col = work_box.min.x / cell_size.width;
    int last_col = work_box.max.x / cell_size.width;

    if (first_col < 0) first_col = 0;
    if (last_col >= _cols_per_row) last_col = _cols_per_row - 1;

    if (first_col > last_col) return; // nothing to do

    int first_row = -work_box.max.y / cell_size.height;
    int last_row = -work_box.min.y / cell_size.height;
    if (first_row < 0) first_row = 0;
    if (last_row > (_size-1)/_cols_per_row) last_row = (_size-1)/_cols_per_row;

    if (first_row > last_row) return; // nothing to do

    std::pair<int, int> range;

    int index = first_row * _cols_per_row + first_col;
    if (index >= _size) return; // Nothing to do

    int end_of_row_skip = _cols_per_row - (last_col - first_col+1);

    BBox row_relative(work_box);
    row_relative.move(-cell_size.width * first_col, cell_size.height * (first_row+ 1));
    BBox col_relative(row_relative);

    int col;
    int row = first_row;

    int first_used_col = last_col +1;
    int last_used_col = first_col -1;
    int first_used_row = last_row+1;
    int last_used_row = first_row-1;

    range.first = -1;

    // Check top line
    for (col = first_col; col <= last_col && index < _size; col++)
    {
    	if (_renderer->intersects(index, col_relative))
    	{
    		if (col < first_used_col) first_used_col = col;
    		if (col > last_used_col) last_used_col = col;
    		if (range.first == -1)
    		{
    			range.first = range.second = index;
    		} else
    		{
    			if (index == range.second + 1)
    			{
    				range.second++;
    			} else
    			{
    				callback->fixed_cell_callback(range);
    				range.first = range.second = index;
    			}
    		}
    	}
    	index++;
    	col_relative.move_left(cell_size.width);
    }

    if (range.first != -1)
    {
    	first_used_row = row;
    	last_used_row = row;
    }

    row++;
    row_relative.move_up(cell_size.height);
    if (row < last_row)
    {
    	int c1, c2, i1, i2;
    	while (row < last_row)
    	{
			// Only need to check start and end of column in middle
			index += end_of_row_skip;
			if (_renderer->intersects(index, row_relative))
			{
				c1 = first_col;
				i1 = index;
			} else
			{
				c1 = first_col + 1;
				i1 = index + 1;
			}

			if (first_col == last_col)
			{
				c2 = first_col;
				i2 = index;
			} else
			{
				col_relative = row_relative;
				int dist = last_col - first_col;
				index += dist;
				col_relative.move_left(dist * cell_size.width);
				if (_renderer->intersects(index, col_relative))
				{
					c2 = last_col;
					i2 = index;
				} else
				{
					c2 = last_col - 1;
					i2 = index - 1;
				}
			}

			if (c1 <= c2)
			{
				if (row < first_used_row) first_used_row = row;
				last_used_row = row;
        		if (c1 < first_used_col) first_used_col = c1;
        		if (c2 > last_used_col) last_used_col = c2;
				if (range.first == -1)
				{
					range.first = i1;
					range.second = i2;
				} else if (i1 == range.second+1)
				{
					range.second = i2;
				} else
				{
					callback->fixed_cell_callback(range);
					range.first = i1;
					range.second = i2;
				}
			}
			index++;
			row++;
			row_relative.move_up(cell_size.height);
    	}
    }

    // Bottom row
    if (row == last_row)
    {
        index += end_of_row_skip;
		col_relative = row_relative;
        for (col = first_col; col <= last_col && index < _size; col++)
        {
        	if (_renderer->intersects(index, col_relative))
        	{
				if (row < first_used_row) first_used_row = row;
        		if (col < first_used_col) first_used_col = col;
        		if (col > last_used_col) last_used_col = col;
        		last_used_row = row;
        		if (range.first == -1)
        		{
        			range.first = range.second = index;;
        		} else
        		{
        			if (index == range.second + 1)
        			{
        				range.second++;
        			} else
        			{
        				callback->fixed_cell_callback(range);
        				range.first = range.second = index;
        			}
        		}
        	}
        	index++;
        	col_relative.move_left(cell_size.width);
        }
    }

    if (range.first != -1)
    {
		callback->fixed_cell_callback(range);
    }

    if (update)
    {
    	if (first_used_col == last_used_col
    		&& first_used_row == last_used_row)
    	{
    		BBox bounds;
    		get_cell_bounds(first_used_col, first_used_row, bounds);
    		_window.update(bounds, 0);
    	} else if (first_used_col <= last_used_col
    			&& first_used_row <= last_used_row)
    	{
    		BBox bounds, b2;
    		get_cell_bounds(first_used_col, first_used_row, bounds);
    		get_cell_bounds(last_used_col, last_used_row, b2);
    		bounds.cover(b2);
    		_window.update(bounds, 0);
    	}
    }
}

}
