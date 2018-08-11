#pragma once

#include <stldefine.h>
#include <time.h>
#include <SYS\TIMEB.H>
#include "UnicodeConv.h"
#include "Singleton.h"

#ifdef _UNICODE
#define Open     OpenW
#define Write    WriteW
#else
#define Open	 OpenA
#define Write    WriteA
#endif

/************************************************************************/
/*brief  写日志文件类
/*author 大松哥
/*func   1.提供写日志文件功能 2.控制台、IDE调试信息输出
/************************************************************************/

template<class CRWFile>
class CLog
{
public:

	enum LOG_LEVEL
	{
		LL_ERROR = 0,
		LL_WARNNING,
		LL_INFORMATION,
	};


	CLog()
		: m_LogLevel(LL_INFORMATION)
		, m_bConsole(FALSE), m_bIDE(FALSE), m_bFlush(TRUE), m_bTime(TRUE)
		, m_bEnable(FALSE), m_bShowLevle(FALSE)
	{}

	~CLog(){}

	BOOL OpenA(const char* filePath, LOG_LEVEL level = LL_INFORMATION, BOOL bAppend = FALSE, UINT uCodePage = ::GetACP());
	BOOL OpenW(const wchar_t* filePath, LOG_LEVEL level = LL_INFORMATION, BOOL bAppend = FALSE, UINT uCodePage = ::GetACP());

	BOOL Close();

	BOOL WriteW(const wchar_t* format, ...);
	BOOL WriteA(const char* format, ...);

	void SetLogLevel(LOG_LEVEL level){ m_LogLevel = level; }

	void EnableConsolePrint(BOOL bFlag){ m_bConsole = bFlag; }
	void EnableIDEPrint(BOOL bFlag){ m_bIDE = bFlag; }
	void EnableFlush(BOOL bFlag){ m_bFlush = bFlag; }
	void EnableTime(BOOL bFlag){ m_bTime = bFlag; }
	void Enable(BOOL bFlag){ m_bEnable = bFlag; }
	void EnableShowLevel(BOOL bFlag){ m_bShowLevle = bFlag }

	tstring GetLastErr(){ return m_strLastErr; }

private:

	CRWFile m_rwFile;

	LOG_LEVEL m_LogLevel;

	BOOL m_bConsole;
	BOOL m_bIDE;
	BOOL m_bFlush;
	BOOL m_bTime;
	BOOL m_bEnable;
	BOOL m_bShowLevle;

	UINT m_uCodePage;

	tstring  m_strLastErr;

};


template<class CRWFile>
BOOL CLog<CRWFile>::OpenA(const char* filePath, LOG_LEVEL level /*= LL_INFORMATION*/, BOOL bAppend /*= FALSE*/, UINT uCodePage /*= ::GetACP()*/)
{
	if (filePath == NULL) return FALSE;

	m_LogLevel = level;
	m_uCodePage = uCodePage;

	return m_rwFile.OpenA(filePath, bAppend, uCodePage);

}

template<class CRWFile>
BOOL CLog<CRWFile>::OpenW(const wchar_t* filePath, LOG_LEVEL level /*= LL_INFORMATION*/, BOOL bAppend /*= FALSE*/, UINT uCodePage /*= ::GetACP()*/)
{
	if (filePath == NULL) return FALSE;

	m_LogLevel = level;
	m_uCodePage = uCodePage;

	return m_rwFile.OpenW(filePath, bAppend, uCodePage);
}

template<class CRWFile>
BOOL CLog<CRWFile>::Close()
{
	m_rwFile.Close();

	return TRUE;
}

template<class RWFile>
BOOL CLog<RWFile>::WriteW(const wchar_t* format, ...)
{
	if (format == NULL) return FALSE;
	if (!m_bEnable) return FALSE;

	std::wstring var_str;
	// format input to string
	va_list ap;
	va_start(ap, format);

	int len = _vscwprintf(format, ap);
	if (len > 0)
	{
		std::vector<wchar_t> buf(len + 1);
		vswprintf_s(&buf.front(), len + 1, format, ap);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(ap);

	// format time string
	wchar_t szTm[1024], szDt[1024];
	__int64 cur_tm;
	_time64(&cur_tm);
	tm *ptm = _localtime64(&cur_tm);

	struct __timeb64 timebuffer;
	_ftime64(&timebuffer);
	swprintf(szTm, 1024, L"%02d:%02d:%02d.%03d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, timebuffer.millitm);

	_wstrdate(szDt);

	if (m_bConsole)
	{
		setlocale(LC_ALL, "chs");
		wprintf(L"%s %s %s\n", szDt, szTm, var_str.c_str());
	}

	if (m_bIDE)
	{
		OutputDebugStringW(var_str.c_str());
		OutputDebugStringW(L"\n");
	}

	std::wstring wline;
	if (m_bTime)
	{
		wline += szDt;
		wline += szTm;
		wline += L"\t";
	}

	wline += var_str;
	wline += L"\r\n";

	std::string bytes = ConvertUnicodeToMultiBytes(wline, m_uCodePage);

	m_rwFile.WriteA(bytes.c_str(), m_bFlush);

	return TRUE;
}

template<class RWFile>
BOOL CLog<RWFile>::WriteA(const char* format, ...)
{
	if (format == NULL) return FALSE;
	if (!m_bEnable) return FALSE;

	std::string var_str;
	// format input to string
	va_list ap;
	va_start(ap, format);

	int len = _vscprintf(format, ap);
	if (len > 0)
	{
		std::vector<char> buf(len + 1);
		vsprintf_s(&buf.front(), len + 1, format, ap);
		var_str.assign(buf.begin(), buf.end() - 1);
	}
	va_end(ap);

	// format time string
	char szTm[1024], szDt[1024];
	__int64 cur_tm;
	_time64(&cur_tm);
	tm *ptm = _localtime64(&cur_tm);

	struct __timeb64 timebuffer;
	_ftime64(&timebuffer);
	sprintf(szTm, "%02d:%02d:%02d.%03d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec, timebuffer.millitm);

	_strdate(szDt);

	if (m_bConsole)
	{
		printf("%s %s %s\n", szDt, szTm, var_str.c_str());
	}

	if (m_bIDE)
	{
		OutputDebugStringA(var_str.c_str());
		OutputDebugStringA("\n");
	}

	std::string wline;
	if (m_bTime)
	{
		wline += szDt;
		wline += szTm;
		wline += "\t";
	}

	wline += var_str;
	wline += "\r\n";

	m_rwFile.WriteA(wline.c_str(), m_bFlush);

	return TRUE;
}

