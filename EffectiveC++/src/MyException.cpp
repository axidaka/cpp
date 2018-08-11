#include "stdafx.h"
#include "MyException.h"


CMyException::CMyException()
{
}


CMyException::~CMyException()
{
}

const char * CMyException::what() const
{
	return "MyException Test";
}

CSales::bad_index::bad_index(int index, const std::string &s /*= "Index error in Sales Object\n"*/)
:logic_error(s), bi(index)
{
}

CSales::CSales(int yy /*= 0*/)
: year(yy)
{
	for (int i = 0; i < MONTHS; i++)
	{
		gross[i] = 0;
	}
}

CSales::CSales(int yy, const double* gr, int n)
:year(yy)
{
	int lim = n < MONTHS ? n : MONTHS;

	int i = 0;
	for (; i < lim; i ++)
	{
		gross[i] = gr[i];
	}

	for (; i < MONTHS; i++)
	{
		gross[i] = 0;
	}
}

double CSales::operator[](int index) const
{
	if (index < 0 || index >= MONTHS)
		throw bad_index(index);

	return gross[index];
}
