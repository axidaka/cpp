#include "Trace.h"

#include <iostream>


inline CTrace::CTrace(const char* name)
:theFunctionName(NULL)
{
	if(sTraceIsActive)
	{
		cout<<"Enter Function"<<name<<endl;
		theFunctionName = new string(name);
	}
}

inline CTrace::~CTrace()
{
	if (sTraceIsActive)
	{
		cout<<"Exit Function"<<theFunctionName<<endl;
		delete theFunctionName;
	}
}

inline void CTrace::debug(const string &msg)
{
	if (sTraceIsActive)
	{
		cout<<msg<<endl;
	}
}