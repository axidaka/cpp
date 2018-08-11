#pragma once

#include <string>
using namespace std;

/*
散列表
解决键冲突的方法有: 分离链接法和开放定址法
分离链接法:将散列到同一个值的所有元素保留到一个链表中
*/
template <typename HashedObj>
class HashTable
{
  public:
    explicit HashTable( int size = 101 );

    bool contains( const HashedObj & x ) const;

    void makeEmpty( );
    void insert( const HashedObj & x );
    void remove( const HashedObj & x );

  private:
    vector<list<HashedObj> > theLists;   // The array of Lists
    int  currentSize;

    void rehash( );
    int myhash( const HashedObj & x ) const;
};

int hash( const string & key );
int hash( int key );

template <typename HashedObj>
int HashTable::myhash(const HashedObj & x) const
{
	int hashVal = hash(x);

	hashVal %= theLists.size();
	if (hashVal < 0)
		hashVal += theLists.size();

	return hashVal;
}

template <typename HashedObj>
void HashTable::makeEmpty()
{
	for (int i = 0; i < theLists.size(); i++)
		theLists[i].clear();
}

template <typename HashedObj>
bool HashTable::contains(const HashedObj & x) const
{
	const list<HashedObj> & whichList = theLists[myhash(x)];
	return find(whichList.begin(), whichList.end(), x) != whichList.end();
}

template <typename HashedObj>
bool HashTable::remove(const HashedObj & x)
{
	list<HashedObj> & whichList = theLists[myhash(x)];
	list<HashedObj>::iterator itr = find(whichList.begin(), whichList.end(), x);

	if (itr == whichList.end())
		return false;

	whichList.erase(itr);
	--currentSize;
	return true;
}

template <typename HashedObj>
bool HashTable::insert(const HashedObj & x)
{
	list<HashedObj> & whichList = theLists[myhash(x)];
	if (find(whichList.begin(), whichList.end(), x) != whichList.end())
		return false;
	whichList.push_back(x);

	// Rehash; see Section 5.5
	if (++currentSize > theLists.size())
		rehash();

	return true;
}
