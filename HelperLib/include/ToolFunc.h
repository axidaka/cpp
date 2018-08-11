#pragma once

#include "stldefine.h"
#include <windows.h>

/************************************************************************/
/*用于在程序异常崩溃时生成dump文件
/************************************************************************/
namespace ToolFunc
{
	std::wstring GetProgramDirectory();

	tstring FormatErrStr(const DWORD dwError);

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	std::wstring GetSubDirectory(const std::wstring& parent, const std::wstring& dir, bool create);

	bool CreateMultipleDirectory(std::wstring path);//目录最后面需要 "\\"
}

