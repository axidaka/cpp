#pragma once

/*
@brief 专用Rational的单线程内存池管理器
@author BigSong
@date 2014/3/12
*/

//class Rational
//{
//public:
//
//	Rational(int a = 0, int b =1)
//	:na(a),nb(b)
//	{}
//
//private:
//	int na;
//	int nb;
//};

class NextOnFreeList
{
public:
	NextOnFreeList* next;
};

class Rational
{
public:

	Rational(int a = 0, int b =1)
		:na(a),nb(b)
	{}
	
	inline void* operator new(size_t size)
	{
		if (freeList == NULL)
		{
			expandTheFreeList();
		}

		NextOnFreeList* head = freeList;
		freeList = head->next;
		return head;
	}
	inline void operator delete(void* doomed)
	{
		NextOnFreeList* head = reinterpret_cast<NextOnFreeList*> (doomed);
		head->next = freeList;
		freeList = head;
	}

	static void newMemPool(){expandTheFreeList();}
	static void deleteMemPool();

private:
	int na;
	int nb;

	static NextOnFreeList* freeList;
	static void expandTheFreeList();
	enum{EXPANSION_SIZE = 32};
};

