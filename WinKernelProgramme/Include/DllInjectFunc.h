#pragma once

#include <windows.h>

//<SE_PRIVILEGE_ENABLED表示该特权可用于调试及更改其它进程的内存
BOOL EnablePrivilege();

/************************************************************************/
/*使用远程线程来插入DLL
/************************************************************************/
bool InjectDll(DWORD dwProcessID, const char* dllPath);

/************************************************************************/
/*使用detours插入DLL
/************************************************************************/

bool DetourMyFuncWithTrampoline();


int UnInjectDll(const char *DllName, const DWORD dwRemoteProcessId);

HMODULE GetRemoteModuleHandle(const DWORD dwPID, const char* szModuleName);

int InjectDllByCreateProcess(char* szDllFullPath, char *szExePath, DWORD dwPid, HANDLE hDesProcess, HANDLE hDesThread);

int  MonitorProgram(char* szExePath, char *szDllName);