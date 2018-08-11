#pragma once

#include "ThreadSafeString.h"
#include "Rational2.h"
#include "Rational3.h"

template<class POOLTYPE, class LOCK>
class MultiThreadMemPool
{
public:

	// Allocate an element from the freeList
	inline void* alloc(size_t reqSize);
	
	// Return an element to the freeList
	inline void  free(void* someElement);

private:

	POOLTYPE  singleThreadPool; // Single - thread pool
	LOCK      theLock;
};


template<class MP, class L>
inline 
void* MultiThreadMemPool<MP,L>::alloc(size_t reqSize)
{
	void* mem;
	theLock.lock();
	mem = singleThreadPool.alloc(reqSize);
	theLock.unlock();

	return mem;
}

template<class MP, class L>
inline 
void MultiThreadMemPool<MP, L>::free(void* someElement)
{
	theLock.lock();
	singleThreadPool.free(someElement);
	theLock.unlock();
}

class Rational4
{
public:

	Rational4(int a = 0, int b = 1)
		:na(a),nb(b)
	{

	}
	~Rational4(void){}

	void *operator new(size_t size){return memPool->alloc(size);}
	void operator delete(void* doomed,size_t size){memPool->free(doomed);}

	static void newMemPool(){memPool = new MultiThreadMemPool<ByteMemoryPool, CriticalSectionLock>;}
	static void deleteMemPool(){delete memPool;}

private:

	int na;
	int nb;
	static MultiThreadMemPool<ByteMemoryPool, CriticalSectionLock>* memPool;
};

MultiThreadMemPool<ByteMemoryPool, CriticalSectionLock>* Rational4::memPool = NULL;

