#include "stdafx.h"
#include "Semaphore_test.h"
#include <process.h>
#include "ToolMacros.h"
#include <algorithm>
#include "ToolFunc.h"
#include <basetsd.h>

HANDLE g_hSemaphore = NULL;

#define MAX_SEM_COUNT 5
#define THREADCOUNT 12

void SemaphoreDown_testfunc()
{
	///< Create a semaphore with initial and max counts of MAX_SEM_COUNT
	g_hSemaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, NULL);
	if (!g_hSemaphore)
	{
		_tprintf(_T("CreateEvent Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
	}

	HANDLE hThread[THREADCOUNT];

	unsigned dwThreadID;

	///< Create worker threads
	for (int i = 0; i < THREADCOUNT; ++i)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadDownProc, (PVOID)(INT_PTR)i, 0, &dwThreadID);

		if (hThread[i] == NULL)
		{
			_tprintf(_T("_beginthreadex failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		}
	}

	///< wait for all threads to exit
	WaitForMultipleObjects(THREADCOUNT, hThread, TRUE, INFINITE);

	for each (HANDLE var in hThread)
	{
		Close_IFHandle(var);
	}

	Close_IFHandle(g_hSemaphore);
}

unsigned __stdcall ThreadDownProc(PVOID pvParam)
{
	int nThreadNum = PtrToUlong(pvParam);

	BOOL bContinue = TRUE;

	while (bContinue)
	{
		DWORD dwRet = WaitForSingleObject(g_hSemaphore, INFINITE);
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
		{
			_tprintf(_T("Thread %d Wait Success\n"), nThreadNum);
			bContinue = FALSE;

			Sleep(5000);

			if (!ReleaseSemaphore(g_hSemaphore, 1, NULL))
			{
			  _tprintf(_T("ReleaseSemaphore Failed: %s"), ToolFunc::FormatErrStr(GetLastError()).c_str());
			}
		}
			break;
		case WAIT_FAILED:
			_tprintf(_T("THread %d Wait Failed\n"), nThreadNum);
			break;
		}
	}

	_endthreadex(0);
	return 0;
}
