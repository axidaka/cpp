#ifndef SORTHELPER_H
#define SORTHELPER_H

// 内部排序

http://blog.csdn.net/hguisu/article/details/7776068/

/*
排序算法总结:
1.当要排序的Comparable对象很大, 快速排序和希尔排序有时效率很低, 主要在于进行太多的复制Comparable工作.
2.插入法排序算法主要包括:直接插入排序 折半插入排序 希尔排序
3.选择法排序算法主要包括:简单选择排序 堆排序
4.交换法排序算法主要包括:冒泡排序 快速排序
5.归并排序:
6.基数排序:
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 插入法排序:每次将一个待排序的记录,按其关键字大小插入到前面已经排好序的子表中的适当位置,直到全部记录插入完成为止.

/**
*直接插入排序 时间复杂度O(N*N) 稳定算法
*/
template<typename Comparable>
void insertionSort(vector<Comparable> &a)
{
	int j;
	for (int p = 1; p < a.size(); p++)
	{
		Comparable tmp = a[p];
		for (j = p; j >= 1 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];

		a[j] = tmp;
	}
}

/**
*折半插入排序: 通过折半查找的方法,在有序表中查找插入位置, 与直接插入排序相比，仅仅减少了关键字间的比较次数，
			   但记录的移动次数不变。复杂度仍未O（N*N) 稳定算法
*/
template<typename Comparable>
void binaryInsertSort(vector<Comparable> &a)
{
	for (int i = 1; i < a.size(); i ++)
	{
		Comparable tmp = a[i];
		int low = 0, high = i - 1;

		while(low <= high)
		{
			int mid = (low + high) / 2;
			if (a[mid] > tmp)
			{
				// 插入位置在mid左边
				high = mid - 1;
			}
			else
			{
				// 插入位置在mid右边
				low = mid + 1;
			}
		}

		// 插入位置是在high + 1, 从i-1位置开始到high +1, 需要整体后移
		for (int j = i - 1; j >= (high + 1) ;j--)
			a[j + 1] = a[j];

		a[high + 1] = tmp;
	}
}

/**
*希尔排序(ShellSort) 使用一个增量序列序列ht,....h2,h1.只要h1=1,任何增量序列都是可行的。
*在使用增量hk的一趟排序之后， 对于每一个i我们有a[i] <= a[i + hk],此时称文件是hk排序的
希尔排序会使关键字相同的记录交换相对位置， 是不稳定排序
*/
template<typename Comparable>
void shellSort(vector<Comparable> &a)
{
	for (int gap = a.size() / 2; gap > 0; gap /=2)
	{
		for (int i = gap; i < a.size(); i ++)
		{
			Comparable tmp = a[i];
			int j = i;
			for (; j >= gap && tmp < a[j - gap]; j -= gap)
				a[j] = a[j - gap];
			a[j] = tmp;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 交换排序:利用交换记录的位置 进行排序的方法， 基本思想是两两比较排序记录的关键，如果是逆序就交换，直到所有所有记录都排好序.

/**
*冒泡排序:
*时间复杂度O(N*N) 稳定排序
*/
template<typename Comparable>
void bubbleSort(vector<Comparable>& a)
{
	for (int i = 1; i < a.size(); i ++)
	{
		bool flag = 1;  // 每趟比较前设置flag = 1, 假定该序列已有序
		for (int j = 0; j < a.size() - i; j++)
		{
			if (a[j] > a[j + 1])
			{
				flag = 0;
				Comparable tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
		if (flag)return;
	}
}


/**
*快速排序(QuickSort), 先选取某个元素v作为总枢元素,通过处理,其左边元素都是小于v,右边元素都是大于v.
*再通过递归方式处理左右子序列
*时间复杂度O(NlogN) 不稳定排序
*/

// 将left center right 处理为 以下大小关系 left < center < right
// a[center]作为中枢值,并与a[right - 1]互换
template<typename Comparable>
const Comparable& median3(vector<Comparable>&a, int left, int right)
{
	int center = (left + right) / 2;
	Comparable tmp;
	if (a[center] < a[left])
	{
		// 确保 a[left] < a[center]
		tmp = a[left];
		a[left] = a[center];
		a[center] = tmp;
	}
	if (a[right] < a[left])
	{
		// 确保 a[left] 是三者中最小
		tmp = a[left];
		a[left] = a[right];
		a[right] = tmp;
	}
	if (a[center] > a[right])
	{
		// 确保a[right] 最大
		tmp = a[center];
		a[center] = a[right];
		a[right] = tmp;
	}

	// 交换a[center] a[right -1]
	tmp = a[center];
	a[center] = a[right - 1];
	a[right - 1] = tmp;


	return a[right - 1];
}

template<typename Comparable>
void quicksort(vector<Comparable> &a, int left, int right)
{
	if (left + 10 <= right)
	{
		// pivot 总枢位置是right - 1
		Comparable pivot = median3(a, left, right);
		int i = left, j = right - 1;
		Comparable tmp;
		for (;;)
		{
			while (a[++i] < pivot){}
			while (a[--j] > pivot){}

			if (i < j)
			{
				tmp = a[i];
				a[i] = a[j];
				a[j] = tmp;
			}
			else
				break;
		}

		// 执行完交换后, 中枢值是在i
		tmp = a[i];
		a[i] = a[right - 1];
		a[right - 1] = tmp;

		quicksort(a, left, i - 1);
		quicksort(a, i + 1, right);

	}
	else
	{
		// 小数组, 快速排序不如插入排序
		//insertionSort
		int j;
		for (int p = left; p <= right; p++)
		{
			Comparable tmp = a[p];
			for (j = p; j >= 1 && tmp < a[j - 1]; j--)
				a[j] = a[j - 1];

			a[j] = tmp;
		}
	}
}

template<typename Comparable>
void quicksort(vector<Comparable> &a)
{
	quicksort(a, 0, a.size() - 1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 选择排序: 不断从待排记录序列中选出关键字最小的记录插入已排序记录序列的后面,直到n个记录全部插入已排序记录序列中

/**
*简单选择排序:设所排序序列的记录个数为n. i取1,2,…,n-1,从所有n-i+1个记录（Ri,Ri+1,…,Rn）中找出排序码最小的记录，
			  与第i个记录交换。执行n-1趟 后就完成了记录序列的排序。
*时间复杂度: O(N*N)
*/
template<typename Comparable>
void selectSort(vector<Comparable> &a)
{
	// 进行N-1趟选择
	for (int i = 0; i < a.size() - 1; i++)
	{
		int k = i;   // 假定每趟的起始位置为最小记录
		for (int j = i + 1; j < a.size(); j++)
		{
			if (a[j] < a[k])
				k = j;
		}

		if (k != i)
		{// 如果k不是假定位置,则交换
			Comparable tmp = a[k];
			a[k] = a[i];
			a[i] = tmp;
		}
	}
}

/**
*堆排序(heapSort), 时间复杂度O(NlogN)
*/
inline int leftChild(int i)
{
	return 2 * i + 1;
}

template<typename Comparable>
void heapAdjust(vector<Comparable> &a, int i, int n)
{
	int child;
	Comparable tmp;

	for (tmp = a[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		// 获取最大的子节点
		if (child != n - 1 && a[child] < a[child + 1])
			child++;

		//如果较大的子结点大于父结点那么把较大的子结点往上移动，替换它的父结点
		if (tmp < a[child])
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}

template<typename Comparable>
void heapSort(vector<Comparable> &a)
{
	// 先将无序建成堆
	for (int i = a.size()/2; i >= 0; i--)
		heapAdjust(a, i, a.size());

	for (int j = a.size() - 1; j > 0; j--)
	{
		//把第一个元素和当前的最后一个元素交换，
		//保证当前的最后一个位置的元素都是在现在的这个序列之中最大的
		Comparable tmp = a[0];
		a[0] = a[j];
		a[j] = tmp;

		heapAdjust(a, 0, j);
	}
}

/**
*归并排序(MergeSort), 采用分治策略, 用递归来实现将问题划分称子序列排序, 最后将排序好的子序列合并.
*时间复杂度O(NlogN)
*/

// 将左右两子序列合并,左子序列(leftPos, rightPos-1),右子序列(rightpos, rightEnd)
template<typename Comparable>
void merge(vector<Comparable> &a, vector<Comparable> &tmpArray, int leftPos, int rightPos, int rightEnd)
{
	int leftEnd = rightPos - 1;
	int tmpPos = leftPos;
	int nElements = rightEnd - leftPos + 1;
	while (leftPos <= leftEnd && rightPos <= rightEnd)
	{
		// 将每次比较中小的元素添加tmpArray
		if (a[leftPos] <= a[rightPos])
			tmpArray[tmpPos++] = a[leftPos++];
		else
			tmpArray[tmpPos++] = a[rightPos++];
	}

	// 如果左子序列有剩余,将余下的拷贝到tmpArray
	while (leftPos <= leftEnd)
	{
		tmpArray[tmpPos++] = a[leftPos++];
	}

	// 同理右子序列
	while (rightPos <= rightEnd)
	{
		tmpArray[tmpPos++] = a[rightPos++];
	}

	for (int i = 0; i < nElements; i ++ , rightEnd --)
	{
		a[rightEnd] = tmpArray[rightEnd];
	}
}

// 递归算法, 将问题划分成左右两个子问题
template<typename Comparable>
void mergeSort(vector<Comparable> &a, vector<Comparable> &tmpArray, int left, int right)
{
	if (left < right)
	{
		int center = (right + left) / 2;
		mergeSort(a, tmpArray, left, center);
		mergeSort(a, tmpArray, center + 1, right);
		merge(a, tmpArray, left, center + 1, right);
	}
}

template<typename Comparable>
void mergeSort(vector<Comparable> &a)
{
	vector<Comparable> tmpArray(a.size());
	mergeSort(a, tmpArray, 0, a.size() - 1);
}

/**
*间接排序 当Comparable对象比较大时,复制或者赋值消耗大, 使用指针数据
*/

// 为了让快速排序算法中支持指针的比较
template<typename Comparable>
class Pointer
{
public:
	Pointer(Comparable *rhs = NULL) :pointee(rhs){}

	//重载<操作符，为保证能正确对Pointer采用快排，  
	//必须将快排中的Type元素比较符号均重写为<  
	bool operator < (const Pointer& rhs) const
	{
		return *pointee < *rhs.pointee;
	}

	//类型转换函数，从Pointer<Type>转换到Type*  
	operator Comparable* ()const
	{
		return pointee;
	}

private:
	Comparable *pointee;
};

template<typename Comparable>
void largeObjectSort(vector<Comparable> &a)
{
	vector<Pointer<Comparable>> p(a.size());

	for (int i = 0; i < a.size(); i ++)
	{
		p[i] = &a[i];
	}

	quicksort(p);

	int i, j, nextj;
	for (i = 0; i < a.size(); i++)
		if (p[i] != &a[i])
		{
			Comparable tmp = a[i];
			for (j = i; p[j] != &a[i]; j = nextj)
			{
				nextj = p[j] - &a[0];
				a[j] = *p[j];//移动中间元素  
				p[j] = &a[j];//复位指针  
			}
			a[j] = tmp;//移动最后元素  
			p[j] = &a[j];//复位指针  
		}

}
#endif // !SORTHELPER_H00