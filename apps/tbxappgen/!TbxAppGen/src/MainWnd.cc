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
// MainWnd.cpp: implementation of the MainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "MainWnd.h"
#include "Res.h"
#include "tbx/path.h"
#include "tbx/reporterror.h"
#include "AppGenerator.h"

#include <fstream>

using namespace tbx;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainWnd::MainWnd(SaveAs saveas)
{
    Window w = saveas.window();
    w.client_handle(this);
	m_taskName = w.gadget(CID_MAIN_TASK_NAME);
	m_types = w.gadget(CID_MAIN_APP_TYPE);
	m_fileType= w.gadget(CID_MAIN_FILE_TYPE);
	m_fileDesc = w.gadget(CID_MAIN_FILE_TYPE_NAME);

    ifstream ind("<TbxAppGen$Dir>.Skeletons.Index.Base");
    ind >> m_index;

    Tag *skeletons = m_index.find_child("Skeletons");
    if (skeletons == 0) report_error("Unable to read base index file");
    Tag *skel = skeletons->first_child();
    int name_id = m_index.attribute_id("Name");
    int file_type_id = m_index.attribute_id("FileType");
    std::string list;
    std::string name;
    while (skel)
    {
       if (!list.empty()) list += ',';
       name = skel->attribute_value(name_id);
       if (skel->find_attribute(file_type_id))
       {
           m_hasFileType[name] = true;
       }
       list += name;
       skel = skel->next();
    }

  	m_types.available(list);
	m_types.selected_index(0);
	m_types.add_text_changed_listener(this);

    saveas.set_save_to_file_handler(this);
}

MainWnd::~MainWnd()
{
}

MainWnd *MainWnd::FromObject(const tbx::Object &object)
{
	return reinterpret_cast<MainWnd *>(object.client_handle());
}


std::string MainWnd::GetTaskName() const
{
	std::string taskName = m_taskName.text();

	return taskName;
}

std::string MainWnd::GetAppType() const
{
    return m_types.selected();
}

std::string MainWnd::GetFileType() const
{
    return m_fileType.text();
}

std::string MainWnd::GetFileDesc() const
{
    return m_fileDesc.text();
}

/**
 * Get the description for the application type
 */
std::string MainWnd::GetAppTypeInfo() const
{
    Tag *skels = m_index.first_child();
    Tag *skel = skels->find_child("Skeleton", "Name", GetAppType());
	std::string rawInfo = (skel == 0) ? "Can't find application details" : skel->text();
	std::string appInfo;
	// Trim text by removing line feeds and spaces at the beginning of lines.
	// 2 linefeeds in a row are used to keep a line feed.
	std::string::size_type lfPos;
	std::string::size_type spPos = 0;
	do
	{
	   lfPos = rawInfo.find('\n', spPos);
	   if (lfPos == std::string::npos) lfPos = rawInfo.size();
	   while (spPos < lfPos && rawInfo[spPos] == ' ') spPos++;
	   appInfo += rawInfo.substr(spPos, lfPos - spPos);
	   spPos = lfPos+1;
	   if (spPos < rawInfo.size())
	   {
	       if (rawInfo[spPos] == '\n')
    	   {
    	      appInfo += '\n';
    	      spPos++;
    	   } else
    	   {
    	      appInfo += ' ';
    	   }
       }
	} while (spPos < rawInfo.size());

	return appInfo;
}

/**
 * Type has changed so see if file type can be enabled
 */
void MainWnd::text_changed(tbx::TextChangedEvent &event)
{
    bool fade = !m_hasFileType[event.text()];
    m_fileType.fade(fade);
    m_fileDesc.fade(fade);
}


void MainWnd::saveas_save_to_file(SaveAs saveas, bool selection, std::string filename)
{
    std::string id;
    Tag *skels = m_index.first_child();
    Tag *skel = skels->find_child("Skeleton", "Name", GetAppType());
    if (skel == 0)
    {
    	string msg("Can't find application type ");
    	msg += GetAppType();
    	report_error(msg.c_str());
    	saveas.file_save_completed(false, filename);
    	return;
    }

    AppGenerator appGen(skel->attribute_value("ID"), filename, GetTaskName());

    TagAttribute *file_type = skel->find_attribute("FileType");
    if (file_type)
    {
       appGen.SetFileType(GetFileType());
       appGen.SetFileDescription(GetFileDesc());
       if (file_type->has_value() && file_type->value() == "ReadOnly")
       {
          appGen.SetFileTypeReadOnly(true);
       }
    }

    if (skel->find_attribute("MakeFile"))
    {
        appGen.SetStandardMakeFile(true);
    }

    appGen.Generate();

    saveas.file_save_completed(true, filename);
}
