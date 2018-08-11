#pragma once

/*
@brief 固定大小对象的内存池模板，可实现管理对象多样
@author BigSong
@date 2014/3/12
*/

template<class T>
class MemoryPool
{
public:

	MemoryPool(size_t size = EXPANSION_SIZE);
	~MemoryPool();

	void FreeMemPool();
	// Allocate a T element from the free list
	inline void *alloc(size_t size);
	// Return a T element to the free list
	inline void free(void* someElement);

private:

	// next element on the free list
	MemoryPool<T> *m_nextptr;

	// if the freelist is empty, expand it by this amount
	enum{EXPANSION_SIZE =32};

	// Add free element to the free list
	void expandTheFreeList(int howMany = EXPANSION_SIZE);

};

template<class T>
MemoryPool<T>::MemoryPool(size_t size)
	:m_nextptr(NULL)
{
	expandTheFreeList(size);
}

template<class T>
MemoryPool<T>::~MemoryPool()
{

}

template<class T>
void MemoryPool<T>::FreeMemPool()
{
	MemoryPool<T>* nextPtr = NULL;
	for (nextPtr = m_nextptr; nextPtr != NULL; nextPtr = m_nextptr)
	{
		m_nextptr = m_nextptr->m_nextptr;
		delete nextPtr;
	}
}

template<class T>
inline 
void* MemoryPool<T>::alloc(size_t size)
{
	if (m_nextptr == NULL)
	{
		expandTheFreeList();
	}

	MemoryPool<T>* head = m_nextptr;
	m_nextptr = m_nextptr->m_nextptr;
	return head;
}

template<class T>
inline
void MemoryPool<T>::free(void* doomed)
{
	MemoryPool<T>* head = reinterpret_cast<MemoryPool<T>*>(doomed);
	head->m_nextptr = m_nextptr;
	m_nextptr = head;
}

template<class T>
void MemoryPool<T>::expandTheFreeList(int howMany /* = EXPANSION_SIZE */)
{
	// We must allocate an object large enough to contain the nextPointer
	size_t size = (sizeof(T) > sizeof(MemoryPool<T>*)) ? 
		sizeof(T) : sizeof(MemoryPool<T>*);
	
	MemoryPool<T>* runner = reinterpret_cast<MemoryPool<T>*> (new char[size]);
	m_nextptr = runner;

	for (int i = 0; i < howMany; i++)
	{
		runner->m_nextptr = reinterpret_cast<MemoryPool<T>*>(new char[size]);
		runner = runner->m_nextptr;
	}

	runner->m_nextptr = NULL;
}


class Rational2
{
public:
	Rational2(int a = 0, int b = 1)
		:na(a),nb(b)
	{

	}

	void* operator new(size_t size)
	{
		return memPool->alloc(size);
	}
	void  operator delete(void* doomed,size_t size)
	{	
		memPool->free(doomed);
	}

	static void newMemPool()
	{
		if (memPool != NULL)
		{
			deleteMemPool();
		}

		memPool = new MemoryPool<Rational2>;
	}
	static void deleteMemPool()
	{
		if (memPool != NULL)
		{
			memPool->FreeMemPool();
			delete memPool;
			memPool = NULL;
		}
		
	}

private:

	int na;
	int nb;

	static MemoryPool<Rational2>* memPool;
};

MemoryPool<Rational2>* Rational2::memPool = NULL;
