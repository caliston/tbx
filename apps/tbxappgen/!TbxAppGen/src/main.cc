/*
 * TbxAppGen tbx RISC OS toolbox library Application generator
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
// main.cc
//
// Entry point for TbxAppGen program
//
// A.R.Buckley
// October 2010
/////////////////////////////////////////////////////////

#include "tbx/application.h"
#include "tbx/matchlifetime.h"
#include "MainWnd.h"
#include "AppInfoWnd.h"

int main(int argc, char *argv[])
{
	tbx::Application genApp("<TbxAppGen$Dir>");

    tbx::MatchLifetime<MainWnd> main("Main");
    tbx::MatchLifetime<AppInfoWnd> app_info("AppTypeInfo");

	genApp.run();

	return 0;
}
