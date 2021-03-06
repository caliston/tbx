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

/*
 * selectcommands.h
 *
 *  Created on: 28 Apr 2010
 *      Author: alanb
 */

#ifndef TBX_SELECTCOMMANDS_H_
#define TBX_SELECTCOMMANDS_H_

#include "../command.h"
#include "itemview.h"
namespace tbx
{
namespace view
{

/**
 * Command to select all items in an item view
 */
class SelectAllCommand : public Command
{
	ItemView *_view;
public:
	/**
	 * Constructor giving view
	 */
	SelectAllCommand(ItemView *v) : _view(v) {}
	/**
	 * Select all items in the given view
	 */
	virtual void execute()
	{
		if (_view->count())
		{
			_view->selection()->select(0, _view->count()-1);
		}
	}
};

/**
 * Command to clear selection from the current view
 */
class ClearSelectionCommand : public Command
{
	ItemView *_view;
public:
	/**
	 * Constructor giving view
	 */
	ClearSelectionCommand(ItemView *v) : _view(v) {}
	/**
	 * Select all items in the given view
	 */
	virtual void execute() {_view->selection()->clear();}
};

/**
 * Command to invert selection in an item view
 */
class InvertSelectionCommand : public Command
{
	ItemView *_view;
public:
	/**
	 * Constructor giving view
	 */
	InvertSelectionCommand(ItemView *v) : _view(v) {}
	/**
	 * Select all items in the given view
	 */
	virtual void execute()
	{
		if (_view->count())
		{
			_view->selection()->toggle(0, _view->count()-1);
		}
	}
};


}
}

#endif /* TBX_SELECTCOMMANDS_H_ */
