#pragma once

template<class _Ty>
class CRefCountObject
{
public:

	CRefCountObject(_Ty* _pointee) :m_pointee(_pointee), m_lRefCount(1){}

	CRefCountObject(const CRefCountObject& rhs)
	{
		rhs.AddRef();
		m_lRefCount = rhs.m_lRefCount;
		m_pointee = rhs.m_pointee;
	}

	CRefCountObject& operator = (const CRefCountObject& rhs)
	{
		rhs.AddRef();
		m_lRefCount = rhs.m_lRefCount;
		m_pointee = rhs.m_pointee;

		return this;
	}

	virtual ~CRefCountObject()
	{
		if (m_pointee != NULL)
		{
			delete m_pointee;
			m_pointee = NULL;
		}
	}

	_Ty* _Get()const
	{
		return (m_pointee != NULL ? m_pointee : NULL);
	}

	long _UseCount()const
	{
		return (m_pointee !=NULL? m_lRefCount : 0);
	}


	void AddRef();
	void RemoveRef();


private:

	long m_lRefCount;  
	bool m_bShareable; 

	_Ty* m_pointee;
};

template<class _Ty>
void CRefCountObject<_Ty>::AddRef()
{
	InterlockedIncrement(&m_lRefCount);
}

template<class _Ty>
void CRefCountObject<_Ty>::RemoveRef()
{
	long lRefCount = InterlockedDecrement(&m_lRefCount);
	if (0 == lRefCount)
	{
		delete this;
	}
}