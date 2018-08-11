#pragma once

#include "stldefine.h"

namespace FileHelper
{
	BOOL MyReadFile(LPCTSTR szFileName, std::string &szBuffer);
	BOOL MyWriteFile(LPCTSTR szFileName, const char *pBuffer, int nSize, bool bAppend = false);

	std::list<std::wstring> GetDirList(std::wstring searchPath);
	std::list<std::wstring> GetFileList(std::wstring searchPath, wstring fileExtension);
}