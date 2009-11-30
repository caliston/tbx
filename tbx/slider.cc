/*
 * slider.cc
 *
 *  Created on: 27-Mar-2009
 *      Author: alanb
 */

#include "slider.h"
#include "swixcheck.h"
#include "kernel.h"

namespace tbx
{

/**
 * Set all three bounds at once
 */
void Slider::set_bounds(int lower, int upper, int step_size)
{
	_kernel_swi_regs regs;
	regs.r[0] = 7;
	regs.r[2] = 578;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = upper;
	regs.r[5] = lower;
	regs.r[6] = step_size;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get all three bounds at once
 */
void Slider::get_bounds(int &lower, int &upper, int &step_size) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 7;
	regs.r[2] = 579;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	upper = regs.r[0];
	lower = regs.r[1];
	step_size = regs.r[2];
}

/**
 * Set lower and upper bounds without changing the step size
 */
void Slider::set_bounds(int lower, int upper)
{
	_kernel_swi_regs regs;
	regs.r[0] = 3;
	regs.r[2] = 578;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = upper;
	regs.r[5] = lower;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}

/**
 * Get all lower and upper bounds at once
 */
void Slider::get_bounds(int &lower, int &upper) const
{
	_kernel_swi_regs regs;
	regs.r[0] = 3;
	regs.r[2] = 579;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	upper = regs.r[0];
	lower = regs.r[1];
}

/**
 * Set Lower bound of slider
 */
void Slider::lower_bound(int value)
{
	_kernel_swi_regs regs;
	regs.r[0] = 2;
	regs.r[2] = 578;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = value;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}
/**
 * Get Lower bound of slider
 */
int Slider::lower_bound() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 2;
	regs.r[2] = 579;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[1];
}

/**
 * Set Upper bound of slider
 */
void Slider::upper_bound(int value)
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[2] = 578;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = value;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}
/**
 * Get Upper bound of slider
 */
int Slider::upper_bound() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 1;
	regs.r[2] = 579;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[0];
}

/**
 * Set step sizeof slider
 */
void Slider::step_size(int value)
{
	_kernel_swi_regs regs;
	regs.r[0] = 4;
	regs.r[2] = 578;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	regs.r[4] = 0;
	regs.r[5] = 0;
	regs.r[6] = value;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));
}
/**
 * Get step size of slider
 */
int Slider::step_size() const
{
	_kernel_swi_regs regs;
	regs.r[0] = 4;
	regs.r[2] = 579;
	regs.r[1] = _handle;
	regs.r[3] = _id;
	swix_check(_kernel_swi(0x44ec6, &regs, &regs));

	return regs.r[2];
}

}
