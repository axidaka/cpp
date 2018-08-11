#include "stdafx.h"
#include "SysLevelFunc.h"
#include "ToolFunc.h"

namespace SysLevelFunc
{


	DWORD GetProcessHandle(LPTSTR lpProcessName)
	{
		DWORD dwRet = 0;
		//< ��ϵͳ���еĽ����ĸ�����
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapShot)
		{
			tcout << _T("CreateToolhelp32Snapshot ") << ToolFunc::FormatErrStr(GetLastError()) << ENDL;
			return -1;
		}
		else
		{
			PROCESSENTRY32 pe32;   ///< ������ڶ���
			pe32.dwSize = sizeof(PROCESSENTRY32);
			BOOL bMore = Process32First(hSnapShot, &pe32);
			BOOL bTerminate = FALSE;
			while (bMore)
			{
				if (0 == _tcscmp(lpProcessName, pe32.szExeFile))
				{
					dwRet = pe32.th32ProcessID;
					break;
				}

				bMore = Process32Next(hSnapShot, &pe32);
			}

			::CloseHandle(hSnapShot);
		}

		return dwRet;
	}

	void EnumProcessWnds(LPTSTR lpProcessName)
	{
		EnumWindows(EnumWindowsProc, GetProcessHandle(lpProcessName));
	}

	BOOL CALLBACK EnumChildProc(_In_ HWND hwnd, _In_ LPARAM lParam)
	{
		DWORD dwPid = 0;
		DWORD dwThID = GetWindowThreadProcessId(hwnd, &dwPid); // ����ҵ����������Ľ���

		if (dwPid == lParam)
		{
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			char buf[WINDOW_TEXT_LEN] = { 0 };
			GetWindowTextA(hwnd, buf, WINDOW_TEXT_LEN);

			if ((dwStyle & WS_POPUP) && (dwStyle & WS_VISIBLE) && (0 != strcmp(buf, "\0")))
			{
				printf("0x%X", dwStyle);
				printf("����:0x%X ӵ�е������� ", hwnd);

				printf(" ���ڱ���:%s \n", buf);

				PostThreadMessage(dwThID, WM_QUIT, 0, 0);
				EnumChildWindows(hwnd, EnumChildProc, lParam);
			}
		}
		return TRUE;
	}

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		DWORD dwPID = 0;
		DWORD dwThID = GetWindowThreadProcessId(hwnd, &dwPID);


		if (dwPID == lParam)
		{
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
			char buf[WINDOW_TEXT_LEN] = { 0 };
			GetWindowTextA(hwnd, buf, WINDOW_TEXT_LEN);

			if ((dwStyle & WS_POPUP) && (dwStyle & WS_VISIBLE) && (0 != strcmp(buf, "\0")))
			{
				printf("0x%X", dwStyle);
				printf("����:0x%X ӵ�е������� ", hwnd);

				printf(" ���ڱ���:%s \n", buf);

				PostThreadMessage(dwThID, WM_QUIT, 0, 0);
				EnumChildWindows(hwnd, EnumChildProc, lParam);
			}
		}
		return TRUE;
	}

}