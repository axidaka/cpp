#pragma once

/*�º���*/

//���ڽ�operator()�����ˣ�Plus����һ���º���
template<class T>
struct Plus{
	T operator()(const T& x, const T& y)const { return x + y; }
};

template<class T>
struct Minus{
	T operator()(const T&x, const T& y)const { return x - y; }
};