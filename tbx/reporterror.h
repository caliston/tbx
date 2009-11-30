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

/*
 * reporterror.h
 *
 *  Created on: 10-Sep-2008
 *      Author: alanb
 */

#ifndef REPORTERROR_H_
#define REPORTERROR_H_

#include "kernel.h"

namespace tbx
{
	enum ReportErrorReply {NONE, OK, CANCEL};
	enum ReportErrorFlags
	{
		REF_OK_BUTTON,
		REF_CANCEL_BUTTON,
		REF_HIGHLIGHT_CANCEL,
		REF_NO_RETURN_TO_CONTINUE,
		REF_NO_ERROR_FROM_PREFIX,
		REF_LEAVE_OPEN,
		REF_CLOSE
	};

	ReportErrorReply report_error(_kernel_oserror *err, const char *title, int flags = 0);
	ReportErrorReply report_error(_kernel_oserror *err, int flags = 0);
	ReportErrorReply report_error(const char* msg, int flags = 0);
	ReportErrorReply report_error(const char* msg, const char *title, int flags = 0);

	void report_error_close(ReportErrorReply reply);
}

#endif /* REPORTERROR_H_ */
