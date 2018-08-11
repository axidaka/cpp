#pragma once

/**
二叉堆
堆:堆是一种经过排序的完全二叉树，其中任一非终端节点的数据值均不大于（或不小于）其左孩子和右孩子节点的值。
*/

template <typename Comparable>
class BinaryHeap
{
  public:
    explicit BinaryHeap( int capacity = 100 );
    explicit BinaryHeap( const vector<Comparable> & items );

    bool isEmpty( ) const;
    const Comparable & findMin( ) const;

    void insert( const Comparable & x );
    void deleteMin( );
    void deleteMin( Comparable & minItem );
    void makeEmpty( );

  private:
    int                currentSize;  // Number of elements in heap
    vector<Comparable> array;        // The heap array

    void buildHeap( );
    void percolateDown( int hole );
};

template<class Comparable>
explicit BinaryHeap::BinaryHeap( const vector<Comparable> & items )
  : array( items.size( ) + 10 ), currentSize( items.size( ) )
{
    for( int i = 0; i < items.size( ); i++ )
        array[ i + 1 ] = items[ i ];
    buildHeap( );
}

/**
 * Establish heap order property from an arbitrary
 * arrangement of items. Runs in linear time.
 */
template<class Comparable>
void BinaryHeap::buildHeap( )
{
    for( int i = currentSize / 2; i > 0; i-- )
        percolateDown( i );
}

/**
 * Insert item x, allowing duplicates.
 * 最坏的插入时间是上滤到根处,时间为O(logN)
 */
template<typename Comparable>
void BinaryHeap::insert( const Comparable & x )
{
    if( currentSize == array.size( ) - 1 )
        array.resize( array.size( ) * 2 );

     // Percolate up 上滤操作
    int hole = ++currentSize;
    for( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
        array[ hole ] = array[ hole / 2 ];
    array[ hole ] = x;
}

/**
 * Remove the minimum item.
 * Throws UnderflowException if empty.
 */
template<typename Comparable>
void BinaryHeap::deleteMin( )
{
    if( isEmpty( ) )
        throw UnderflowException( );

    array[ 1 ] = array[ currentSize-- ];
    percolateDown( 1 );
}

/**
 * Remove the minimum item and place it in minItem.
 * Throws UnderflowException if empty.
 */
template<typename Comparable>
void BinaryHeap::deleteMin( Comparable & minItem )
{
    if( isEmpty( ) )
        throw UnderflowException( );

    minItem = array[ 1 ];
    array[ 1 ] = array[ currentSize-- ];
    percolateDown( 1 );
}

/**
 * 下滤操作,将临时堆顶值移动到合适位置, 最坏的时间为O(logN)
 * Internal method to percolate down in the heap.
 * hole is the index at which the percolate begins.
 */
void BinaryHeap::percolateDown( int hole )
{
    int child;
    Comparable tmp = array[ hole ];

    for( ; hole * 2 <= currentSize; hole = child )
    {
        child = hole * 2;
        if( child != currentSize && array[ child + 1 ] < array[ child ] )
            child++;
        if( array[ child ] < tmp )
            array[ hole ] = array[ child ];
        else
            break;
    }
    array[ hole ] = tmp;
}