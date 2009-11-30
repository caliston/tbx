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


#ifndef FIXEDCELLVIEW_H_
#define FIXEDCELLVIEW_H_

#include "window.h"
#include "margin.h"
#include "redrawlistener.h"
#include "openwindowlistener.h"
#include "size.h"
#include <utility>

namespace tbx {

/**
 * Base class for the renderer the FixedCellView.
 *
 * Derive from this class and override its render_cell method
 * to display a cell.
 */
class FixedCellRenderer
{
public:
	/**
	 * Information on what needs to be redrawn
	 */
	struct Info
	{
		/**
		 * Redraw event object from FixedCellView
		 */
		const RedrawEvent &redraw;

		/**
		 * Bounding box of cell to redraw in work area coordinates
		 */
		BBox bounds;

		/**
		 * Bottom left of items bounding box in screen coordinates
		 */
		Point screen;

		/**
		 * zero based index of cell to redraw
		 */
		int index;

		/**
		 * Constructed used by FixedCellView
		 */
		Info(const RedrawEvent &r) : redraw(r) {};
	};

	/**
	 * Destructor
	 */
	virtual ~FixedCellRenderer() {}

	/**
	 * Called to render each cell that needs drawing
	 *
	 * The cell is only clipped to the containing Windows visible area,
	 * not to the cell bounds so the content must be drawn entirely
	 * inside the cell bounding box or the clip area should be set
	 * before drawing.
	 *
	 * @param info Information on redraw event and cell to be redrawn
	 */
	virtual void render_cell(const FixedCellRenderer::Info &info) = 0;

	/**
	 * Called to get the size of the cells
	 */
	virtual Size cell_size() const = 0;

	/**
	 * Check if the point given hits content of the cell rather
	 * then a margin or background.
	 *
	 * Default returns true so all the cell is content.
	 *
	 * @param index index of item to check
	 * @param pos location to check relative to bottom left of cell
	 * @return true if content is at that location, otherwise false
	 */
	virtual bool hit_test(int index, const Point &pos) const {return true;}

	/**
	 * Check if the given rectangle intersects the content of the cell
	 * rather than a margin or background.
	 *
	 * Default returns true so all cell is content.
	 *
	 * @param index index of item to  check
	 * @param box box to check if it intersect the cell
	 * @return true if some contents are in the box, otherwise false
	 */
	virtual bool intersects(int index, const BBox &box) const {return true;}
};

/**
 * Call back interface for for_each_in_box function
 */
class FixedCellCallback
{
public:
	virtual ~FixedCellCallback() {};

	/**
	 * Called for every continuous range of cells that matches
	 * the criteria in the for_each_in_box function.
	 */
	virtual void fixed_cell_callback(const std::pair<int,int> &range) = 0;
};

/**
 * Class to display objects with a fixed display size in a window.
 *
 * The actual data for the objects is held separately and is
 * displayed using a given cell renderer.
 *
 * The inserted/changed/deleted functions should be called after
 * any change to the collection being displayed.
 */
class FixedCellView : public tbx::RedrawListener, tbx::OpenWindowListener
{
private:
	tbx::Window _window;
	FixedCellRenderer *_renderer;
	Margin _margin;
	int _size;
	int _cols_per_row;

public:
	FixedCellView(Window window, FixedCellRenderer *renderer);
	FixedCellView(std::string window_name, FixedCellRenderer *renderer);
	virtual ~FixedCellView();

	void calc_layout();

	const Margin &margin() const {return _margin;}
	void margin(const Margin &margin) { _margin = margin; refresh();}

	Window window() const { return _window;}

	// Window events used
	virtual void redraw(const RedrawEvent &event);
	virtual void open_window(OpenWindowEvent &event);

	// Calls to update view
	void refresh();
	virtual void inserted(int where, int how_many);
	virtual void removed(int where, int how_many);
	void changed(int where, int how_many);

	// Information
	/**
	 * Total number of items displayed
	 */
	int size() const {return _size;}

	/**
	 * Number of columns displayed per row.
	 */
	int columns_per_row() const {return _cols_per_row;}
	/**
	 * Number of rows currently displayed
	 */
	int row_count() const {return (_size - 1)/_cols_per_row + 1;}

	int insert_index(const Point &scr_pt) const;
	int screen_index(const Point &scr_pt) const;
	int screen_location(const Point &scr_pt, int &column, int &row) const;
	int hit_test(const Point &scr_pt) const;


	void get_cell_bounds(int index, BBox &bounds);
	void get_cell_bounds(int column, int row, BBox &bounds);

	void update_cell(int index);
	void update_cells(int first_index, int last_index);

	void for_each_in_box(const BBox &box, FixedCellCallback *callback, bool update);

private:
	// Helpers
	void recalc_layout(const BBox &visible_area, bool repaint);

};

}

#endif /* FIXEDCELLVIEW_H_ */
