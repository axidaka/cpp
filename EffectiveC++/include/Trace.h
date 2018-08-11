#pragma once

#include <string>
using namespace std;

/*
@brief  调试跟踪
*/
class CTrace
{

public:

	CTrace(const char* name);
	~CTrace();
	void debug(const string &msg);
	static bool sTraceIsActive;
	
private:

     //< 使用对象指针的意图是避免在跟踪处于关闭时
	 //< 增加不必要对象的创建与销毁
	string* theFunctionName;  
};

bool CTrace::sTraceIsActive = false;
