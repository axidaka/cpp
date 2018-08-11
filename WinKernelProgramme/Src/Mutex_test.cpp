#include "stdafx.h"
#include <process.h>
#include "ToolMacros.h"
#include "StringHelper.h"
#include "ToolFunc.h"
#include "Mutex_test.h"
#include "Log.h"
#include "WinRWFile.h"
#include "RootDirectory.h"

CQueue g_q(10);                     // The shared queue
volatile LONG g_fShutDown = FALSE;  // Signals client/server threads to die

// Handles to all client/server threads & number of client/server threads
HANDLE g_hThreads[MAXIMUM_WAIT_OBJECTS];
int    g_nNumThreads = 0;
CLog<CWinRWFile>   g_log;
///////////////////////////////////////////////////////////////////////////////


CQueue::CQueue(int nMaxElements)
: m_hmtxQ(m_h[0]), m_hsemNumElements(m_h[1])
{
	m_pElements = (PELEMENT)HeapAlloc(GetProcessHeap(), 0, sizeof(ELEMENT)*nMaxElements);
	m_nMaxElements = nMaxElements;

	m_hmtxQ = CreateMutex(NULL, FALSE, NULL);
	m_hsemNumElements = CreateSemaphore(NULL, 0, m_nMaxElements, NULL);
}


///////////////////////////////////////////////////////////////////////////////


CQueue::~CQueue() 
{
	Close_IFHandle(m_hmtxQ);
	Close_IFHandle(m_hsemNumElements);
	HeapFree(GetProcessHeap(), 0, m_pElements);
}


///////////////////////////////////////////////////////////////////////////////

BOOL CQueue::Append(PELEMENT pElement, DWORD dwTimeout) 
{
	BOOL bOk = FALSE;
	DWORD dw = WaitForSingleObject(m_hmtxQ, dwTimeout);

	if (WAIT_OBJECT_0 == dw)
	{///<This Thead has exclusive access to the queue
		///<Increment the number of element int queue
		LONG lPreCount;
		bOk = ReleaseSemaphore(m_hsemNumElements, 1, &lPreCount);
		if (bOk)
		{
			///< The queue is not full, append the element
			m_pElements[lPreCount] = *pElement;
		} 
		else
		{
			SetLastError(ERROR_DATABASE_FULL);
		}

		///< allow other threads to access the queue
		ReleaseMutex(m_hmtxQ);
	} 
	else if (WAIT_TIMEOUT == dw)
	{
		SetLastError(ERROR_TIMEOUT);
	}
	
	return bOk;
}


///////////////////////////////////////////////////////////////////////////////


BOOL CQueue::Remove(PELEMENT pElement, DWORD dwTimeout) 
{
	BOOL bOK = (WAIT_OBJECT_0 == WaitForMultipleObjects(_countof(m_h), m_h, TRUE, dwTimeout));

	if (bOK)
	{
		///< mutex is signed and semaphore currentcount > 0
		*pElement = m_pElements[0];
		MoveMemory(&m_pElements[0], &m_pElements[1], 
			sizeof(ELEMENT)*(m_nMaxElements - 1));

		ReleaseMutex(m_hmtxQ);
	} 
	else
	{
		SetLastError(ERROR_TIMEOUT);
	}

	return bOK;
}

///////////////////////////////////////////////////////////////////////////////


unsigned __stdcall ClientThread(PVOID pvParam) {

	int nThreadNum = PtrToUlong(pvParam);

	int nRequestNum = 0;
	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0)) {

		// Keep track of the current processed element
		nRequestNum++;

		CQueue::ELEMENT e = { nThreadNum, nRequestNum };

		// Try to put an element on the queue
		if (g_q.Append(&e, 200)) {

			// Indicate which thread sent it and which request
			g_log.Write(_T("Thread %d Send: %d\n"), nThreadNum, nRequestNum);
		}
		else {

			// Couldn't put an element on the queue
			g_log.Write(_T("Thread %d Send: %d %s\n"), nThreadNum, nRequestNum,
				(GetLastError() == ERROR_TIMEOUT)? _T("timeout") : _T("full"));
		}

		Sleep(2500);   // Wait before appending another element
	}
	_endthreadex(0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////


unsigned __stdcall ServerThread(PVOID pvParam) {

	int nThreadNum = PtrToUlong(pvParam);

	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0)) {

		CQueue::ELEMENT e;

		// Try to get an element from the queue
		if (g_q.Remove(&e, 5000)) {

			// Indicate which thread is processing it, which thread
			// sent it and which request we're processing
			g_log.Write(_T("Thread %d Processing: %d:%d\n"), 
				nThreadNum, e.m_nThreadNum, e.m_nRequestNum);

			// The server takes some time to process the request
			Sleep(2000 * e.m_nThreadNum);

		}
		else {
			// Couldn't get an element from the queue
			g_log.Write(_T("Thread %d timeout\n"), nThreadNum);
		}
	}

	_endthreadex(0);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
void Mutex_TestMain()
{
	g_log.Write(_T("Input Q or q to Exit\n"));

	ToolFunc::Self.Init(ToolFunc::GetProgramDirectory());
	g_log.OpenW(ToolFunc::Self.GetFullPath(L"run.log").c_str(), CLog<CWinRWFile>::LL_INFORMATION, FALSE);
	g_log.Enable(TRUE);
	g_log.EnableTime(FALSE);
	g_log.EnableConsolePrint(TRUE);

	unsigned dwThreadID;
	///< Create the Client Threads
	for (int x = 0; x < 4; x ++)
	{
		g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, ClientThread, (PVOID)(INT_PTR)x, 0, &dwThreadID);
	}

	///< Create the Sever Threads
	for (int x = 0; x < 2; x ++)
	{
		g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, ServerThread, (PVOID)(INT_PTR)x, 0, &dwThreadID);
	}

	int ch;
	ch = getchar();
	while (ch != 'Q' && ch != 'q')
	{
		g_log.Write(_T("Input Q or q to Exit\n"));

		ch = getchar();
	}

	InterlockedExchange(&g_fShutDown, TRUE);

	WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);
	while (g_nNumThreads--)
		Close_IFHandle(g_hThreads[g_nNumThreads]);

	g_log.Close();
}
