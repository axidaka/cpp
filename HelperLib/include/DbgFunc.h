#pragma once

#include <windows.h>
#include <eh.h>

/************************************************************************/
/*brief 用处理C++异常类的方式来捕捉处理结构化（异步）异常，需要工程设置 /EHa编译开关
/*author
/*func  使用C++风格的简单优雅，使用结构化异常的强大和高效
/************************************************************************/
class CSEHException
{
public:

	CSEHException();
	~CSEHException();

	// 函数主要是抛出 CSE_Exception异常
	static void My_trans_func(unsigned int u, EXCEPTION_POINTERS *pExp);

private:

	// Handles Win32 exceptions (C structured exceptions) as C++ typed exceptions.
	void initialize_seh_trans_to_ce();

private:
	_se_translator_function m_OldFunc;
};

/************************************************************************/
/*brief  用C++类的方式封装了结构化异常信息
/*author
/*func
/************************************************************************/
class CSE_Exception
{
public:
	CSE_Exception(unsigned int iSe, EXCEPTION_POINTERS *pExp);
	CSE_Exception(const  CSE_Exception& Ex);
	~CSE_Exception();

	void Report(const wchar_t* pszMsgText)const;
	
private:
	CSE_Exception();

	unsigned int        m_iSE;
	EXCEPTION_POINTERS* m_pExp;
};
