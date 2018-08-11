#pragma once

#include <Windows.h>
#include<iostream>
#include <string>
using namespace std;

class Locker
{
public:

	Locker(){}
	virtual ~Locker(){}
	virtual void lock() = 0;
	virtual void unlock() = 0;
};

class CriticalSectionLock : public Locker
{
public:
	CriticalSectionLock()
	{
		InitializeCriticalSection(&cs);
		//cout<<"CriticalSectionLock Constructor"<<endl;
	}
	virtual ~CriticalSectionLock()
	{
		DeleteCriticalSection(&cs);
		//cout<<"CriticalSectionLock Destroy"<<endl;
	}

	virtual void lock()
	{
		EnterCriticalSection(&cs);
		//cout<<"CriticalSectionLock lock"<<endl;
	}

	virtual void unlock()
	{
		LeaveCriticalSection(&cs);
		//cout<<"CriticalSectionLock unlock"<<endl;
	}
private:

	CRITICAL_SECTION cs;
};


template<class LOCKER>
class ThreadSafeString : public string
{
public:
	ThreadSafeString(const char* s)
		:string(s)
	{

	}

	int length();

private:
	LOCKER lock;
};

template<class LOCKER>
inline 
int ThreadSafeString<LOCKER>::length()
{
	lock.lock();
	int len = string::length();
	lock.unlock();

	return len;
}