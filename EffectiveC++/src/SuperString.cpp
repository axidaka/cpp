#include "StdAfx.h"
#include "SuperString.h"

// SuperString::SuperString()
// {
// }

SuperString::SuperString( const char* s /*= NULL*/ )
:pstr(NULL)
{
	cout<<"Using SuperString(const char* s) constructor"<<endl;
	if (s != NULL)
	{
		int len = strlen(s) + 1;
		pstr = new char[len];
		strcpy_s(pstr, len, s);
	}
}

inline 
SuperString::SuperString( const SuperString& s )
:pstr(NULL)
{
	cout<<"Using SuperString(const SuperString& s) constructor"<<endl;
	if (s.pstr != NULL)
	{
		int len = strlen(s.pstr) + 1;
		pstr = new char[len];
		strcpy_s(pstr, len, s.pstr);
	}
}


SuperString::~SuperString(void)
{
	cout<<"Using SuperString ~SuperString(void)"<<endl;
	if (pstr != NULL)
	{
		delete[] pstr;
	}
}

SuperString& SuperString::operator=( const SuperString& s )
{
	cout<<"Using SuperString ="<<endl;
	if (pstr != s.pstr)
	{
		delete[]pstr;
		if (s.pstr != NULL)
		{
			int len = strlen(s.pstr) + 1;
			pstr = new char[len];
			strcpy_s(pstr, len, s.pstr);
		}
		pstr = NULL;
	}

	return *this;
}
