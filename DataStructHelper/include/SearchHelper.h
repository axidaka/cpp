#pragma once

// 查找算法

/*
二分查找
*/

// 递归版本
template<typename Comparable>
int binarySearchWithRecursion(vector<Comparable>& a, int start, int end, Comparable key)
{
    if (start > end)return -1;
    int mid = (start + end )/2;
    if (a[mid] == key)
        return mid;
    else if (key < a[mid])
        return binarySearchWithRecursion(a, start, mid -1, key);
    else 
        return binarySearchWithRecursion(a, mid +1, end, key);
}

// 非递归版本
int binarySearch(vector<Comparable>& a, Comparable key)
{
    int start = 0, end = a.size() -1;

    while (start <= end)
    {
        int mid = (start + end)/2;
        if (a[mid] == key)
            return mid;
        else if (key < a[mid])
            end = mid -1;
        else 
            start = mid + 1;
    } 

    return -1;   
}