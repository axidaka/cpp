#pragma once

#include <windows.h>

//<SE_PRIVILEGE_ENABLED��ʾ����Ȩ�����ڵ��Լ������������̵��ڴ�
BOOL EnablePrivilege();

/************************************************************************/
/*ʹ��Զ���߳�������DLL
/************************************************************************/
bool InjectDll(DWORD dwProcessID, const char* dllPath);

/************************************************************************/
/*ʹ��detours����DLL
/************************************************************************/

bool DetourMyFuncWithTrampoline();


int UnInjectDll(const char *DllName, const DWORD dwRemoteProcessId);

HMODULE GetRemoteModuleHandle(const DWORD dwPID, const char* szModuleName);

int InjectDllByCreateProcess(char* szDllFullPath, char *szExePath, DWORD dwPid, HANDLE hDesProcess, HANDLE hDesThread);

int  MonitorProgram(char* szExePath, char *szDllName);