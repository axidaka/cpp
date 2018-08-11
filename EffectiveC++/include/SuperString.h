#pragma once
#include <iostream>
using namespace std;

class SuperString
{
public:
	//SuperString();
	SuperString(const char* s = NULL);
	SuperString(const SuperString& s);
	SuperString& operator = (const SuperString& s);
	~SuperString(void);

private:

	char* pstr;
};

