#ifndef BEAUTYOFPROGRAM_H
#define BEAUTYOFPROGRAM_H

#include <vector>
using namespace std;

// 编程之美 罗列的问题
	// 最大子序列和
int MaxSubSum1(const vector<int> &vct_array);  // 时间复杂度O（N*N*N）
int MaxSubSum2(const vector<int> &vct_array);  // 时间复杂度O（N*N)）
int MaxSubSum3(const vector<int> &vct_array);  // 时间复杂度O(NlogN)
int MaxSubSum4(const vector<int> &vct_array);  // 时间复杂度O(N)

#endif //!BEAUTYOFPROGRAM_H