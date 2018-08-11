#pragma once

#include "stdafx.h"
#include <stdio.h>
#include "Windows.h"
#include "FileHelper.h"

#define SERVICE_NAME "polling_wallpaper"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
void WINAPI service_main(int argc, char** argv);
void WINAPI ServiceHandler(DWORD fdwControl);

TCHAR szSvcName[80];
SC_HANDLE schSCManager;
SC_HANDLE schService;
int uaquit;
//FILE* log;

DWORD WINAPI srv_core_thread(LPVOID para)
{
	std::wstring searchPath = L"G:\\bizhi";
	std::list<std::wstring> listPaperFile = FileHelper::GetFileList(searchPath, L".jpg");
	int i = 0;
	for (;;)
	{
		if (uaquit)
		{
			break;
		}
		
		auto iter_lstBegin = listPaperFile.begin();
		auto iter_lstEnd = listPaperFile.end();
		for (; iter_lstBegin != iter_lstEnd; iter_lstBegin ++)
		{
			CLogger::GetInstance().logEventFormat(Standard, FALSE, L"SystemParametersInfo: %s",
				iter_lstBegin->c_str());
			if (!SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (PVOID)iter_lstBegin->c_str(), SPIF_SENDCHANGE))
			{
				CLogger::GetInstance().logEventFormat(Errors, FALSE, L"SystemParametersInfo Faild(%s)",
					ToolFunc::FormatErrStr(GetLastError()).c_str());
			}

			Sleep(10*60*1000); // 
		}
			
	}
	return NULL;
}


void WINAPI ServiceHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		uaquit = 1;
		//add you quit code here
		break;
	default:
		return;
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}
}


void WINAPI service_main(int argc, char** argv)
{
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;
	hServiceStatusHandle = RegisterServiceCtrlHandler(_T(SERVICE_NAME), ServiceHandler);
	if (hServiceStatusHandle == 0)
	{
		DWORD nError = GetLastError();
	}
	//add your init code here

	//add your service thread here
	HANDLE task_handle = CreateThread(NULL, NULL, srv_core_thread, NULL, NULL, NULL);
	if (task_handle == NULL)
	{
		CLogger::GetInstance().logEventFormat(Errors, FALSE, L"create srv_thread failed(%s)",
			ToolFunc::FormatErrStr(GetLastError()).c_str());
	}

	// Initialization complete - report running status 
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 9000;
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		DWORD nError = GetLastError();
	}

}
//do not change main function
int my_main(int argc,  _TCHAR *argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[2];

	ServiceTable[0].lpServiceName = _T(SERVICE_NAME);
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_main;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	// 启动服务的控制分派机线程
	StartServiceCtrlDispatcher(ServiceTable);

	return 0;
}

