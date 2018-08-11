#include "StdAfx.h"
#include "Logger.h"
#include <Shlwapi.h>
#include "RootDirectory.h"
#include "AutoLock.h"

std::auto_ptr<CLogger> CLogger::s_instance;
CCriticalSectionLock CLogger::s_instance_mutex;

CLogger& CLogger::GetInstance()
{
	if (!s_instance.get())
	{
		CAutoLcok<CCriticalSectionLock> locker(s_instance_mutex);
		if (!s_instance.get())
		{
			s_instance.reset(new CLogger());
		}
	}

	return *s_instance.get();
}

CLogger::CLogger()
	:d_level(Standard), m_bEnble(FALSE)
{		
}

CLogger::~CLogger()
{
	if (m_pMylog)
	{
		m_pMylog->Close();
		delete m_pMylog;
		m_pMylog = NULL;
	}
}

void CLogger::setLogFilename(wstring filename, bool append)
{
	if (!m_pMylog)	return;
	
	wstring LogPath = ToolFunc::Self.GetFullPath(L"LogDir\\");
	if (PathIsDirectory(LogPath.c_str()) != FILE_ATTRIBUTE_DIRECTORY)
	{
		//duowanTool::CreateMultipleDirectory(LogPath);
		m_bEnble = FALSE;
		return;
	}
	
	m_bEnble = TRUE;

	m_LogFilePath = LogPath + filename;
	
	//if (PathFileExists(m_LogFilePath.c_str()))
	//{
	//	DeleteFile(m_LogFilePath.c_str());
	//}
	
	m_pMylog->Open(m_LogFilePath.c_str(), append, TRUE, TRUE);
	m_pMylog->Enable(TRUE);
	m_pMylog->Write(1, L"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
}

void CLogger::logEvent(LoggingLevel level, BOOL EnableIDEPrint, const wchar_t* message)
{
	if (!m_pMylog && m_bEnble) return;

	wstring strbuf;

	switch(level)
	{
	case Errors:
		strbuf = L"(Error)\t";
		break;

	case Warnings:
		strbuf = L"(Warn)\t";
		break;

	case Standard:
		strbuf = L"(Std) \t";
		break;

	default:
		strbuf = L"(Unkwn)\t";
		break;
	}

	strbuf += message;
	m_pMylog->EnableIDEPrint(EnableIDEPrint);
	m_pMylog->Write(1, strbuf.c_str());
}
