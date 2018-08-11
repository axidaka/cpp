#include "stdafx.h"
#include "WriteDump.h"
#include "Logger.h"
#include "ToolFunc.h"
#include <DbgHelp.h>
#include "RootDirectory.h"

#pragma comment(lib, "DbgHelp.lib")

namespace WriteDump
{
	void CreateMiniDumpFile(LPCWSTR lpstrMiniDumpFilePathName, EXCEPTION_POINTERS* pException)
	{
		HANDLE hDumpFile = CreateFile(lpstrMiniDumpFilePathName, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hDumpFile)
		{
			CLogger::GetInstance().logEventFormat(Errors, FALSE, L"CreateMiniDumpFile-CreateFile Failed(%s)", 
				ToolFunc::FormatErrStr(GetLastError()).c_str());
			return;
		}

		// Dump ��Ϣ
		MINIDUMP_EXCEPTION_INFORMATION dumpinfo;
		dumpinfo.ThreadId = GetCurrentThreadId();
		dumpinfo.ExceptionPointers = pException;
		dumpinfo.ClientPointers = TRUE;

		// д��Dump�ļ���Ϣ
		if (!MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithIndirectlyReferencedMemory,
			&dumpinfo, NULL, NULL))
		{
			CLogger::GetInstance().logEventFormat(Errors, FALSE, L"MiniDumpWriteDump Failed(%s)",
				ToolFunc::FormatErrStr(GetLastError()).c_str());
		}

		CloseHandle(hDumpFile);
	}

	// ����UNhandle Exception�Ļص�����
	LONG MyUnhandledExceptionFilter(EXCEPTION_POINTERS* pException)
	{
		CreateMiniDumpFile(ToolFunc::Self.GetFullPath(L"MiniDump.dmp").c_str(), pException);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	struct A
	{
		int a;
		void Print()
		{
			printf("a: %d\n", a);
		}
	};
	struct B
	{
		A* pA;
		B() : pA(0) {}
	};

	void CreateMiniDump_test()
	{
		// ����UNhandle Exception�Ļص�����
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)MyUnhandledExceptionFilter);

		//int i = 1;
		//int j = 0;
		//int k = i / j;

		B* pB = new B();
		pB->pA->Print();
	}

}