#include "stdafx.h"
#include "DbgFunc.h"
#include <tchar.h>

CSEHException::CSEHException()
{
	m_OldFunc = NULL;
	initialize_seh_trans_to_ce();
}

CSEHException::~CSEHException()
{
	if (m_OldFunc)
	{
		_set_se_translator(m_OldFunc);
	}
}

void CSEHException::My_trans_func(unsigned int u, EXCEPTION_POINTERS *pExp)
{
	CSE_Exception ex(u, pExp);

	throw ex;
}

void CSEHException::initialize_seh_trans_to_ce()
{
	m_OldFunc = _set_se_translator(My_trans_func);
}


/*****************************************************************************************/


CSE_Exception::CSE_Exception()
:m_pExp(NULL), m_iSE(0)
{
}

CSE_Exception::CSE_Exception(unsigned int iSe, EXCEPTION_POINTERS *pExp)
: m_pExp(pExp), m_iSE(iSe)
{
}

CSE_Exception::CSE_Exception(const CSE_Exception& Ex)
{
	m_iSE = Ex.m_iSE;
	m_pExp = Ex.m_pExp;
}

CSE_Exception::~CSE_Exception()
{
}

void CSE_Exception::Report(const wchar_t* pszMsgText) const
{
	_tprintf(L"SEH Exception Report:%s\n", pszMsgText);
}
