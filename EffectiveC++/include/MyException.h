#pragma once

#include <exception>

class CMyException :
	public std::exception
{
public:
	CMyException();
	~CMyException();

	 virtual const char * what() const;
};

#include <stdexcept>

// 异常类的继承、嵌套异常类声明
class CSales
{
public:

	enum {MONTHS = 12};

	class bad_index : public std::logic_error
	{
	private:
		int bi;     // bad index value

	public:

		explicit bad_index(int index, const std::string &s = "Index error in Sales Object\n");
		virtual~bad_index()throw(){}
		int ib_val()const{ return bi; }

	};

	explicit CSales(int yy = 0);
	CSales(int yy, const double* gr, int n);
	virtual ~CSales(){}

	int Year()const{ return year; }
	virtual double operator[](int index)const;

private:

	double gross[MONTHS];
	int year;
};