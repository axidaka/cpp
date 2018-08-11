#ifndef __GAMEPROXY_LOG_H__
#define __GAMEPROXY_LOG_H__

#pragma once

#include "CriticalSectionLock.h"
#include <string>

class MyLog
{
	private:
		HANDLE m_hLog;
		BOOL m_bConsole;
		BOOL m_bIDE;
		BOOL m_bFlush;
		BOOL m_bTime;
		BOOL m_bDate;
		BOOL m_bScrollToBottom;
		BOOL m_bCLearOld;
		BOOL m_bEnable;
		UINT m_uCodePage;

	public:
		BOOL Open(const wchar_t *szFile, BOOL bAppend, BOOL bTime, BOOL bFlush, UINT uCodePage = ::GetACP());
		void Close();
		BOOL Write(int nType, const wchar_t *fmt, ...);
		BOOL DebugOut(int nType, const wchar_t *fmt, ...);
		void Flush();
		void EnableFlush(BOOL);
		void EnableTime(BOOL);
		void EnableDate(BOOL);
		void Enable(BOOL);

		void EnableConsolePrint(BOOL);
		void EnableIDEPrint(BOOL);

		//Ĭ���Զ��������ı��ײ���false�Ļ��ᱣ����ǰ��������λ��
		
		void SetWindow(HWND hTextBox, BOOL bCLearOld, BOOL bScrollToBottom);
		void ClearWindow();

		HWND m_hTextBox;
		std::wstring m_dbg_line;
		CCriticalSectionLock m_cslock;


		~MyLog();
		MyLog();

};

#endif
