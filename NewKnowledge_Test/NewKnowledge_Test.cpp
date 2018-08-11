// NewKnowledge_Test.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "testfunc.h"
#include "ToolFunc.h"
#include <shlwapi.h>
#include "DllInjectFunc.h"
#include "Use_OpenResourceLib.h"
#include "ExtendPython_test.h"
#include "MyWindowServer.h"

//#ifdef _DEBUG
//#pragma comment(lib, "HelperLibd.lib")
//#pragma comment(lib, "EffectiveC++d.lib")
//#pragma comment(lib, "WinKernelProgrammed.lib")
//#pragma comment(lib, "DataStructHelperd.lib")
//#else
//#pragma comment(lib, "HelperLib.lib")
//#pragma comment(lib, "EffectiveC++.lib")
//#pragma comment(lib, "WinKernelProgramme.lib")
//#pragma comment(lib, "DataStructHelper.lib")
//#endif // DEBUG

#pragma comment(lib, "shlwapi.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	std::wcout.imbue(std::locale("chs"));

	ToolFunc::Self.Init(ToolFunc::GetProgramDirectory());

	MyLog* pMyLog = new MyLog;
	CLogger::GetInstance().SetLogger(pMyLog);
	pMyLog->EnableConsolePrint(TRUE);

	wstring LogPath = ToolFunc::Self.GetFullPath(L"LogDir");
	if (PathIsDirectory(LogPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY)
	{
		ToolFunc::CreateMultipleDirectory(LogPath);
	}

	CLogger::GetInstance().setLogFilename(L"NewKnowledge.log", false);
	
	cout << __FILE__ << endl;
	//ProcessFunc_Test();
	//WriteDump::CreateMiniDump_test();
	//TestDllInject();
	
	//HandleSEHAsCppException();
	//Test();
	Sort_Test();
	//RunPythonFile_Test();

	system("pause");
	
	return 0;
}

