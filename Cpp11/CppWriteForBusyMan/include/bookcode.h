#ifndef __BOOK_CODE_H__
#define __BOOK_CODE_H__

#include <iostream>
#include <functional>

using namespace std;
using std::function;

function<int(int, int)> get_fun()
{
	int n = 10;
	// 这是n是局部变量，按引用[&n]不可行
	return [n](int a, int b) {return a + b + n; };
}

void Chapter4()
{
	// lambda函数 [闭包] (参数) (语句组) 或者 [闭包] (参数) -> 返回类型 (语句组) 
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

	//闭包是被lambda函数“捕获”的变量的可选列表，允许lambda函数捕捉的变量是指在代码周围，但在lambda本身外声明的变量

	int my_id = 5000;
	auto f4 = [my_id](int n) {return my_id + n; };
	cout << f4(10) << endl;

	auto f5 = [&my_id](int n) {my_id += n; return my_id; };
	cout << my_id;
	cout << ":" << f5(10);
	cout << ":" << my_id << endl;

	// [=] 所有在周围的代码声明的局部变量都可以访问，但只能按值访问
	int a = 1, b = 2;
	auto f6 = [=] () {return my_id + a + b; };
	cout << f6() << endl;

	// [&] 所有在周围的代码声明的局部变量都可以访问，且能修改这些值
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

	// 返回lambda函数 function<返回类型（函数参数列表）函数名（参数）
	auto f9 = get_fun();
	cout << f9(1, 2) << endl;
}

#endif
