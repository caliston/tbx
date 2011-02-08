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
// MainWnd.h: interface for the MainWnd class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MAINWND_H
#define MAINWND_H

#include "tbx/saveas.h"
#include "tbx/window.h"
#include "tbx/stringset.h"
#include "tbx/writablefield.h"
#include "tbx/path.h"
#include "tbx/tag.h"
#include "tbx/textchangedlistener.h"
#include <map>

class MainWnd :
   public tbx::SaveAsSaveToFileHandler,
   public tbx::TextChangedListener
{
private:
   virtual void saveas_save_to_file(tbx::SaveAs saveas, bool selection, std::string filename);
   virtual void text_changed(tbx::TextChangedEvent &event);

public:
	MainWnd(tbx::SaveAs saveas);
	virtual ~MainWnd();

	static MainWnd *FromObject(const tbx::Object &object);

    // Attributes
    std::string GetTaskName() const;
    std::string GetAppType() const;
    std::string GetFileType() const;
    std::string GetFileDesc() const;
    std::string GetAppTypeInfo() const;

protected:
	tbx::WritableField m_taskName;
	tbx::StringSet m_types;
	tbx::WritableField m_fileType;
	tbx::WritableField m_fileDesc;
	tbx::TagDoc m_index;
	std::map<std::string, bool> m_hasFileType;
};

#endif
