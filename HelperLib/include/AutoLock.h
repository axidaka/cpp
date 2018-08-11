#pragma once

template<class LOCK>
class CAutoLcok
{
public:

	CAutoLcok(LOCK& Lock)
		:m_lock(Lock)
	{
		m_lock.Lock();
	}

	~CAutoLcok()
	{
		m_lock.Unlock();
	}

private:

	LOCK& m_lock;
};