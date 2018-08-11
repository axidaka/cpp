#pragma once

#include <windows.h>


	void Event_testfunc(BOOL bManual = TRUE);

	//ManualRest test

	unsigned __stdcall	ThreadOutput(PVOID pvParam);
	unsigned __stdcall  ThreadLength(PVOID pvParam);
	unsigned __stdcall  ThreadConver(PVOID pvParam);

	//AutoReset test

	unsigned __stdcall	ThreadRead1(PVOID pvParam);
	unsigned __stdcall  ThreadRead2(PVOID pvParam);
	unsigned __stdcall  ThreadWrite(PVOID pvParam);