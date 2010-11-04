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
 * scalefactors.h
 *
 *  Created on: 2 Nov 2010
 *      Author: alanb
 */

#ifndef TBX_SCALEFACTORS_H_
#define TBX_SCALEFACTORS_H_

namespace tbx
{
/**
 * Class for sprite ScaleFactors.
 *
 * ScaleFactors are used to scale a coordinate using integer values
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
	const int *as_array() const {return &_xmult;}

};

}

#endif /* TBX_SCALEFACTORS_H_ */
