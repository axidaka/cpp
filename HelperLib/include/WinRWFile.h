#pragma once

/************************************************************************/
/*brief
/*author
/*func
/************************************************************************/

class CWinRWFile
{
public:
	CWinRWFile();
	~CWinRWFile();

	BOOL OpenW(const wchar_t* szFile, BOOL bAppend, UINT uCodePage);

	BOOL OpenA(const char* szFile, BOOL bAppend, UINT uCodePage);

	BOOL WriteW(const wchar_t* szBuffer, BOOL bFlush);

	BOOL WriteA(const char* szBuffer, BOOL bFlush);

#ifdef _UNICODE
	#define  Open  OpenW
	#define  Write WriteW
#else
	#define  Open  OpenA
	#define  Write WriteA
#endif

	BOOL Close();

private:

	HANDLE m_hLog;
	UINT m_uCodePage;
};

