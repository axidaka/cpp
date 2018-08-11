#pragma once

#include "stldefine.h"
#include <windows.h>

/************************************************************************/
/*�����ڳ����쳣����ʱ����dump�ļ�
/************************************************************************/
namespace ToolFunc
{
	std::wstring GetProgramDirectory();

	tstring FormatErrStr(const DWORD dwError);

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	std::wstring GetSubDirectory(const std::wstring& parent, const std::wstring& dir, bool create);

	bool CreateMultipleDirectory(std::wstring path);//Ŀ¼�������Ҫ "\\"
}

