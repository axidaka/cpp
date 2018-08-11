#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>


//typedef LONG NTSTATUS;
//#define NT_SUCCESS(status)      ((NTSTATUS)(status)>=0)
//
///*
//* 虽然本程序用不到这么多枚举值，还是列出一份最完整的。这个程序本身不求完
//* 美，尽可能多地保留一些未文档化的参考资料。
//*/
//typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S
//{
//	SystemBasicInformation,                // 00 Y N
//	SystemProcessorInformation,            // 01 Y N
//	SystemPerformanceInformation,          // 02 Y N
//	SystemTimeOfDayInformation,            // 03 Y N
//	SystemNotImplemented1,                 // 04 Y N
//	SystemProcessesAndThreadsInformation,  // 05 Y N
//	SystemCallCounts,                      // 06 Y N
//	SystemConfigurationInformation,        // 07 Y N
//	SystemProcessorTimes,                  // 08 Y N
//	SystemGlobalFlag,                      // 09 Y Y
//	SystemNotImplemented2,                 // 10 Y N
//	SystemModuleInformation,               // 11 Y N
//	SystemLockInformation,                 // 12 Y N
//	SystemNotImplemented3,                 // 13 Y N
//	SystemNotImplemented4,                 // 14 Y N
//	SystemNotImplemented5,                 // 15 Y N
//	SystemHandleInformation,               // 16 Y N
//	SystemObjectInformation,               // 17 Y N
//	SystemPagefileInformation,             // 18 Y N
//	SystemInstructionEmulationCounts,      // 19 Y N
//	SystemInvalidInfoClass1,               // 20
//	SystemCacheInformation,                // 21 Y Y
//	SystemPoolTagInformation,              // 22 Y N
//	SystemProcessorStatistics,             // 23 Y N
//	SystemDpcInformation,                  // 24 Y Y
//	SystemNotImplemented6,                 // 25 Y N
//	SystemLoadImage,                       // 26 N Y
//	SystemUnloadImage,                     // 27 N Y
//	SystemTimeAdjustment,                  // 28 Y Y
//	SystemNotImplemented7,                 // 29 Y N
//	SystemNotImplemented8,                 // 30 Y N
//	SystemNotImplemented9,                 // 31 Y N
//	SystemCrashDumpInformation,            // 32 Y N
//	SystemExceptionInformation,            // 33 Y N
//	SystemCrashDumpStateInformation,       // 34 Y Y/N
//	SystemKernelDebuggerInformation,       // 35 Y N
//	SystemContextSwitchInformation,        // 36 Y N
//	SystemRegistryQuotaInformation,        // 37 Y Y
//	SystemLoadAndCallImage,                // 38 N Y
//	SystemPrioritySeparation,              // 39 N Y
//	SystemNotImplemented10,                // 40 Y N
//	SystemNotImplemented11,                // 41 Y N
//	SystemInvalidInfoClass2,               // 42
//	SystemInvalidInfoClass3,               // 43
//	SystemTimeZoneInformation,             // 44 Y N
//	SystemLookasideInformation,            // 45 Y N
//	SystemSetTimeSlipEvent,                // 46 N Y
//	SystemCreateSession,                   // 47 N Y
//	SystemDeleteSession,                   // 48 N Y
//	SystemInvalidInfoClass4,               // 49
//	SystemRangeStartInformation,           // 50 Y N
//	SystemVerifierInformation,             // 51 Y Y
//	SystemAddVerifier,                     // 52 N Y
//	SystemSessionProcessesInformation      // 53 Y N
//} SYSTEM_INFORMATION_CLASS;
//
//typedef struct _SYSTEM_MODULE_INFORMATION  // Information Class 11
//{
//	ULONG  Reserved[2];
//	PVOID  Base;
//	ULONG  Size;
//	ULONG  Flags;
//	USHORT Index;
//	USHORT Unknown;
//	USHORT LoadCount;
//	USHORT ModuleNameOffset;
//	CHAR   ImageName[256];
//} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;
//
///*
//* 参看DDK文档以及<<Windows NT/2000 Native API Reference>> by Gary Nebbett
//* 这些Native API由ntdll.dll输出
//*/
//typedef NTSTATUS(__stdcall *ZWQUERYSYSTEMINFORMATION) (IN SYSTEM_INFORMATION_CLASS SystemInformationClass, IN OUT PVOID SystemInformation, IN ULONG SystemInformationLength, OUT PULONG ReturnLength OPTIONAL);
//typedef ULONG(__stdcall *RTLNTSTATUSTODOSERROR) (IN NTSTATUS Status);
//
//namespace SysLevelFunc
//{
//};
//
//
//
//
///*
//* <<Windows NT/2000 Native API Reference>> by Gary Nebbett
//*************************************************************************
//*/
//
//
//
///************************************************************************
//*                                                                      *
//*                            Function Prototype                        *
//*                                                                      *
//************************************************************************/
//
//static void    ListModule(void);
//static BOOLEAN LocateNtdllEntry(void);
//static void    PrintWin32Error(char *message, DWORD dwMessageId);
//static void    PrintZwError(char *message, NTSTATUS status);
//static PVOID   PrivateFindModule(const char *ModuleName);
//
///************************************************************************
//*                                                                      *
//*                            Static Global Var                         *
//*                                                                      *
//************************************************************************/
//
///*
//* 由ntdll.dll输出的Native API函数指针
//*/
//static ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation = NULL;
//static RTLNTSTATUSTODOSERROR    RtlNtStatusToDosError = NULL;
//
///************************************************************************/
//
//static void ListModule(void)
//{
//	NTSTATUS                    status;
//	PSYSTEM_MODULE_INFORMATION  module = NULL;
//	ULONG                       n = 0;
//	ULONG                       i = 0;
//	void                       *buf = NULL;
//
//	ZwQuerySystemInformation(SystemModuleInformation, &n, 0, &n);
//	if (NULL == (buf = calloc((size_t)n, 1)))
//	{
//		fprintf(stderr, "calloc() failed\n");
//		goto ListModule_return;
//	}
//	/*
//	* <<Windows NT/2000 Native API Reference>> by Gary Nebbett所给的例子
//	* 1.3以及A.2对第二、三形参理解有误，下面才是正确的用法。
//	*/
//	status = ZwQuerySystemInformation(SystemModuleInformation, buf, n, NULL);
//	if (!NT_SUCCESS(status))
//	{
//		PrintZwError("ZwQuerySystemInformation() failed", status);
//		goto ListModule_return;
//	}
//	module = (PSYSTEM_MODULE_INFORMATION)((PULONG)buf + 1);
//	n = *((PULONG)buf);
//	for (i = 0; i < n; i++)
//	{
//		printf("0x%08X %s\n", module[i].Base, module[i].ImageName + module[i].ModuleNameOffset);
//	}
//
//ListModule_return:
//
//	if (buf != NULL)
//	{
//		free(buf);
//		buf = NULL;
//	}
//	return;
//}  /* end of ListModule */
//
///*
//* ntdll.dll输出了所有的Native API
//*/
//static BOOLEAN LocateNtdllEntry(void)
//{
//	BOOLEAN boolean_ret = FALSE;
//	char    NTDLL_DLL[] = "ntdll.dll";
//	HMODULE ntdll_dll = NULL;
//
//	/*
//	* returns a handle to a mapped module without incrementing its
//	* reference count
//	*/
//	if ((ntdll_dll = GetModuleHandle(NTDLL_DLL)) == NULL)
//	{
//		PrintWin32Error("GetModuleHandle() failed", GetLastError());
//		return(FALSE);
//	}
//	if (!(ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(ntdll_dll,
//		"ZwQuerySystemInformation")))
//	{
//		goto LocateNtdllEntry_return;
//	}
//	if (!(RtlNtStatusToDosError = (RTLNTSTATUSTODOSERROR)GetProcAddress(ntdll_dll,
//		"RtlNtStatusToDosError")))
//	{
//		goto LocateNtdllEntry_return;
//	}
//	boolean_ret = TRUE;
//
//LocateNtdllEntry_return:
//
//	if (FALSE == boolean_ret)
//	{
//		PrintWin32Error("GetProcAddress() failed", GetLastError());
//	}
//	ntdll_dll = NULL;
//	return(boolean_ret);
//}  /* end of LocateNtdllEntry */
//
//static void PrintWin32Error(char *message, DWORD dwMessageId)
//{
//	char *errMsg;
//
//	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
//		dwMessageId,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&errMsg, 0, NULL);
//	fprintf(stderr, "%s: %s", message, errMsg);
//	LocalFree(errMsg);
//	return;
//}  /* end of PrintWin32Error */
//
//static void PrintZwError(char *message, NTSTATUS status)
//{
//	char *errMsg;
//
//	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
//		RtlNtStatusToDosError(status),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&errMsg, 0, NULL);
//	fprintf(stderr, "%s: %s", message, errMsg);
//	LocalFree(errMsg);
//	return;
//}  /* end of PrintZwError */
//
//int main(int argc, char * argv[])
//{
//	if (FALSE == LocateNtdllEntry())
//	{
//		fprintf(stderr, "LocateNtdllEntry() failed\n");
//		return(EXIT_FAILURE);
//	}
//	ListModule();
//	system("pause");
//	return(EXIT_SUCCESS);
//}  /* end of main */

/************************************************************************/

#include <windows.h>
#include <TlHelp32.h>
#include "stldefine.h"

namespace SysLevelFunc
{
	//<根据线程名查找线程ID
	DWORD GetProcessHandle(LPTSTR lpProcessName);

	void EnumProcessWnds(LPTSTR lpProcessName);

#define  WINDOW_TEXT_LEN 256

	BOOL CALLBACK EnumChildProc(_In_  HWND hwnd, _In_  LPARAM lParam);

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

}