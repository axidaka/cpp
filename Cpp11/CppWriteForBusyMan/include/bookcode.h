#ifndef __BOOK_CODE_H__
#define __BOOK_CODE_H__

#include <iostream>
#include <functional>

using namespace std;
using std::function;

function<int(int, int)> get_fun()
{
	int n = 10;
	// ����n�Ǿֲ�������������[&n]������
	return [n](int a, int b) {return a + b + n; };
}

void Chapter4()
{
	// lambda���� [�հ�] (����) (�����) ���� [�հ�] (����) -> �������� (�����) 
	auto f1 = [](int a, int b) {return a + b; };
	cout << f1(1, 2) << endl;

	auto f2 = [](int a, int b) -> int {return a + b; };
	cout << f2(1, 2) << endl;

	auto f3 = [](int n) {
		for (int i = 0; i < n ; ++i)
		{
			cout << i << ",";
		}

		cout << endl;
	};

	f3(10);

	//�հ��Ǳ�lambda���������񡱵ı����Ŀ�ѡ�б�����lambda������׽�ı�����ָ�ڴ�����Χ������lambda�����������ı���

	int my_id = 5000;
	auto f4 = [my_id](int n) {return my_id + n; };
	cout << f4(10) << endl;

	auto f5 = [&my_id](int n) {my_id += n; return my_id; };
	cout << my_id;
	cout << ":" << f5(10);
	cout << ":" << my_id << endl;

	// [=] ��������Χ�Ĵ��������ľֲ����������Է��ʣ���ֻ�ܰ�ֵ����
	int a = 1, b = 2;
	auto f6 = [=] () {return my_id + a + b; };
	cout << f6() << endl;

	// [&] ��������Χ�Ĵ��������ľֲ����������Է��ʣ������޸���Щֵ
	auto f7 = [&]() {my_id += a; return my_id; };
	cout << f7() << endl;

	int arr[5] = { 1, 2, 3, 4, 5 };
	int sum = 0;
	auto f8 = [&sum](int n) { sum += n;};
	for each (int var in arr)
	{
		f8(var);
	}
	cout << sum << endl;

	// ����lambda���� function<�������ͣ����������б���������������
	auto f9 = get_fun();
	cout << f9(1, 2) << endl;
}

#endif
