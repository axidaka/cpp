#include "stdafx.h"
#include "WinRWFile.h"
#include "UnicodeConv.h"
#include <string>
#include <assert.h>

CWinRWFile::CWinRWFile()
:m_hLog(INVALID_HANDLE_VALUE)
{
}


CWinRWFile::~CWinRWFile()
{
}

BOOL CWinRWFile::Close()
{
	if (m_hLog !=  INVALID_HANDLE_VALUE)
	{
		FlushFileBuffers(m_hLog);
		CloseHandle(m_hLog);
		m_hLog = INVALID_HANDLE_VALUE;
	}
	return TRUE;
}

BOOL CWinRWFile::OpenW(const wchar_t* szFile, BOOL bAppend, UINT uCodePage)
{
	// 判断输入有效性 按照方式打开文件 设置文件读写位置
	if (szFile == NULL) return FALSE;
	if (m_hLog != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hLog);
		m_hLog = INVALID_HANDLE_VALUE;
	}

	if (bAppend)
	{
		m_hLog = CreateFileW(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		m_hLog = CreateFileW(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if (m_hLog == INVALID_HANDLE_VALUE)return FALSE;
	
	if (bAppend) SetFilePointer(m_hLog, 0, 0, FILE_END);

	m_uCodePage = uCodePage;

	return TRUE;
}

BOOL CWinRWFile::OpenA(const char* szFile, BOOL bAppend, UINT uCodePage)
{
	if (szFile == NULL) return FALSE;

	std::wstring wszFile= ConvertMultiBytesToUnicode(szFile, uCodePage);
	return OpenW(wszFile.c_str(), bAppend, uCodePage);
}

BOOL CWinRWFile::WriteW(const wchar_t* szBuffer, BOOL bFlush)
{
	assert(m_hLog != INVALID_HANDLE_VALUE);
	std::string buf = ConvertUnicodeToMultiBytes(szBuffer, m_uCodePage);
	return WriteA(buf.c_str(), bFlush);
}

BOOL CWinRWFile::WriteA(const char* szBuffer, BOOL bFlush)
{
	assert(m_hLog != INVALID_HANDLE_VALUE);
	DWORD nNumOfByteWritten;
	if (WriteFile(m_hLog, szBuffer, strlen(szBuffer), &nNumOfByteWritten, NULL))
	{
		if (bFlush)
		{
			FlushFileBuffers(m_hLog);
		}

		return TRUE;
	}
	
	return FALSE;
}

