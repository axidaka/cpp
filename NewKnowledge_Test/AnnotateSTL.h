#pragma once

/*仿函数*/

//由于将operator()重载了，Plus成了一个仿函数
template<class T>
struct Plus{
	T operator()(const T& x, const T& y)const { return x + y; }
};

template<class T>
struct Minus{
	T operator()(const T&x, const T& y)const { return x - y; }
};