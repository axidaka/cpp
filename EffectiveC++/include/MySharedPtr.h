#pragma once

#include "RefCountObject.h"

template<class _Ty>
class CMySharedPtr 
{
	typedef CRefCountObject<_Ty> _RefCtTy;
public:

	CMySharedPtr() :m_pRefCountObject(NULL){}
	CMySharedPtr(_Ty* realPtr = NULL) :m_pRefCountObject(new _RefCtTy(realPtr)){}
	CMySharedPtr(const CMySharedPtr& rhs) :m_pRefCountObject(rhs.m_pRefCountObject)
	{
		m_pRefCountObject->AddRef();
	}
	CMySharedPtr& operator= (const CMySharedPtr& rhs)
	{
		if (*this != rhs && this->m_pRefCountObject != rhs.m_pRefCountObject)
		{
			m_pRefCountObject->RemoveRef();
			m_pRefCountObject = rhs.m_pRefCountObject;
		}

		return *this;
	}

	~CMySharedPtr()
	{
		if (m_pRefCountObject != NULL) 
			m_pRefCountObject->RemoveRef();
	}
	
	_Ty* get()const;
	_Ty* operator ->()const;
	long use_count()const;

private:

	CRefCountObject<_Ty>* m_pRefCountObject;
};

template<class _Ty>
_Ty* CMySharedPtr<_Ty>::get()const
{
	if (m_pRefCountObject != NULL)
	{
		return m_pRefCountObject->_Get();
	}

	return NULL;
}

template<class _Ty>
_Ty* CMySharedPtr<_Ty>::operator ->()const
{
	if (m_pRefCountObject != NULL)
	{
		return m_pRefCountObject->_Get();
	}

	return NULL;
}

template<class _Ty>
long CMySharedPtr<_Ty>::use_count()const
{
	return m_pRefCountObject->_UseCount();
}