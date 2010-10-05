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

#include "reporterror.h"
#include "tbxexcept.h"
#include "swis.h"

#include <cstring>

using namespace std;

namespace tbx
{

ReportErrorReply report_error(_kernel_oserror *err, const char *title, int flags /*= 0*/)
{
	if (!err) return NONE;

    _kernel_swi_regs regs;

	regs.r[0] = reinterpret_cast<int>(err);
	regs.r[1] = flags;
	regs.r[2] = reinterpret_cast<int>(title);

	_kernel_swi(Wimp_ReportError, &regs, &regs);

	return ReportErrorReply(regs.r[1]);
}

ReportErrorReply report_error(_kernel_oserror *err, int flags /*= 0*/)
{
	if (!err) return NONE;

	_kernel_swi_regs regs;
	char appName[128];

    // Use Toolbox_GetSysInfo to get the application name
    regs.r[0] = 0;
    regs.r[1] = reinterpret_cast<int>(appName);
	if (_kernel_swi(0x44ece, &regs, &regs) != 0)
		strcpy(appName, "Application");

    return report_error(err, appName, flags);
}

//****************************************************************************

ReportErrorReply report_error(const char* msg, int flags /* = 0*/)
{
	_kernel_oserror err;
	err.errnum = 0;
	strncpy(err.errmess, msg, 251);
	err.errmess[251] = 0;

	return report_error(&err);
}

ReportErrorReply report_error(const char *msg, const char *title, int flags /*= 0*/)
{
	_kernel_oserror err;
	err.errnum = 0;
	strncpy(err.errmess, msg, 251);
	err.errmess[251] = 0;

	return report_error(&err, title, flags);
}

}
