#include "BeautyOfProgram.h"

// 时间复杂度O（N*N*N）,第三重循环中出现了大量不必要的计算
int MaxSubSum1(const vector<int> &vct_array)
{
	int maxSum = 0;
	for (int nBegin = 0; nBegin < vct_array.size(); nBegin ++)
	{
		for (int nEnd = nBegin; nEnd < vct_array.size(); nEnd ++)
		{
			int thisSum = 0;
			for (int nIndex = nBegin; nIndex <= nEnd; nIndex ++)
			{
				thisSum += vct_array[nIndex];
			}

			if (thisSum > maxSum)
			{
				maxSum = thisSum;
			}
		}
	}

	return maxSum;
}

// 时间复杂度O（N*N)）
int MaxSubSum2(const vector<int> &vct_array)
{
	int maxSum = 0;
	for (int i = 0; i < vct_array.size(); i++)
	{
		int thisSum = 0;
		for (int j = i; j < vct_array.size(); j++)
		{
			thisSum += vct_array[j];

			if (thisSum > maxSum)
			{
				maxSum = thisSum;
			}
		}
	}

	return maxSum;
}

int maxSumRec(const vector<int> &vct_array, int left, int right)
{
	if (left == right)
	{
		if (vct_array[left] > 0)
			return vct_array[left];
		else
			return 0;
	}

	int center = (left + right) / 2;
	int maxLeftSum = maxSumRec(vct_array, left, center);
	int maxRightSum = maxSumRec(vct_array, center + 1, right);

	// 求出左边以center为起点的最大值
	int maxLeftBorderSum = 0, leftBorderSum = 0;
	for (int i = center; i >= left; i --)
	{
		leftBorderSum += vct_array[i];
		if (leftBorderSum > maxLeftBorderSum)
			maxLeftBorderSum = leftBorderSum;
	}

	// 求出右边以center + 1 为起点的最大值
	int maxRightBorderSum = 0, rightBorderSum = 0;
	for (int j = center + 1; j <= right; j ++)
	{
		rightBorderSum += vct_array[j];	
		if (rightBorderSum > maxRightBorderSum)
			maxRightBorderSum = rightBorderSum; 
	}

	if (maxLeftSum >= maxRightSum && maxLeftSum >= (maxLeftBorderSum + maxRightBorderSum))
		return maxLeftSum;
	if (maxRightSum > maxLeftSum && maxRightSum >= (maxLeftBorderSum + maxRightBorderSum))
		return maxRightSum;
	return (maxLeftBorderSum + maxRightBorderSum);
}

// 时间复杂度O(NlogN), 采用“分治”策略， 将问题分成两个大致相等的子问题
int MaxSubSum3(const vector<int> &vct_array)
{
	return maxSumRec(vct_array, 0, vct_array.size() - 1);
}

// 时间复杂度O(N)
int MaxSubSum4(const vector<int> &vct_array)
{
	int maxSum = 0, thisSum = 0;
	for (int i = 0; i < vct_array.size(); i ++)
	{
		thisSum += vct_array[i];

		if (thisSum > maxSum)
			maxSum = thisSum;
		else if (thisSum < 0)
			thisSum = 0;
	}

	return maxSum;
}
