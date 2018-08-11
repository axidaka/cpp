#include "stdafx.h"
#include "DllInjectFunc.h"

#include <TlHelp32.h>
#include "ToolFunc.h"
#include "detours.h"
#include "UnicodeConv.h"
#include "RootDirectory.h"

#pragma pack (push , 1)  

typedef struct   INJECT_CODE
{
	BYTE      int_PUSHAD;         // pushad        0x60         
	BYTE		int_PUSHFD;			//
	BYTE      int_PUSH;             // push &szDLL     0x68  
	DWORD push_Value;           //            &szDLL = ""的path  
	BYTE      int_MOVEAX;              //  move eax &LoadLibrary  0xB8  
	DWORD eax_Value;             //     &LoadLibrary  
	WORD    call_eax;         //     call eax    0xD0FF(FF D0) (LoadLibrary("");  
	BYTE	PopFd;				//
	BYTE	PopAd;				//	
	BYTE      jmp;             //     move eax &ReplaceOldCode  0xB8         
	DWORD jmp_Value;             //     JMP的参数  
	char szDLL[MAX_PATH]; //  ""的FullPath  
}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;
#pragma pack (pop , 1)  

//<SE_PRIVILEGE_ENABLED表示该特权可用于调试及更改其它进程的内存
BOOL EnablePrivilege()
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return FALSE;
	}
	LPTSTR szPrivName = SE_DEBUG_NAME;
	TOKEN_PRIVILEGES tkp;
	LookupPrivilegeValue(NULL, szPrivName, &tkp.Privileges[0].Luid); //修改进程权限
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL); //通知系统修改进程权限
	return((GetLastError() == ERROR_SUCCESS));
}


/************************************************************************/
/*使用远程线程来插入DLL
/************************************************************************/
bool InjectDll(DWORD dwProcessID, const char* dllPath)
{
	if ((0 == dwProcessID) || (0 == strcmp(dllPath, "\0")))
	{
		return false;
	}

	if (!EnablePrivilege())
	{
		return false;
	}

	// 打开要注入的进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL)
	{
		_tprintf(_T("Open Remote Process Failed(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return false;
	}

	// 在指定进程空间中申请虚拟内存空间，为了存放DLL名称
	DWORD dwSize = strlen(dllPath) + 1;
	LPVOID buffer;
	buffer = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if (buffer == NULL)
	{
		// 关闭句柄
		_tprintf(_T("VirtualAllocEx Failed(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		CloseHandle(hProcess);
		return false;
	}

	// 将DLL名称写入到指定进程的刚申请的虚拟内存空间
	BOOL bRet = WriteProcessMemory(hProcess, buffer, (LPVOID)dllPath, dwSize, NULL);
	if (!bRet)
	{
		// 关闭句柄和释放内存
		_tprintf(_T("WriteProcessMemory Failed(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		VirtualFreeEx(hProcess, buffer, dwSize, MEM_COMMIT);
		CloseHandle(hProcess);
		return false;
	}

	// 动态装载指定的DLL文件

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)::GetProcAddress(::GetModuleHandleA("Kernel32"), "LoadLibraryA"), buffer, 0, 0);
	if (hThread == NULL) // 创建加载DLL的远程线程失败
	{
		// 关闭句柄和释放内存
		_tprintf(_T("CreateRemoteThread Failed(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		VirtualFreeEx(hProcess, buffer, dwSize, MEM_COMMIT);
		CloseHandle(hProcess);
		return false;
	}

	// 等待远程线程执行完毕
	WaitForSingleObject(hThread, INFINITE);

	// 释放资源
	VirtualFreeEx(hProcess, buffer, dwSize, MEM_COMMIT);
	CloseHandle(hThread);
	CloseHandle(hProcess);

	return true;
}

/************************************************************************/
/*使用detours插入DLL
/************************************************************************/


DETOUR_TRAMPOLINE(BOOL WINAPI RealCreateProcessA(
	__in_opt    LPCSTR lpApplicationName,
	__inout_opt LPSTR lpCommandLine,
	__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	__in        BOOL bInheritHandles,
	__in        DWORD dwCreationFlags,
	__in_opt    LPVOID lpEnvironment,
	__in_opt    LPCSTR lpCurrentDirectory,
	__in        LPSTARTUPINFOA lpStartupInfo,
	__out       LPPROCESS_INFORMATION lpProcessInformation
	), CreateProcessA);

BOOL WINAPI MyCreateProcessA(
	__in_opt    LPCSTR lpApplicationName,
	__inout_opt LPSTR lpCommandLine,
	__in_opt    LPSECURITY_ATTRIBUTES lpProcessAttributes,
	__in_opt    LPSECURITY_ATTRIBUTES lpThreadAttributes,
	__in        BOOL bInheritHandles,
	__in        DWORD dwCreationFlags,
	__in_opt    LPVOID lpEnvironment,
	__in_opt    LPCSTR lpCurrentDirectory,
	__in        LPSTARTUPINFOA lpStartupInfo,
	__out       LPPROCESS_INFORMATION lpProcessInformation)
{
	return DetourCreateProcessWithDllA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
		bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation,
		ConvertUnicodeToMultiBytes(ToolFunc::Self.GetFullPath(L"DllInject.dll")).c_str(), RealCreateProcessA);
}

bool DetourMyFuncWithTrampoline()
{
	//< 蹦床函数跳转到截获函数
	DetourFunctionWithTrampoline((PBYTE)RealCreateProcessA, (PBYTE)MyCreateProcessA);

	return true;
}

int InjectDllByCreateProcess(char* szDllFullPath, char *szExePath, DWORD dwPid, HANDLE hDesProcess, HANDLE hDesThread)
{
	DWORD dwEntryPoint = 0;
	LPINJECT_CODE pszRemoteAddr = NULL;
	HMODULE			hModule = NULL;//GetRemoteModuleHandle(dwPid,szExePath);
	INJECT_LOADLIBRARY_CODE	InjectCode = { 0 };
	DWORD	dwNewEntryPoint = 0;
	SIZE_T	ReadedNumber = 0;
	int		nRet = 0;
	MEMORY_BASIC_INFORMATION	mbi = { 0 };
	CONTEXT ThreadCxt;
	ThreadCxt.ContextFlags = CONTEXT_FULL;
	if (GetThreadContext(hDesThread, &ThreadCxt) == FALSE)
		return 1;

	if (VirtualQueryEx(hDesProcess, (PVOID)ThreadCxt.Eax, &mbi, sizeof(mbi)) == 0)
		return 2;

	hModule = (HMODULE)mbi.AllocationBase;
	dwEntryPoint = ThreadCxt.Eax;
	pszRemoteAddr = (LPINJECT_CODE)VirtualAllocEx(hDesProcess, NULL, sizeof(INJECT_LOADLIBRARY_CODE), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	//xlog("Alloc:%x,EntryPoint:%x",pszRemoteAddr,dwEntryPoint);
	if (pszRemoteAddr == NULL)
		return 3;

	InjectCode.int_PUSHAD = 0x60;
	InjectCode.int_PUSHFD = 0x9c;
	InjectCode.int_PUSH = 0x68;
	InjectCode.push_Value = (DWORD)pszRemoteAddr + offsetof(INJECT_CODE, szDLL);
	InjectCode.int_MOVEAX = 0xb8;
	InjectCode.eax_Value = (DWORD)LoadLibraryA;
	InjectCode.call_eax = 0xD0FF;
	InjectCode.PopFd = 0x9d;
	InjectCode.PopAd = 0x61;
	InjectCode.jmp = 0xe9;
	InjectCode.jmp_Value = dwEntryPoint - ((DWORD)pszRemoteAddr + offsetof(INJECT_CODE, jmp_Value) + 4);
	strcpy_s(InjectCode.szDLL, MAX_PATH, szDllFullPath);

	if (WriteProcessMemory(hDesProcess, pszRemoteAddr, (PVOID)&InjectCode, sizeof(INJECT_CODE), &ReadedNumber) == FALSE)
		return 4;

	ThreadCxt.Eax = (DWORD)pszRemoteAddr;
	if (SetThreadContext(hDesThread, &ThreadCxt) == FALSE)
		return 5;

	return nRet;
}

HMODULE GetRemoteModuleHandle(const DWORD dwPID, const char* szModuleName)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;

	MODULEENTRY32 me32 = { 0 };
	HMODULE		hMod = NULL;
	size_t		size = 0;

	if (szModuleName == NULL)
		return NULL;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return NULL;

	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &me32))
	{
		//xlog("err:%d",GetLastError());
		goto _exit0;
	}

	{
		std::wstring wszModuleName = ConvertMultiBytesToUnicode(szModuleName);
		size = wszModuleName.size();
		do
		{
			if (_tcsnicmp(me32.szModule, wszModuleName.c_str(), size) == 0 || _tcsnicmp(me32.szExePath, wszModuleName.c_str(), size) == 0)
			{
				hMod = me32.hModule;
				break;
			}
		} while (Module32Next(hModuleSnap, &me32));
	}

_exit0:
	if (hModuleSnap != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hModuleSnap);
	}
	return hMod;

}

int UnInjectDll(const char *DllName, const DWORD dwRemoteProcessId)
{
	HANDLE hRemoteProcess = NULL, hRemoteThread = NULL;
	int nRet = 0;
	HMODULE	hMod = NULL;

	//打开目标进程
	if ((hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwRemoteProcessId)) == NULL)
		return 1;

	//得到LoadLibraryA函数地址
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");

	if (pfnStartAddr == NULL)
	{
		nRet = 2;
		goto _exit0;
	}

	hMod = GetRemoteModuleHandle(dwRemoteProcessId, DllName);
	if (hMod == NULL)
	{
		nRet = 3;
		goto _exit0;
	}

	if ((hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, hMod, 0, NULL)) == NULL)
	{
		nRet = 4;
		goto _exit0;
	}
_exit0:

	if (hRemoteProcess != NULL)
	{
		CloseHandle(hRemoteThread);
	}
	if (hRemoteProcess != NULL)
	{
		CloseHandle(hRemoteProcess);
	}

	return nRet;
}




int  MonitorProgram(char* szExePath, char *szDllName)
{
	BOOL bReturn = FALSE;
	DWORD BytesReturned = 0;
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = { 0 };
	wchar_t szDirecotyPath[MAX_PATH] = L"";
	wchar_t* pCurDir = NULL;
	char szInjectDll[MAX_PATH] = "";
	char szTemPath[MAX_PATH] = "";

	std::wstring wszExePath = ConvertMultiBytesToUnicode(szExePath);

	_wsplitpath_s(wszExePath.c_str(), szDirecotyPath, 3, &szDirecotyPath[2], MAX_PATH - 2, NULL, 0, NULL, 0);

	if (szDirecotyPath[0] != '\0')
		pCurDir = szDirecotyPath;

	//create process with  CREATE_SUSPENDED param
	bReturn = CreateProcess(
		wszExePath.c_str(),
		NULL,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, //for suspended.
		NULL,
		pCurDir,
		&si,
		&pi);

	if (!bReturn)
	{
		bReturn = CreateProcess(
			NULL,
			(LPWSTR)wszExePath.c_str(),
			NULL,
			NULL,
			FALSE,
			NORMAL_PRIORITY_CLASS | CREATE_SUSPENDED, //for suspended.
			NULL,
			pCurDir,
			&si,
			&pi);

		if (!bReturn)
		{
			return 1;
		}
	}

	//GetModuleFileName(NULL,szTemPath,MAX_PATH);
	//_splitpath_s(szTemPath,szInjectDll,3,&szInjectDll[2],MAX_PATH - 2,NULL,0,NULL,0);

	//strcat_s(szInjectDll,MAX_PATH,szDllName);

	int nRet = InjectDllByCreateProcess(szDllName, szExePath, pi.dwProcessId, pi.hProcess, pi.hThread);

	//xlog("inject:%d,%s",nRet,szDllName);
	ResumeThread(pi.hThread);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	if (nRet == 0)
	{
		//AddMonitorProcess(pi.dwProcessId);
	}

	return 0;
}