#pragma once

#include <Windows.h>


class CCriticalSectionLock 
{
public:
	CCriticalSectionLock()
	{
		InitializeCriticalSection(&cs);
	}
	virtual ~CCriticalSectionLock()
	{
		DeleteCriticalSection(&cs);
	}

	void Lock()
	{
		EnterCriticalSection(&cs);
	}
	
	BOOL TryLock()
	{
		return TryEnterCriticalSection(&cs);
	}
	void Unlock()
	{
		LeaveCriticalSection(&cs);
	}
private:

	CRITICAL_SECTION cs;
};
