#pragma once
#include "MyLog.h"
#include "UnicodeConv.h"
#include <memory>
#include <string>
using namespace std;

enum LoggingLevel
{
	Errors,			//!< Only actual error conditions will be logged.
	Warnings,       //!< Warnings will be logged as well.
	Standard,		//!< Basic events will be logged (default level).
};


class CLogger
{
private:
	static std::auto_ptr<CLogger> s_instance;
	static CCriticalSectionLock s_instance_mutex;
public:
	static CLogger& GetInstance();

protected:
	explicit CLogger();
	CLogger(const CLogger&);
	CLogger& operator =(const CLogger&);
public:
	~CLogger(void);

	MyLog *SetLogger(MyLog *pLogger)
	{
		MyLog *pRet = m_pMylog;
		m_pMylog = pLogger;
		return pRet;
	}

	MyLog * GetLogger()
	{
		return m_pMylog;
	}

	void setLoggingLevel(LoggingLevel level)
	{
		d_level = level;
	}

	LoggingLevel getLoggingLevel(void) const
	{
		return d_level;
	}
	
	void setLogFilename(wstring filename, bool append = false);

	void logEvent(LoggingLevel level, BOOL EnableIDEPrint, const wchar_t* message);
	
	void logEventFormat(LoggingLevel level, BOOL EnableIDEPrint, const wchar_t* pszFormat,...)
	{
		if (!m_pMylog && m_bEnble) return;
		
		wchar_t szBuf[1024+1];
		va_list args;
		va_start( args, pszFormat);
		vswprintf_s(szBuf,1024,pszFormat,args);
		logEvent(level, EnableIDEPrint, szBuf);
	}

	void logEventFormat(LoggingLevel level, BOOL EnableIDEPrint, const char* pszFormat, ...)
	{
		if (!m_pMylog && m_bEnble) return;

		char szBuf[1024+1];
		va_list args;
		va_start( args, pszFormat);
		vsprintf_s(szBuf,1024,pszFormat,args);
		logEvent(level, EnableIDEPrint, ConvertMultiBytesToUnicode(szBuf).c_str());
	}

private:

	MyLog* m_pMylog;
	wstring m_LogFilePath;
	LoggingLevel	d_level;		//!< Holds current logging level
	BOOL  m_bEnble;
};
