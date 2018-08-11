#include "stdafx.h"
#include "ProcessFunc.h"
#include "StringHelper.h"
#include <tlhelp32.h>

namespace processfunc
{


	std::wstring GetFileByProcessId(DWORD processId)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);

		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return L"";
		}

		MODULEENTRY32W entry = { 0 };
		entry.dwSize = sizeof(MODULEENTRY32W);

		if (Module32FirstW(hSnapshot, &entry))
		{
			CloseHandle(hSnapshot);
			return entry.szExePath;
		}

		CloseHandle(hSnapshot);
		return L"";
	}

	BOOL CloseProcessByFile(const std::wstring &filepath)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return FALSE;
		}

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &entry))
		{
			while (Process32Next(hSnapshot, &entry))
			{
				if (StringHelper::CompareIgnore(StringHelper::StrW2A(filepath),
					StringHelper::StrW2A(GetFileByProcessId(entry.th32ProcessID)) ) == 0 )
				{
					return TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID), 0);
				}
			}
		}

		CloseHandle(hSnapshot);
		return FALSE;
	}

	BOOL IsProcessExist(const std::wstring &name)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return FALSE;
		}

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &entry))
		{
			while (Process32Next(hSnapshot, &entry))
			{
				if (StringHelper::CompareIgnore(StringHelper::StrW2A(name),
					StringHelper::StrW2A(entry.szExeFile)) == 0)
				{
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	void CloseProcess(const std::wstring &name)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
		{
			return;
		}

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &entry))
		{
			while (Process32Next(hSnapshot, &entry))
			{
				if (StringHelper::CompareIgnore(StringHelper::StrW2A(name),
					StringHelper::StrW2A(entry.szExeFile)) == 0)
				{
					TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID), 0);
				}
			}
		}

		CloseHandle(hSnapshot);
	}

	BOOL CreateProcess(const std::wstring &strCmd, const std::wstring &strDir)
	{
		STARTUPINFO si;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));

		BOOL bRet = FALSE;

		if (strDir.empty()) {
			bRet = ::CreateProcessW(NULL, const_cast<LPWSTR>(strCmd.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		}
		else {
			bRet = ::CreateProcessW(NULL, const_cast<LPWSTR>(strCmd.c_str()), NULL, NULL, FALSE, 0, NULL, strDir.c_str(), &si, &pi);
		}

		DWORD dwErr = GetLastError();

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return bRet;
	}

	
}
