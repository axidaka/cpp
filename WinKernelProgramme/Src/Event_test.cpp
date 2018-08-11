#include "stdafx.h"
#include "Event_test.h"
#include <process.h>
#include "stldefine.h"
#include "ToolMacros.h"
#include <algorithm>
#include "StringHelper.h"
#include "ToolFunc.h"

HANDLE	g_Event = NULL;

TCHAR*  g_shareMem = NULL;


void Event_testfunc(BOOL bManual /*= TRUE*/)
{
	///< Create the ManualReset nosignaled event
	g_Event = CreateEvent(NULL, bManual, FALSE, NULL);
	if (!g_Event)
	{
		_tprintf(_T("CreateEvent Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
	}

	///< Spawn 3 new threads only to read
	HANDLE hThread[3];
	unsigned dwThreadID;

	if (bManual)
	{
		///< 事件对象是人工重置，得到通知时所有线程均变为可调度线程，对于共享内存
		///< 当前线程都只能以读取方式操作
		hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadOutput, NULL, 0, &dwThreadID);
		hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadLength, NULL, 0, &dwThreadID);
		hThread[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadConver, NULL, 0, &dwThreadID);
	}
	else
	{
		///< 事件对象是自动重置，得到通知时只有一个变为可调度线程，对于共享内￿
		///< 当前线程都可以读写操作，操作后需要置事件为有信号
		hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadRead1, NULL, 0, &dwThreadID);
		hThread[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadRead2, NULL, 0, &dwThreadID);
		hThread[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadWrite, NULL, 0, &dwThreadID);
	}


	///< Init shareMem
	if (g_shareMem == NULL)
	{
		g_shareMem = new TCHAR[255];
		_tcscpy_s(g_shareMem, 255, _T("Manual Or Auto Event test"));
	}

	///< allow all 3 threads to access the memory
	SetEvent(g_Event);

	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

	for each (HANDLE& var in hThread)
	{
		Close_IFHandle(var);
	}

	Delete_IFPtr(g_shareMem);

	///< 关闭句柄前要确保所有的Wait都已经完成，要不然Wait结果未定义，导致线程挂起
	Close_IFHandle(g_Event);

}

unsigned __stdcall  ThreadOutput(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
		_tprintf(_T("ThreadOutput:%s\n"), g_shareMem);
		break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadOutput Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadOutput: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);
	return 0;
}

unsigned __stdcall  ThreadLength(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
		_tprintf(_T("ThreadLength:%d\n"), _tcslen(g_shareMem));
		break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadLength Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadLength: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);

	return 0;
}

unsigned __stdcall  ThreadConver(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
		_tprintf(_T("ThreadConver Upper:%s\n"), StringHelper::ToUpper(g_shareMem).c_str());
		break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadConver Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadConver: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);

	return 0;
}

unsigned __stdcall ThreadRead1(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
	{
		_tprintf(_T("ThreadRead1 :%s\n"), g_shareMem);
		SetEvent(g_Event);
	}
	break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadRead1 Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadRead1: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);

	return 0;
}

unsigned __stdcall ThreadRead2(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
	{
		_tprintf(_T("ThreadRead2 :%s\n"), g_shareMem);
		SetEvent(g_Event);
	}
	break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadRead2 Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadRead2: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);

	return 0;
}

unsigned __stdcall ThreadWrite(PVOID pvParam)
{
	DWORD dwRet = WaitForSingleObject(g_Event, INFINITE);

	switch (dwRet)
	{
	case WAIT_OBJECT_0:
	{
		_tcscpy_s(g_shareMem, 255, _T("Changed By ThreadWrite"));
		_tprintf(_T("ThreadWrite :%s\n"), g_shareMem);

		SetEvent(g_Event);
	}
	break;
	case WAIT_FAILED:
	{
		_tprintf(_T("ThreadWrite Wait Failed: %s\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		if (g_Event == NULL)
		{
			_tprintf(_T("ThreadWrite: g_event has been NULL\n"));
		}
	}
	break;
	}

	_endthreadex(0);

	return 0;
}
