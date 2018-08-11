#pragma once

#include <windows.h>
#include <eh.h>

/************************************************************************/
/*brief �ô���C++�쳣��ķ�ʽ����׽����ṹ�����첽���쳣����Ҫ�������� /EHa���뿪��
/*author
/*func  ʹ��C++���ļ����ţ�ʹ�ýṹ���쳣��ǿ��͸�Ч
/************************************************************************/
class CSEHException
{
public:

	CSEHException();
	~CSEHException();

	// ������Ҫ���׳� CSE_Exception�쳣
	static void My_trans_func(unsigned int u, EXCEPTION_POINTERS *pExp);

private:

	// Handles Win32 exceptions (C structured exceptions) as C++ typed exceptions.
	void initialize_seh_trans_to_ce();

private:
	_se_translator_function m_OldFunc;
};

/************************************************************************/
/*brief  ��C++��ķ�ʽ��װ�˽ṹ���쳣��Ϣ
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
