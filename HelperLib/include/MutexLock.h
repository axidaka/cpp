#pragma once

#include <windows.h>
#include "Logger.h"

class CMutexLock
{
public:

	CMutexLock(HANDLE hMutex)
		:m_hMutex(hMutex)
	{
		//CLogger::GetInstance().logEventFormat(Warnings, TRUE, L"Enter MutexLock:%d\n", GetCurrentThreadId());
		::WaitForSingleObject(hMutex, INFINITE);
	}

	~CMutexLock()
	{
		//CLogger::GetInstance().logEventFormat(Warnings, TRUE, L"Release MutexLock:%d-----------\n", GetCurrentThreadId());
		::ReleaseMutex(m_hMutex);
	}

private:
   HANDLE m_hMutex;
};