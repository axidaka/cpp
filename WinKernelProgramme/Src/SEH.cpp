#include "stdafx.h"
#include "SEH.h"
#include <tchar.h>

DWORD TryException_test1()
{
	DWORD dwTemp = 0;

	__try{
		dwTemp = 5 / dwTemp; // Generates an exception
		dwTemp +=  10;         // never executes
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		_tprintf(_T("Handle an exception\n"));
		MessageBeep(0);
	}

	_tprintf(_T("Handle after  exception\n"));

	return dwTemp;
}

void TryFinally_test1()
{
	DWORD dwTemp = 0;
	__try{
		_tprintf(_T("WaitForSingleObject Semaphore INFINITE\n"));

		dwTemp = 5 / dwTemp;
	}
	__finally{
		// Global unwind occurs because filter evaluated to EXCEPTION_EXECUTE_HANDER
		_tprintf(_T("Release Semaphore\n"));

	}

	_tprintf(_T("the hander after finally\n"));
}

void GlobalUnwind_test1()
{
	_tprintf(_T("GlobalUnwind test begin\n"));

	__try{
		MultiLayersFinally_test();
		_tprintf(_T("this will happen in try under Release\n"));
	}
	__except (EXCEPTION_EXECUTE_HANDLER){
		// after the unwind, the exception hander executes
		_tprintf(_T("hander the exception\n"));
	}

	_tprintf(_T("the hander after exception\n"));
}

void MultiLayersFinally_test()
{
	__try{
		_tprintf(_T("firstlayer:WaitForSingleObject Semaphore INFINITE\n"));

		DWORD dwTemp = 0;
		__try{
			_tprintf(_T("secondlayer:WaitForSingleObject Semaphore INFINITE\n"));

			dwTemp = 5 / dwTemp;

			_tprintf(_T("this will happen in try under Release\n"));
		}
		__finally{
			// Global unwind occurs because filter evaluated to EXCEPTION_EXECUTE_HANDER
			_tprintf(_T("secondlayer:Release Semaphore\n"));

		}

		_tprintf(_T("secondlayer:the hander after finally\n"));

	}
	__finally{
		_tprintf(_T("firstlayer:Release Semaphore\n"));

	}

	_tprintf(_T("firstlayer:the hander after finally\n"));
}

char g_szBuffer[100];

LONG OilFilter(char** ppchBuffer)
{
	if (*ppchBuffer == NULL)
	{
		*ppchBuffer = g_szBuffer;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

void ContinueExecution_test()
{
	int x = 0;
	char* pchBffer = NULL;

	__try{

		*pchBffer = 'J';
		x = 5 / x;
	}
	__except (OilFilter(&pchBffer)){
		MessageBox(NULL, _T("An exception occured"), NULL, MB_OK);
	}

	MessageBox(NULL, _T("Function Completed"), NULL, MB_OK);

}

PTOP_LEVEL_EXCEPTION_FILTER sm_pfnUnhandledExceptionFilterPrev;
int* g_pInt = NULL;

void UnhandledExeption_test()
{
	sm_pfnUnhandledExceptionFilterPrev = SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//__try{
	//	_tprintf(_T("firstlayer:WaitForSingleObject Semaphore INFINITE\n"));

	//	__try{
	//		_tprintf(_T("secondlayer:WaitForSingleObject Semaphore INFINITE\n"));
	OutputDebugString(L"begin exception");
	int n = 0;
	int y = 5 / n;

	OutputDebugString(L"after exception");

	//	}
	//	__except (EXCEPTION_CONTINUE_SEARCH){
	//		// Global unwind occurs because filter evaluated to EXCEPTION_EXECUTE_HANDER
	//		_tprintf(_T("secondlayer:Release Semaphore\n"));

	//	}

	//	_tprintf(_T("secondlayer:the hander after finally\n"));

	//}
	//__finally{
	//	_tprintf(_T("firstlayer:Release Semaphore\n"));

	//}

	//_tprintf(_T("firstlayer:the hander after finally\n"));
}

LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pep)
{
	OutputDebugString(L"MyUnhandledExceptionFilter");

	// Default to trying another filter (safest thing to do)
	LONG lDisposition = EXCEPTION_CONTINUE_SEARCH;

	// We only fix access violations
	if (pep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
	{
		OutputDebugString(L"We Do Something to fit");
		g_pInt = new int;
		pep->ContextRecord->Eax = (DWORD)g_pInt;
		lDisposition = EXCEPTION_CONTINUE_EXECUTION;
	}

	if (lDisposition == EXCEPTION_CONTINUE_SEARCH)
	{
		OutputDebugString(L"Let Default handle to fit");
		lDisposition =  sm_pfnUnhandledExceptionFilterPrev(pep);
	}

	return (lDisposition);
}

