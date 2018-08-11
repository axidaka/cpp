#pragma once
#include <windows.h>

namespace WriteDump
{
	void CreateMiniDumpFile(LPCWSTR lpstrMiniDumpFilePathName, EXCEPTION_POINTERS* pException);
	
	void CreateMiniDump_test();
}