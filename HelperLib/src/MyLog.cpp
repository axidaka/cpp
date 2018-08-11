// 
// kage 自用的log类
//
#include "stdafx.h"
#include "MyLog.h"

#include <time.h>
#include <sys/timeb.h>
#include <list>
#include <string>
#include <vector>
#include "UnicodeConv.h"
#include "AutoLock.h"

MyLog::MyLog()
{
	m_bConsole = FALSE;
	m_bIDE = FALSE;
	m_hLog = NULL;
	m_hTextBox = NULL;
	m_bTime = TRUE;
	m_bDate = FALSE;
	m_bEnable = FALSE;
}

MyLog::~MyLog()
{
	Close();
}

BOOL MyLog::Open(const wchar_t *szFile, BOOL bAppend, BOOL bTime, BOOL bFlush, UINT uCodePage)
{
	m_bFlush = bFlush;
	m_bTime = bTime;

	if(szFile == NULL)return TRUE;

	if(m_hLog != NULL)CloseHandle(m_hLog);

	if(bAppend)
		m_hLog = CreateFileW(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	else
		m_hLog = CreateFileW(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_hLog == INVALID_HANDLE_VALUE)return FALSE;

	if(bAppend)SetFilePointer(m_hLog, 0, 0, FILE_END);

	m_uCodePage = uCodePage;

	return TRUE;
}

void MyLog::EnableConsolePrint(BOOL bFlag)
{
	m_bConsole = bFlag;
}

void MyLog::EnableIDEPrint(BOOL bFlag)
{
	m_bIDE = bFlag;
}

void MyLog::EnableFlush(BOOL bFlag)
{
	m_bFlush = bFlag;
}

void MyLog::EnableTime(BOOL bFlag)
{
	m_bTime = bFlag;
}

void MyLog::EnableDate(BOOL bFlag)
{
	m_bDate = bFlag;
}

void MyLog::Enable(BOOL bFlag)
{
	m_bEnable = bFlag;
}

void MyLog::SetWindow(HWND hTextBox, BOOL bCLearOld, BOOL bScrollToBottom)
{
	m_hTextBox = hTextBox;
	m_bCLearOld = bCLearOld;
	m_bScrollToBottom = bScrollToBottom;
}

void MyLog::ClearWindow()
{
	if(m_hTextBox)SetWindowText(m_hTextBox, NULL);
}

void MyLog::Close()
{
	if(m_hLog)
	{
		FlushFileBuffers(m_hLog);
		CloseHandle(m_hLog);
		m_hLog = NULL;
	}
}

void MyLog::Flush()
{
	if(m_hLog != NULL)FlushFileBuffers(m_hLog);
}

// 不要在界面线程中调用，因为会引起死锁
// WTL的CString.FormatV本身调用的是api wvsprintf()
// 而在2000的系统下这个函数不支持 %I64d 等写法

BOOL MyLog::DebugOut(int nType, const wchar_t *fmt, ...)
{
	if(m_bEnable == FALSE)return TRUE;

	if (fmt == NULL)
	{
		if(nType == 12)
		{
			Write(1, L"%s", m_dbg_line);
		}
		m_dbg_line.clear();
		return TRUE;
	}


	std::wstring var_str;

	va_list	ap;
	va_start(ap, fmt);
	int len = _vscwprintf(fmt, ap);
	if (len > 0)
	{
		std::vector<wchar_t> buf(len + 1);
		vswprintf(&buf.front(), buf.size(), fmt, ap);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(ap);

	m_dbg_line += var_str;

	return TRUE;
}

BOOL MyLog::Write(int nType, const wchar_t *fmt, ...)
{
	if (fmt == NULL)return FALSE;
	if(m_bEnable == FALSE)return TRUE;

	wchar_t szTm[1024], szDt[1024];
	std::wstring var_str;

	//if(false)
	{
		__int64 cur_tm;
		_time64(&cur_tm);
		tm *ptm = _localtime64(&cur_tm);

		struct __timeb64 timebuffer;
		_ftime64( &timebuffer );
		swprintf(szTm, 1024, L"%02d:%02d:%02d.%03d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, timebuffer.millitm);
	}

	//_strtime(szTm);
	_wstrdate(szDt);


	va_list	ap;
	va_start(ap, fmt);
	int len = _vscwprintf(fmt, ap);
	if (len > 0)
	{
		std::vector<wchar_t> buf(len + 1);
		vswprintf(&buf.front(), buf.size(), fmt, ap);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(ap);

	CAutoLcok<CCriticalSectionLock> autolock(m_cslock);

	if(m_hLog && m_bEnable == TRUE) 
	{
		DWORD nHave;

		std::wstring text_line;
		if(m_bTime)
		{
			text_line += szTm;

			if(m_bDate)
				text_line += szDt;

			text_line += L"\t";
		}
		text_line += var_str;
		text_line += L"\r\n";

	    std::string bytes = ::ConvertUnicodeToMultiBytes(text_line, m_uCodePage);
		WriteFile(m_hLog, &bytes[0], bytes.size(), &nHave, NULL);

		if(m_bFlush)FlushFileBuffers(m_hLog);
	}

	if(m_bConsole)
	{
		setlocale(LC_ALL, "chs");
		wprintf(L"%s %s\n", szTm, var_str.c_str());
	}

	if(m_bIDE)
	{
		OutputDebugString(var_str.c_str());
		OutputDebugString(L"\n");
	}

	if(m_hTextBox)
	{
		static wchar_t pEditBuf[16384];
		const int nEditBufSize = sizeof(pEditBuf) / sizeof(pEditBuf[0]);
		std::wstring strInfo;
		long nEditBuf;
		long nTxtBuf;

		if(m_bTime)
			strInfo = szTm + var_str;
		else
			strInfo = var_str;

		nTxtBuf = strInfo.size();

		nEditBuf = 0;
		if(m_bCLearOld == FALSE && m_hTextBox)
		{
			nEditBuf = GetWindowText(m_hTextBox, pEditBuf, sizeof(pEditBuf));
		}

		int nOldScrollPos = GetScrollPos(m_hTextBox, SB_VERT);
		
		if (nEditBuf + nTxtBuf + 1 >= nEditBufSize)
		{
			//truncated
			wcscpy(pEditBuf, L"== too long(>16K), auto truncated by UI ==\r\n");
			nEditBuf = wcslen(pEditBuf);
		}
		
		swprintf(pEditBuf + nEditBuf, nEditBufSize - nEditBuf, L"%s\r\n", strInfo.c_str());

		if(m_bScrollToBottom == TRUE)
		{
			if(m_hTextBox)SendMessage(m_hTextBox, WM_SETTEXT, 0, (LPARAM)pEditBuf);
			//SetWindowText(m_hTextBox, pEditBuf);
			PostMessage(m_hTextBox, WM_VSCROLL, SB_BOTTOM, 0);
		}
		else
		{
			if(nOldScrollPos <=0 ) SetWindowText(m_hTextBox, pEditBuf);

			//SetScrollPos(m_hTextBox, SB_VERT, nOldScrollPos, TRUE);
			//SendMessage(m_hTextBox, EM_SCROLL, nOldScrollPos, 0);
		}
	}

	return TRUE;
}
