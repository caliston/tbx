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
// GenAppSaver.h: interface for the GenAppSaver class.
//
//////////////////////////////////////////////////////////////////////

#ifndef APPGENERATOR_H
#define APPGENERATOR_H

#include "tbx/Path.h"
#include <string>
#include <vector>
#include <map>

class MainWnd;

class AppGenerator
{
public:
	AppGenerator(std::string id, std::string filename, std::string taskName);
	virtual ~AppGenerator();

    void SetFileType(std::string fileType) {m_fileType = fileType;}
    void SetFileDescription(std::string desc) {m_fileDesc = desc;}
    void SetStandardMakeFile(bool useStandard) {m_standardMakeFile = true;}
    void SetFileTypeReadOnly(bool ftReadOnly) {m_fileTypeReadOnly = ftReadOnly;}

    void Generate();

protected:
    void CreateBootFile();
    void CreateRunFile();
    void CreateMessagesFile();
    void CopyAndScanResFile();
    void CopyFiles(const tbx::Path &fromPath, const tbx::Path &toPath);
    void CopySourceDirectory(const tbx::Path &from, const tbx::Path &to);
    void CopyWithReplace(const tbx::Path &from, const tbx::Path &to);
 	void CreateSpriteFile();
 	void CreateMakeFile();

	void ReplaceTokens(char *&data, int &dataLength);
	bool Replace(std::string &text, const std::string &what, const std::string &with);

    void LoadModuleInfo();
    void AddObjectModule(int object_class);
    void AddGadgetModule(int gadget_class);

protected:
    std::string m_id;
    std::string m_appName;
    std::string m_taskName;
    std::string m_fileType;
    std::string m_fileDesc;

	tbx::Path m_targetPath;
	tbx::Path m_sourcePath;
	std::string m_appNameLC;
	std::vector< std::string> m_runCommon;
	bool m_standardMakeFile;
	bool m_makeFileFound;
	bool m_fileTypeReadOnly;

    // Module information variables structures
	struct ModuleInfo
	{
	   ModuleInfo() {}
	   ModuleInfo(const std::string &n, const std::string &ver) : name(n), version(ver) {}
	   std::string name;
	   std::string version;
	};
	std::map<int, ModuleInfo> m_objectModules;
	std::map<int, ModuleInfo> m_gadgetModules;

    std::map<std::string, std::string> m_modulesFound;

};

#endif // !defined(AFX_GENAPPSAVER_H__041ED09D_BDF5_438E_AF92_D8FEBFC26FAB__INCLUDED_)
