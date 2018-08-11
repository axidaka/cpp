#pragma once


/*
@brief 单线程可变大小内存管理器
*/


class MemoryChunk
{
public:

	MemoryChunk(MemoryChunk* nextChunk, size_t reqSize);
	~MemoryChunk();

	inline void* alloc(size_t size);
	inline void free(void* someElement);

	// Pointer to next memory chunk on the list
	MemoryChunk* nextMemChunk(){return next;}

	// How much space do we have letf on this memory chunk?
	size_t spaceAvailable(){return chunkSize - byteAlreadyAllocated;}

	// this is the default size of a single memory chunk
	enum{DEFAULT_CHUNK_SIZE = 4096};

private:

	MemoryChunk* next;
	void*        mem;

	// the size of a single memory chunk
	size_t chunkSize;

	// This many bytes already allocated on the current memory chunk
	size_t byteAlreadyAllocated;
};

class  ByteMemoryPool
{
public:
	ByteMemoryPool(size_t initSize = MemoryChunk::DEFAULT_CHUNK_SIZE);
	~ByteMemoryPool();

	// Allocate memory from private pool
	inline void* alloc(size_t size);

	// Free memory previously allocated from the pool
	inline void free(void* someElement);

private:

	// A list of memory chunks, this is our private storage
	MemoryChunk* listOfMemorychunks;

	// Add one memory chunk to our private storage
	void expandStorage(size_t reqSize);
};

class Rational3
{
public:

	Rational3(int a = 0, int b = 1)
		:na(a),nb(b)
	{

	}
	~Rational3(void){}

	void *operator new(size_t size);
	void operator delete(void* doomed,size_t size);

	static void newMemPool(){memPool = new ByteMemoryPool;}
	static void deleteMemPool(){delete memPool;}

private:

	int na;
	int nb;
	static ByteMemoryPool* memPool;
};

