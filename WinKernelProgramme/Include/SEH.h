#pragma once

#include <windows.h>

	// try exception
DWORD TryException_test1();
void  TryFinally_test1();
void  GlobalUnwind_test1();
void  MultiLayersFinally_test();
void  ContinueExecution_test();
void  UnhandledExeption_test();   ///δ�����쳣�������


// Our global unhandled exception filter for instances of this class
 LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pep);