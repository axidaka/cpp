#include "StdAfx.h"
#include "Rational3.h"

ByteMemoryPool* Rational3::memPool = NULL;

MemoryChunk::MemoryChunk( MemoryChunk* nextChunk, size_t reqSize )
	:mem(NULL)
{
	chunkSize = (reqSize > DEFAULT_CHUNK_SIZE) ? reqSize : DEFAULT_CHUNK_SIZE;
	next = nextChunk;
	byteAlreadyAllocated = 0;
	mem = new char[chunkSize];
}

MemoryChunk::~MemoryChunk()
{
	if (mem != NULL)
	{
		delete []mem;
	}
}

void* MemoryChunk::alloc( size_t reqSize )
{
	void* addr = static_cast<void*>
		((char*)mem + byteAlreadyAllocated);
	byteAlreadyAllocated += reqSize;
	return addr;
}

void MemoryChunk::free( void* someElement )
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
ByteMemoryPool::ByteMemoryPool( size_t initSize /*= MemoryChunk::DEFAULT_CHUNK_SIZE*/ )
	:listOfMemorychunks(NULL)
{
	expandStorage(initSize);
}

ByteMemoryPool::~ByteMemoryPool()
{
	MemoryChunk* memChunk = listOfMemorychunks;
	while (memChunk != NULL)
	{
		listOfMemorychunks = listOfMemorychunks->nextMemChunk();
		delete memChunk;
		memChunk = listOfMemorychunks;
	}
}

void* ByteMemoryPool::alloc( size_t reqSize )
{
	size_t space = listOfMemorychunks->spaceAvailable();
	if (space < reqSize)
	{
		expandStorage(reqSize);
	}

	return listOfMemorychunks->alloc(reqSize);
}

void ByteMemoryPool::free( void* someElement )
{
	listOfMemorychunks->free(someElement);
}

void ByteMemoryPool::expandStorage( size_t reqSize )
{
	listOfMemorychunks  = new MemoryChunk(listOfMemorychunks, reqSize);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
void * Rational3::operator new( size_t size )
{
	return memPool->alloc(size);
}

void Rational3::operator delete( void* doomed,size_t size )
{
	memPool->free(doomed);
}
