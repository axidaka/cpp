#ifndef __SINGLETON_H_
#define __SINGLETON_H_

#include "stldefine.h"
#include "CriticalSectionLock.h"
#include "AutoLock.h"


/*
模式：单实例模式
*/

template<class T>
class CSingleton
{
public:

	static T* GetInstance()	
	{
		if (!s_instance.get())
		{
			//CAutoLcok<CCriticalSectionLock> autolock(lock);
			if (!s_instance.get())
			{
				s_instance.reset(new T());
			}
		}

		return s_instance.get();
	}

	virtual ~CSingleton(){};

protected:

	explicit CSingleton(){};
	CSingleton(const CSingleton<T> &);
	CSingleton& operator = (const CSingleton<T> &);

private:

	static std::auto_ptr<T> s_instance;
	static CCriticalSectionLock lock;
};

template<typename T>
__declspec(selectany) std::auto_ptr<T> CSingleton<T>::s_instance;

template<typename T>
__declspec(selectany) CCriticalSectionLock CSingleton<T>::lock;


#endif//__SINGLETON_H_