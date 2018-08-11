#pragma once

#include <string.h>
#include "base64code.h"
#include "UnicodeConv.h"
#include "RootDirectory.h"

//< 生成Base64编码且Utf-8的字符串
void StringToBase64AndUtf8()
{
	{
		// string 进行Base64 且是Utf-8
		std::string strUtf8 = ConvertUnicodeToMultiBytes(L"缔造者1", CP_UTF8);
		std::string strBase64 = duowanTool::EncodeBase64(/*"缔造者1"*/strUtf8);
		std::wstring wstrBase64 = ConvertMultiBytesToUnicode(strBase64);
	}

	{
	std::wstring wstr = ConvertMultiBytesToUnicode("缔造者1");
	std::string strUtf8 = ConvertUnicodeToMultiBytes(wstr, CP_UTF8);
	std::string strUtf8Base64 = duowanTool::EncodeBase64(strUtf8);
	std::wstring wstrUtf8base64 = ConvertMultiBytesToUnicode(strUtf8Base64);
}

	{
		///string to base64 wstring  utf-8
		std::string strBase64 = duowanTool::EncodeBase64("犬夜叉牛2");
		std::wstring wstrBase64 = ConvertMultiBytesToUnicode(strBase64, CP_UTF8);
	}
}

#include <map>
#include <set>
#include "stldefine.h"
using namespace std;

void testMap2Map()
{
	map<tstring, map<tstring, set<tstring>>> map2map;
	set<tstring> setstr;
	setstr.insert(_T("LastValue"));
	map<tstring, set<tstring>> ValueMap;
	ValueMap.insert(make_pair(_T("SecondKey"), setstr));
	map2map.insert(make_pair(_T("FirstKey"), ValueMap));
}

#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

class GdiPlusIniter{
public:
	GdiPlusIniter(){
		Gdiplus::GdiplusStartupInput StartupInput;
		GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
	}

	~GdiPlusIniter(){
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}
private:
	ULONG_PTR m_gdiplusToken;
};

#include "ToolFunc.h "

void ModifyResolution()
{
	ToolFunc::CRootDirectory rootdir;
	rootdir.Init(ToolFunc::GetProgramDirectory());
	std::wstring picpath = rootdir.GetFullPath(L"ac.PNG");
	Bitmap* pSrcBitmap = Bitmap::FromFile(picpath.c_str());
	tstring strErr = ToolFunc::FormatErrStr(GetLastError());
	if (pSrcBitmap == NULL || pSrcBitmap->GetLastStatus() != Gdiplus::Status::Ok)
	{
		if (pSrcBitmap != NULL)
		{
			delete pSrcBitmap;
		}

		return;
	}

	tcout << "width :" << pSrcBitmap->GetWidth() << ENDL;
	tcout << "height :" << pSrcBitmap->GetHeight() << ENDL;

	Gdiplus::Status status = pSrcBitmap->SetResolution(800, 600);

	if (status != Gdiplus::Status::Ok)
	{
		delete pSrcBitmap;
		pSrcBitmap = NULL;
		return;
	}

	///< 保存图片
	CLSID encoderClsid;
	EncoderParameters encoderParameters;
	ULONG quality = 70;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &quality;
	ToolFunc::GetEncoderClsid(L"image/jpeg", &encoderClsid);

	wstring newPath = rootdir.GetRootDirectory() + L"\\new.png";
	pSrcBitmap->Save(newPath.c_str(), &encoderClsid, &encoderParameters);

	delete pSrcBitmap;
	pSrcBitmap = NULL;

}

void ScreenShotByGdiplus(HWND hwnd)
{
	GdiPlusIniter gdipluginit;

	HDC hWndDC = GetWindowDC(hwnd);
	RECT rtClientWnd;
	::GetClientRect(hwnd, &rtClientWnd);
	//  GetWindowRect(hwnd, &rtScreen);
	int width = rtClientWnd.right - rtClientWnd.left;
	int height = rtClientWnd.bottom - rtClientWnd.top;

	HDC hMemDC = CreateCompatibleDC(hWndDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hWndDC, width, height);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	BitBlt(hMemDC, 0, 0, width, height, hWndDC, 0, 0, SRCCOPY);

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hWndDC);
	DeleteDC(hMemDC);

	CLSID encoderClsid;
	EncoderParameters encoderParameters;
	ULONG quality = 100;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &quality;
	ToolFunc::GetEncoderClsid(L"image/jpeg", &encoderClsid);

	Gdiplus::Bitmap gditBitmap(hBitmap, NULL);
	
	cout << "width :" << gditBitmap.GetWidth() << endl;
	cout << "Height :" << gditBitmap.GetHeight() << endl;
		Gdiplus::Image* pThumbnail = gditBitmap.GetThumbnailImage(800, 600, NULL, NULL);
		if (pThumbnail != NULL)
		{
			pThumbnail->Save(L"D://800.png", &encoderClsid, &encoderParameters);
			delete pThumbnail;
			pThumbnail = NULL;
		}
	
	//gditBitmap.Save(L"D://screen.jpg", &encoderClsid, &encoderParameters);

	DeleteObject(hBitmap);
}

void ScreenShotByWndPos(HWND hWnd)
{
	HDC       hScrDC, hMemDC;// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;  // 位图句柄
	int       nX, nY, nX2, nY2;      	// 选定区域坐标
	int       nWidth, nHeight;

	RECT* pRtWnd = NULL;
	WINDOWINFO windowInfo;
	windowInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &windowInfo);
	pRtWnd = &windowInfo.rcWindow;

	// 获得选定区域坐标
	nX = pRtWnd->left;
	nY = pRtWnd->top;
	nX2 = pRtWnd->right;
	nY2 = pRtWnd->bottom;

	cout << "top: " << pRtWnd->top << endl;
	cout << "left: "<<pRtWnd->left<<endl;

	nWidth = pRtWnd->right - pRtWnd->left;
	nHeight = pRtWnd->bottom - pRtWnd->top;

	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL); //为屏幕创建设备描述表	
	hMemDC = CreateCompatibleDC(hScrDC); //为屏幕设备描述表创建兼容的内存设备描述表

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); // 创建一个与屏幕设备描述表兼容的位图	
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);	// 把新位图选到内存设备描述表中

	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	//////////////////////////////////////////////////////
	CLSID encoderClsid;
	EncoderParameters encoderParameters;
	ULONG quality = 100;
	encoderParameters.Count = 1;
	encoderParameters.Parameter[0].Guid = EncoderQuality;
	encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
	encoderParameters.Parameter[0].NumberOfValues = 1;
	encoderParameters.Parameter[0].Value = &quality;
	ToolFunc::GetEncoderClsid(L"image/jpeg", &encoderClsid);

	Gdiplus::Bitmap gditBitmap(hBitmap, NULL);

	cout << "width :" << gditBitmap.GetWidth() << endl;
	cout << "Height :" << gditBitmap.GetHeight() << endl;
	//Gdiplus::Image* pThumbnail = gditBitmap.GetThumbnailImage(800, 600, NULL, NULL);
	//if (pThumbnail != NULL)
	//{
	//	pThumbnail->Save(L"D://800.png", &encoderClsid, &encoderParameters);
	//	delete pThumbnail;
	//	pThumbnail = NULL;
	//}

	gditBitmap.Save(L"D://screen.jpg", &encoderClsid, &encoderParameters);

	DeleteObject(hBitmap);
}

#include "D3DScreenShot.h"
///< test different ways of screenshot
void TestDifferentWaysOfScreenShot()
{
	//CD3DScreenShot screenshot;
	//screenshot.Init();

	//tcout << L"按下b键" << ENDL;
	//if (RegisterHotKey(NULL, 1, 0, 0x42))  //0x42 is 'b'
	//{
	//	_tprintf(_T("Hotkey 'ALT+b' registered, using MOD_NOREPEAT flag\n"));
	//}

	// 
	//MSG msg = { 0 };
	//while (::GetMessage(&msg, NULL, 0, 0) != 0)
	//{
	//	if (msg.message == WM_HOTKEY)
	//	{
	//		POINT pNow = { 0, 0 };
	//		if (::GetCursorPos(&pNow))
	//		{

	//			HWND hwndPoint = NULL;
	//			hwndPoint = ::WindowFromPoint(pNow);
	//			if (hwndPoint != NULL)
	//			{
	//				//cout << "Success get hwnd" << ENDL;
	//				//ScreenShotByWndPos(hwndPoint);

	//				//int i = 0;
	//				//cout << "input 0=windowed 1=fullscreen" << endl;
	//				//cin >> i;
	//				//bool bFullScreen = (0 == i) ? false : true;
	//				if (!screenshot.ScreenShotByHwnd(hwndPoint, true))
	//				{
	//					OutputDebugString(screenshot.GetLastErr().c_str());
	//				}
	//			}
	//		}
	//	}
	//}
	//screenshot.Uninit();
	//UnregisterHotKey(NULL, 1);


}

#include "StringHelper.h"

void TestStringHelper()
{
	{
		tstring strRet = StringHelper::ToUpper(_T("AbCd"));
		_tprintf(_T("ToUpperW Result:%s\n"), strRet.c_str());
	}

	{
		tstring strRet = StringHelper::ToLower(_T("AbCd"));
		_tprintf(_T("ToLowerW Result:%s\n"), strRet.c_str());
	}

	{
		string strRet = StringHelper::ToUpperA("AbCd");
		printf("ToUpperA Result:%s\n", strRet.c_str());
	}

	{
		string strRet = StringHelper::ToLowerA("AbCd");
		printf("ToLowerW Result:%s\n", strRet.c_str());
	}
}

//#include "ThreadSafeString.h"
#include "Rational.h"
#include "Rational2.h"
#include "Rational3.h"
#include "Rational4.h"
#include "DetectMemoryLeak.h"      ///< 检测内存泄露

template<class T>
void MemoryPoolTest()
{
	T* array[1000];
	T::newMemPool();

	DWORD dwStart = GetTickCount();
	cout << "Start Time:" << dwStart << endl;
	{
		for (int j = 0; j < 500; j++)
		{
			for (int i = 0; i < 1000; i++)
			{
				array[i] = new T(i);
			}

			for (int i = 0; i < 1000; i++)
			{
				delete array[i];
			}
		}
	}

	T::deleteMemPool();

	DWORD dwEnd = GetTickCount();
	cout << "End Time:" << dwEnd << endl;

	cout << "Running Time is :" << dwEnd - dwStart << endl;
};

int TestMemPoolFunc()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	//{
	//	ThreadSafeString<CriticalSectionLock> threadsafestring("ChapterThree");
	//	cout<<"the len is"<<threadsafestring.length()<<endl;
	//}


	//{
	//	Rational* array[1000];
	//	Rational::newMemPool();

	//	DWORD dwStart = GetTickCount();
	//	cout<<"Start Time:"<<dwStart<<endl;
	//	{
	//		for (int j = 0; j < 500; j ++)
	//		{
	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				array[i] = new Rational(i);
	//			}

	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				delete array[i];
	//			}
	//		}
	//	}

	//	Rational::deleteMemPool();

	//	DWORD dwEnd = GetTickCount();
	//	cout<<"End Time:"<<dwEnd<<endl;

	//	cout<<"Running Time is :"<<dwEnd - dwStart<<endl;
	//}

	//{
	//	Rational2* array[1000];
	//	Rational2::newMemPool();

	//	DWORD dwStart = GetTickCount();
	//	cout<<"Start Time:"<<dwStart<<endl;
	//	{
	//		for (int j = 0; j < 500; j ++)
	//		{
	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				array[i] = new Rational2(i);
	//			}

	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				delete array[i];
	//			}
	//		}
	//	}

	//	Rational2::deleteMemPool();

	//	DWORD dwEnd = GetTickCount();
	//	cout<<"End Time:"<<dwEnd<<endl;

	//	cout<<"Running Time is :"<<dwEnd - dwStart<<endl;
	//}

	//{
	//	Rational3* array[1000];
	//	Rational3::newMemPool();

	//	DWORD dwStart = GetTickCount();
	//	cout<<"Start Time:"<<dwStart<<endl;
	//	{
	//		for (int j = 0; j < 500; j ++)
	//		{
	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				array[i] = new Rational3(i);
	//			}

	//			for (int i = 0; i < 1000; i ++)
	//			{
	//				delete array[i];
	//			}
	//		}
	//	}

	//	Rational3::deleteMemPool();

	//	DWORD dwEnd = GetTickCount();
	//	cout<<"End Time:"<<dwEnd<<endl;

	//	cout<<"Running Time is :"<<dwEnd - dwStart<<endl;
	//}

	MemoryPoolTest<Rational2>();
	MemoryPoolTest<Rational3>();
	MemoryPoolTest<Rational4>();

	return 0;
}

#include "Log.h"
#include "WinRWFile.h"

void TestLog()
{
	CLog<CWinRWFile> log;
	wstring szFile = ToolFunc::Self.GetSubDirFilePath(L"Log", L"testlog.log");
	log.Enable(TRUE);
	log.EnableConsolePrint(TRUE);
	log.EnableIDEPrint(TRUE);

	if (log.Open(szFile.c_str(), CLog<CWinRWFile>::LL_INFORMATION, TRUE))
	{
		string str;
		while (cin >> str)
		{
			log.WriteA("输入:%s", str.c_str());
			
			wstring wstr = ConvertMultiBytesToUnicode(str);
			log.WriteW(L"W输入;%s", wstr.c_str());
			str.clear();
		}
	}

	log.Close();
}

///< 检测DNF盒子全屏模式和窗口模式
void JudgeFullScreenMode()
{
	HWND hwndDnf = ::FindWindow(_T("地下城与勇士"), _T("地下城与勇士"));
	if (hwndDnf != NULL)
	{
		DWORD dwExStyle = ::GetWindowLong(hwndDnf, GWL_EXSTYLE);
		if (!(dwExStyle & WS_EX_TOPMOST))
		{
			tcout << "窗口模式" << endl;
		}
		else
		{	
			tcout << "全屏模式" << endl;
		}
	}
	
}

#include "MySharedPtr.h"

void TestMySharedPtr()
{
	CMySharedPtr<int> sptr(new int(100));
	printf("SharePtr:%d\n", *sptr.get());
	printf("sptr UseCount;%d\n", sptr.use_count());

	{
		CMySharedPtr<int> sptr2(sptr);
		printf("SharePtr:%d\n", *sptr2.get());
		printf("sptr UseCount;%d\n", sptr2.use_count());
	}
}

#include <process.h>
//#ifdef _DEBUG
// #pragma comment(lib, "DllInjectd.lib")
// #else
// #pragma comment(lib, "DllInject.lib")
// #endif
unsigned _stdcall ThreadTest(void* LPparam)
{
	printf("LoadLibrary\n");

	HMODULE  hin = LoadLibrary(L"DllInjectd.dll");
	if (hin == NULL)
	{
		printf("%s\n", ToolFunc::FormatErrStr(GetLastError()).c_str());
	}
	FreeLibrary(hin);
	return 0;
}

void TestDllInject()
{
	
	//printf("%d\n", fnDllInject());
	HANDLE hthread = (HANDLE)_beginthreadex(NULL, 0, &ThreadTest, NULL, NULL, NULL);
	if (hthread != NULL)
	{
		CloseHandle(hthread);
	}
}

#include "Event_test.h"
#include "Semaphore_test.h"
#include "Mutex_test.h"
#include "FileMap.h"
#include "DllInjectFunc.h"

void DllInject_Test()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	tstring strCmdline = _T("Notepad");

	if (CreateProcess(NULL, (LPTSTR)strCmdline.c_str(),
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
	{
		wstring dllPath = ToolFunc::Self.GetFullPath(L"DllInjectd.dll");
		InjectDll(pi.dwProcessId, ConvertUnicodeToMultiBytes(dllPath).c_str());
		CloseHandle(pi.hProcess);
	}
}

void DllInjectByDetours_Test()
{
	DetourMyFuncWithTrampoline();

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	string strCmdline = "Notepad";

	if (CreateProcessA(NULL, (LPSTR)strCmdline.c_str(),
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
	}
}

void Judge_FAT32()
{
	TCHAR RootPathName[] = TEXT("G:\\"); //查询分区根目录

	TCHAR FSType[20];

	if (!GetVolumeInformation(RootPathName, NULL, 0,

		NULL,

		NULL,

		NULL, FSType, sizeof(FSType)))

	{

		DWORD error = GetLastError();

		exit(0);

	}

	if (_tcscmp(FSType, _T("FAT32")) == 0)
		//是FAT32
	{

		//你的代码
		printf("Fat");
	}

	else if (_tcscmp(FSType, _T("NTFS")) == 0)
		//是NTFS
	{

		//你的代码
		printf("NTFS");
	}
	else
	{
		printf("others");
	}
}

#include "ShareMem.h"
#include "Logger.h"
#include "ToolMacros.h"

volatile  LONG g_fShutDown = FALSE;
HANDLE    g_hThreads[MAXIMUM_WAIT_OBJECTS];
int       g_nNumThreads = 0;
CShareMem g_shareMem;

unsigned __stdcall ReadShareMemThread(PVOID pvParam) {
	int nThreadNum = PtrToUlong(pvParam);
	char* pOutput = new char[2];
	int nOffset = 0;

	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0))
	{
		ZeroMemory(pOutput, 2);
		g_shareMem.ReadMem((PBYTE)pOutput, 1, nOffset++);
	
		if (nOffset == 52) nOffset = 0;
		Sleep(1000);
	}

	delete[] pOutput;
	_endthreadex(0);
	return 0;
}

unsigned __stdcall WriteShareMemThread(PVOID pvParam) {

	int nThreadNum = PtrToUlong(pvParam);

	char* pOutput = new char[2];
	_itoa(nThreadNum, pOutput, 10);
	//strcpy(pOutput, "1");
	int nOffset = 0;

	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0))
	{
		//ZeroMemory(pOutput, 2);
		g_shareMem.WriteMem((PBYTE)pOutput, 1, nOffset++);

		if (nOffset == 52) nOffset = 0;
		Sleep(1000);
	}

	delete[] pOutput;
	_endthreadex(0);
	return 0;
}

void ThreadSafeShareMem_test()
{
	_tprintf(_T("Input Q or q to Exit\n"));
	
	g_shareMem.Create(L"ThreadSafeShareMem_test", 255);
	string strInputData("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	g_shareMem.WriteMem((PBYTE)strInputData.c_str(), strInputData.size(), 0);

	unsigned dwThreadID;
	for (int i = 0; i < 2; i ++)
	{
		g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, ReadShareMemThread,
			(PVOID)(INT_PTR)i, 0, &dwThreadID);
	}

	for (int i = 0; i < 2; i++)
	{
		g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, WriteShareMemThread,
			(PVOID)(INT_PTR)(i+2), 0, &dwThreadID);
	}

	int ch;
	ch = getchar();
	while (ch != 'Q' && ch != 'q')
	{
		ch = getchar();
	}

	InterlockedExchange(&g_fShutDown, TRUE);

	InterlockedExchange(&g_fShutDown, TRUE);

	WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);
	while (g_nNumThreads--)
		Close_IFHandle(g_hThreads[g_nNumThreads]);

}

#include "SEH.h"

#include "DbgFunc.h"

void SEH_test()
{
	CSEHException theException;

	try
	{
		int x, y = 0;
		x = 5 / y;
	}
	catch (CSE_Exception &ex)
	{
		ex.Report(L"SEH_test raise an exception");
	}
}

#include "ProcMsgQueue.h"

void ProcMsgQueue_Test()
{
	CProcMsgQueue pmq;
	int nInput = 0;

	pmq.Create(L"ProcMsgQueue_test");

	while (cin>>nInput)
	{
		cout << "1 Write" << endl;
		cout << "2 Read" << endl;

		switch (nInput)
		{
		case 1:
		{
				  ProcMsgItem* pitem = NULL;
				  cout << "Input:" << endl;
				  string strInput;
				  cin >> strInput;
				  int nlen = strInput.size();
				  char* pTemp = new char[8 + nlen];
				  ZeroMemory(pTemp, 8 + nlen);

				  pitem = (ProcMsgItem*)pTemp;
				  pitem->dwSize = nlen + (sizeof(DWORD)) * 2;
				  pitem->dwSrcPid = 16;
				  memcpy(pitem->buf, strInput.c_str(), nlen);
				  pmq.EnQueue(*pitem);

				  delete[]pTemp;
				  pTemp = NULL;

				 //while (true)
				 //{
					// string strInput("ProcessShareTest");
			
					// int nlen = strInput.size();
					// char* pTemp = new char[8 + nlen];
					// ZeroMemory(pTemp, 8 + nlen);

					// ProcMsgItem* pitem = NULL;
					// pitem = (ProcMsgItem*)pTemp;
					// pitem->dwSize = nlen + (sizeof(DWORD)) * 2;
					// pitem->dwSrcPid = 16;
					// memcpy(pitem->buf, strInput.c_str(), nlen);
					// if (FAILED(pmq.EnQueue(*pitem)))
					// {
					//	 delete[]pTemp;
					//	 pTemp = NULL;
					//	 printf("队列已满\n");
					//	 break;
					// }

					// delete[]pTemp;
					// pTemp = NULL;
				 //}
		}
			break;
		case 2:
		{
				  while (true)
				  {
					  ProcMsgItem* pItem = NULL;
					  if (FAILED(pmq.DeQueue(pItem)) || pItem == NULL)
					 {
						  delete pItem;
						  pItem = NULL;
						  printf("队列已空\n");
						  break;
					 }

					  int nlen = pItem->dwSize - 8 + 1;
					  char *pTemp = new char[nlen];
					  ZeroMemory(pTemp, nlen);
					  memcpy(pTemp, (char*)pItem->buf, nlen);
					  pTemp[nlen - 1] = '\0';

					 // CLogger::GetInstance().logEventFormat(Standard, FALSE, pTemp);
					  printf("%s\n", pTemp);

					  delete pItem;
					  pItem = NULL;


					  delete[]pTemp;
					  pTemp = NULL;
				  }
				
		}
			break;
		default:
			break;
		}
	}
}

CProcMsgQueue g_promsg;

unsigned __stdcall ReadProcMsgThread(PVOID pvParam) {

	ProcMsgItem* pItem = NULL;

	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0))
	{
		
		if (FAILED(g_promsg.DeQueue(pItem)) || pItem == NULL)
		{
			printf("队列已空\n");
		//	Sleep(2000);
			continue;
		}

		int nlen = pItem->dwSize - 8 + 1;
		char *pTemp = new char[nlen];
		ZeroMemory(pTemp, nlen);
		memcpy(pTemp, (char*)pItem->buf, nlen);
		pTemp[nlen - 1] = '\0';

		CLogger::GetInstance().logEventFormat(Standard, FALSE, "ReadProcMsg:%s\n", pTemp);
		//printf("ReadProcMsg:%s\n", pTemp);

		delete pItem;
		pItem = NULL;


		delete[]pTemp;
		pTemp = NULL;

		Sleep(1000);
	}


	_endthreadex(0);
	return 0;
}

unsigned __stdcall WriteProcMsgThread(PVOID pvParam) {

	int nThreadNum = PtrToUlong(pvParam);

	char* pOutput = new char[2];
	_itoa(nThreadNum, pOutput, 10);
	//strcpy(pOutput, "1");
	int nOffset = 0;

	while (1 != InterlockedCompareExchange(&g_fShutDown, 0, 0))
	{
		ProcMsgItem* pitem = NULL;

		string strInput(StringHelper::FormatA("WriteProcMsg:%d--%d", nThreadNum, nOffset++));

		int nlen = strInput.size();
		char* pTemp = new char[8 + nlen];
		ZeroMemory(pTemp, 8 + nlen);

		pitem = (ProcMsgItem*)pTemp;
		pitem->dwSize = nlen + (sizeof(DWORD)) * 2;
		pitem->dwSrcPid = 16;
		memcpy(pitem->buf, strInput.c_str(), nlen);
		g_promsg.EnQueue(*pitem);

		CLogger::GetInstance().logEventFormat(Standard, FALSE, "%s\n", strInput.c_str());

		delete[]pTemp;
		pTemp = NULL;
		Sleep(3000);
	}

	delete[] pOutput;
	_endthreadex(0);
	return 0;
}

void ThreadSafeProcMsg_test()
{
	_tprintf(_T("Input Q or q to Exit\n"));

	g_promsg.Create(L"ThreadSafePeocMsg_test");

	unsigned dwThreadID;
	//for (int i = 0; i < 1; i++)
	//{
	//	g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, ReadProcMsgThread,
	//		(PVOID)(INT_PTR)i, 0, &dwThreadID);
	//}

	for (int i = 0; i < 2; i++)
	{
		g_hThreads[g_nNumThreads++] = (HANDLE)_beginthreadex(NULL, 0, WriteProcMsgThread,
			(PVOID)(INT_PTR)(i + 2), 0, &dwThreadID);
	}

	int ch;
	ch = getchar();
	while (ch != 'Q' && ch != 'q')
	{
		ch = getchar();
	}

	InterlockedExchange(&g_fShutDown, TRUE);

	InterlockedExchange(&g_fShutDown, TRUE);

	WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);
	while (g_nNumThreads--)
		Close_IFHandle(g_hThreads[g_nNumThreads]);

}

#include "AnnotateSTL.h"

void Functor_Test()
{
	Plus<int> plusObj;
	Minus<int> minusObj;

	//  使用仿函数跟使用一般函数一样
	cout << plusObj(1, 2) << endl;
	cout << minusObj(2, 1) << endl;

	// 直接产生仿函数的临时对象
	cout << Plus<int>()(1, 2) << endl;
	cout << Minus<int>()(2, 1) << endl;
}

#include "WriteDump.h"

#include "MyException.h"

void MyException_Test()
{
	try
	{
		throw CMyException();
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
	}
}

struct Big
{
	double stuff[20000];
};

#include <new>

// bad_alloc 异常和 new，对于使用new导致的内存分配问题，C++最新处理方式是让
// new引发 bad_alloc异常
void BadAllocException_Test()
{
	Big* pb;

	try{
		cout << "Trying to get a big block of memory:\n";
		pb = new Big[10000];   ///new 堆空间到底能申请多大
		cout << "Got past the new request:\n";
	}
	catch (bad_alloc &ba)
	{
		cout << "Caught the exception!\n";
		cout << ba.what() << endl;
		exit(EXIT_FAILURE);
	}

	cout << "Memory successfully allocated\n";
	pb[0].stuff[0] = 4;
	cout << pb[0].stuff[0] << endl;
	delete[]pb;

}

void BadIndexException_Test()
{
	double values[12] = 
	{
		1111,2222,3333,4444,5555,6666,
		7777,8888,9999,1234,2345,3456
	};

	CSales yearSales(2014, values, 12);

	cout << "First try block:\n";
	try
	{
		int i = 0;
		cout << "Year:" << yearSales.Year() << endl;

		for (i = 0; i < 12; i ++)
		{
			cout << "Moths: " << i + 1 <<":"<<yearSales[i] << endl;
		}

		cout<<yearSales[i];
		cout << "End of try block" << endl;
	}
	catch (CSales::bad_index& e)
	{
		cout << e.what();
		cout << "bad_index: " << e.ib_val() << endl;
	}
}

#include <exception>
using namespace std;

void MyQuit()
{
	cout << "Terminating due to uncaught exception" << endl;
	exit(5);
}


// 未捕捉异常不会导致程序立即异常终止，修改terminate调用的函数。
void SetTerminate_Test()
{
	int i = 10, j = 0, result;
	set_terminate(MyQuit);

	try
	{
		try
		{
			int k = i / j;
			cout << "ddd2222" << endl;
		}
		catch (std::exception & e)
		{
			cout << "kkkkkk" << endl;
			cout << e.what() << ENDL;
		}
		cout << "aaaaa" << endl;
	}
	catch (...)
	{
		cout << "...." << endl;
	}
	cout << "dddadfa" << endl;
	//try
	//{
	//	if (j == 0)
	//		throw "Divide by zero!";   //抛出异常，由terminate函数捕获，
	//	else
	//		result = i / j;
	//}
	//catch (int) //不会由该函数捕获
	//{
	//	cout << "捕获到整型异常.\n";
	//}
	//catch (std::exception & e)
	//{
	//	cout << e.what() << endl;
	//}
	//catch (char*)
	//{
	//	cout << "捕捉到字符串异常.\n";
	//}
	//cout << "This should never print.\n";
}

// compile with: /EHa 捕获异步（结构化）异常和同步 (C++) 异常的异常处理模型。

void fail() {   // generates SE and attempts to catch it using catch(...)
	try {
		int i = 0, j = 1;
		j /= i;   // This will throw a SE (divide by zero).
	}
	catch (...) {   // catch block will only be executed under /EHa
		cout << "Caught an exception in catch(...)." << endl;
	}
}

void UseEHa_test() {
	__try {
		fail();
	}

	// __except will only catch an exception here
	__except (EXCEPTION_EXECUTE_HANDLER) {
		// if the exception was not caught by the catch(...) inside fail()
		cout << "An exception was caught in __except." << endl;
	}
}

#include "DbgFunc.h"

void SEFunc()
{
	// 这里的代码使用 结构化异常处理SEH的 结束处理程序
	__try
	{
		int x, y = 0;
		x = 5 / y;
		cout << "the code after Exception can run Under Release" << endl;
	}
	__finally
	{
		cout << "In Finally" << endl;
	}

	cout << "the code after try_finally can run under Release" << endl;
}

void HandleSEHAsCppException()
{
	CSEHException ex;
	try
	{
		SEFunc();
	}
	catch (CSE_Exception & e)
	{
		cout << "Caught a _try exception with SE_Exception\n";
	}
}

class CRole
{
public:
	CRole(){}
	~CRole(){ cout << "CRole Destroy" << endl; }

private:

};

struct CommonProfessionData{
	wstring AvatarpageUrl;
	set<wstring/*customAvaNames*/>           setAvaName;
	map<wstring/*rolename+server*/, CRole*>  mapAllRole;

	~CommonProfessionData()
	{
		map<wstring/*rolename+server*/, CRole*>::iterator iter_roleMapEnd = mapAllRole.end();
		map<wstring/*rolename+server*/, CRole*>::iterator iter_roleMapBegin = mapAllRole.begin();
		for (; iter_roleMapBegin != iter_roleMapEnd; iter_roleMapBegin++)
		{
			delete(iter_roleMapBegin->second);
		}
	}
};

struct DwAccountData{
	map<wstring/*bigprofession*/, CommonProfessionData*>  mapComProData;

	~DwAccountData()
	{
		map<wstring/*bigprofession*/, CommonProfessionData*>::iterator iter_proMapEnd = mapComProData.end();
		map<wstring/*bigprofession*/, CommonProfessionData*>::iterator iter_proMapBegin = mapComProData.begin();
		for (; iter_proMapBegin != iter_proMapEnd; iter_proMapBegin++)
		{
			delete (iter_proMapBegin->second);
		}
	}
};

void TestStruct()
{
	DwAccountData* pB = new DwAccountData;
	
	CRole* pRole = new CRole;
	CommonProfessionData* pComProData = new CommonProfessionData;
	pComProData->mapAllRole[L"dddd"] = pRole;

	pB->mapComProData[L"dddsw3e"] = pComProData;
	delete pB;
}

#include "Md5.h"

void Use_MD5_Test()
{
	cout << HashHelper::MD5HashString("我是谁") << endl;
	cout << HashHelper::MD5HashString("Who am I") << endl;
}

char* getcharBuffer()
{
	return "6969,3p3p";
}

void changeto4p(char* buffer)
{
	while (*buffer)
	{
		if (*buffer == '3')
		{
			*buffer = '4';
		}
		buffer++;
	}
}

void  Test()
{
	printf("%s\n", "Any Key continue...");
	getchar();
	char* str = getcharBuffer();
	changeto4p(str);
	printf("%s\n", str);
}

#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

#include <Urlmon.h>
#pragma comment(lib, "Urlmon.lib")

void GetPublicNetIP()
{
	HINTERNET hSession =  InternetOpenA("HttpOpen", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		HINTERNET hRequest =  InternetOpenUrlA(hSession, "http://www.ip138.com/ip2city.asp", NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (hRequest != NULL)
		{
			char buff[1024] = { 0 };
			DWORD dwBytesRead = 0;
			if (InternetReadFile(hRequest, buff, 1024, &dwBytesRead) && dwBytesRead <= 1024)
			{

				string publicnetIp;
				publicnetIp.resize(32);
				char* str_ip = strstr(buff, "[");
				if (str_ip !=  NULL)
				{
					sscanf_s(str_ip + 1, "%[^]]", &publicnetIp[0], 32);
					printf("IP: %s", publicnetIp.c_str());
				}
			}
			InternetCloseHandle(hRequest);
		}

		InternetCloseHandle(hSession);
	}

	string Inernet_ip;
	Inernet_ip.resize(32);
	TCHAR szTempPath[_MAX_PATH] = { 0 }, szTempFile[MAX_PATH] = { 0 };
	std::string buffer;
	GetTempPath(MAX_PATH, szTempPath);
	UINT nResult = GetTempFileName(szTempPath, _T("~ex"), 0, szTempFile);
	int ret = URLDownloadToFile(NULL, _T("http://www.ip138.com/ip2city.asp"), szTempFile, BINDF_GETNEWESTVERSION, NULL);
	if (ret == S_FALSE) return;

	FILE *fp;
	if (_wfopen_s(&fp, szTempFile, _T("rb")) != 0){
		return ;
	}
	fseek(fp, 0, SEEK_END);//得到文件大小  
	int ilength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (ilength > 0)
	{
		buffer.resize(ilength);
		fread(&buffer[0], sizeof(TCHAR), ilength, fp);
		fclose(fp);
		DeleteFile(_T("ip.ini"));

		char* str_ip = strstr(&buffer[0], "[");
		if (str_ip != NULL)
		{
			sscanf_s(str_ip + 1, "%[^]]", &Inernet_ip[0], 32);
		}

	}
}

//#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */

int GetMACAddress()
{

	/* Declare and initialize variables */

	// It is possible for an adapter to have multiple
	// IPv4 addresses, gateways, and secondary WINS servers
	// assigned to the adapter. 
	//
	// Note that this sample code only prints out the 
	// first entry for the IP address/mask, and gateway, and
	// the primary and secondary WINS server for each adapter. 

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return 1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return 1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			printf("\tComboIndex: \t5d\n", pAdapter->ComboIndex);
			printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
			printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
			printf("\tAdapter Addr: \t");
			for (i = 0; i < pAdapter->AddressLength; i++) {
				if (i == (pAdapter->AddressLength - 1))
					printf("%.2X\n", (int)pAdapter->Address[i]);
				else
					printf("%.2X-", (int)pAdapter->Address[i]);
			}
			printf("\tIndex: \t%d\n", pAdapter->Index);
			printf("\tType: \t");
			switch (pAdapter->Type) {
			case MIB_IF_TYPE_OTHER:
				printf("Other\n");
				break;
			case MIB_IF_TYPE_ETHERNET:
				printf("Ethernet\n");
				break;
			case MIB_IF_TYPE_TOKENRING:
				printf("Token Ring\n");
				break;
			case MIB_IF_TYPE_FDDI:
				printf("FDDI\n");
				break;
			case MIB_IF_TYPE_PPP:
				printf("PPP\n");
				break;
			case MIB_IF_TYPE_LOOPBACK:
				printf("Lookback\n");
				break;
			case MIB_IF_TYPE_SLIP:
				printf("Slip\n");
				break;
			default:
				printf("Unknown type %ld\n", pAdapter->Type);
				break;
			}

			printf("\tIP Address: \t%s\n",
				pAdapter->IpAddressList.IpAddress.String);
			printf("\tIP Mask: \t%s\n", pAdapter->IpAddressList.IpMask.String);

			printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
			printf("\t***\n");

			if (pAdapter->DhcpEnabled) {
				printf("\tDHCP Enabled: Yes\n");
				printf("\t  DHCP Server: \t%s\n",
					pAdapter->DhcpServer.IpAddress.String);

				printf("\t  Lease Obtained: ");
				/* Display local time */
				error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseObtained);
				if (error)
					printf("Invalid Argument to _localtime32_s\n");
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						printf("Invalid Argument to asctime_s\n");
					else
						/* asctime_s returns the string terminated by \n\0 */
						printf("%s", buffer);
				}

				printf("\t  Lease Expires:  ");
				error = _localtime32_s(&newtime, (__time32_t*)&pAdapter->LeaseExpires);
				if (error)
					printf("Invalid Argument to _localtime32_s\n");
				else {
					// Convert to an ASCII representation 
					error = asctime_s(buffer, 32, &newtime);
					if (error)
						printf("Invalid Argument to asctime_s\n");
					else
						/* asctime_s returns the string terminated by \n\0 */
						printf("%s", buffer);
				}
			}
			else
				printf("\tDHCP Enabled: No\n");

			if (pAdapter->HaveWins) {
				printf("\tHave Wins: Yes\n");
				printf("\t  Primary Wins Server:    %s\n",
					pAdapter->PrimaryWinsServer.IpAddress.String);
				printf("\t  Secondary Wins Server:  %s\n",
					pAdapter->SecondaryWinsServer.IpAddress.String);
			}
			else
				printf("\tHave Wins: No\n");
			pAdapter = pAdapter->Next;
			printf("\n");
		}
	}
	else {
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return 0;
}


void CompareTime_Test()
{
	string strtime1 = "2015-05-11_11:41";
	string strtime2 = "2015-05-11_12:41";

	string strtimeVal = "2015-05-11_11:42";
	
	if (strtime1 <= strtimeVal && strtimeVal <= strtime2)
	{
		cout << "time valid" << endl;
	}
	
}

#include "FileHelper.h"

void FileHelp_Test()
{
	wstring szFielName;/*= L"E:\\工作区\\mine\\taocode\\NewKnowledge\\NewKnowledge_Test\\testfunc.h";
	string szBuffer;
	FileHelper::MyReadFile(szFielName.c_str(), szBuffer);
	cout << szBuffer;*/

	szFielName = L"C:\\test.txt";
	char szBuffer[1024] = {"123456799999999"};
	

	FileHelper::MyWriteFile(szFielName.c_str(), &szBuffer[0], strlen(szBuffer));
}


#include "ProcessFunc.h"

void ProcessFunc_Test()
{
	if (processfunc::IsProcessExist(L"360rp.exe"))
		cout << "360rp.exe 存在" << endl;

	if (!processfunc::IsProcessExist(L"ddd"))
		cout << "not exist" << endl;
}

#include "BeautyOfProgram.h"

void  DataStructHelper_Test()
{
	vector<int> vct_array;
	vct_array.push_back(4);
	vct_array.push_back(-3);
	vct_array.push_back(5);
	vct_array.push_back(-2);
	vct_array.push_back(-1);
	vct_array.push_back(2);
	vct_array.push_back(6);
	vct_array.push_back(-2);
	
	printf("测试： 子序列最大和 方法1: %d\n", MaxSubSum1(vct_array));
	printf("测试： 子序列最大和 方法2：%d\n", MaxSubSum2(vct_array));
	printf("测试： 子序列最大和 方法3：%d\n", MaxSubSum3(vct_array));
	printf("测试： 子序列最大和 方法4：%d\n", MaxSubSum4(vct_array));

} 

#include "SortHelper.h"
void Sort_Test()
{
	{
		printf("测试内部排序： 直接插入排序\n");
		vector<int> vct_array;
		vct_array.push_back(81);
		vct_array.push_back(94);
		vct_array.push_back(11);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(95);
		vct_array.push_back(28);
		vct_array.push_back(58);
		insertionSort<int>(vct_array);
		for each (int var in vct_array)
		{
			printf("%d ", var);
		}
		printf("\n");
	}

	{
			printf("测试内部排序： 折半插入排序\n");
			vector<int> vct_array;
			vct_array.push_back(81);
			vct_array.push_back(94);
			vct_array.push_back(11);
			vct_array.push_back(12);
			vct_array.push_back(35);
			vct_array.push_back(17);
			vct_array.push_back(95);
			vct_array.push_back(28);
			vct_array.push_back(58);
			binaryInsertSort<int>(vct_array);
			for each (int var in vct_array)
			{
				printf("%d ", var);
			}
			printf("\n");
	}


	{
	printf("测试内部排序： 希尔排序\n");
	vector<int> vct_array;
	vct_array.push_back(81);
	vct_array.push_back(94);
	vct_array.push_back(11);
	vct_array.push_back(12);
	vct_array.push_back(35);
	vct_array.push_back(17);
	vct_array.push_back(95);
	vct_array.push_back(28);
	vct_array.push_back(58);
	shellSort<int>(vct_array);
	for each (int var in vct_array)
		printf("%d ", var);
	printf("\n");
}

		{
			printf("测试内部排序： 冒泡\n");
			vector<int> vct_array;
			vct_array.push_back(81);
			vct_array.push_back(94);
			vct_array.push_back(11);
			vct_array.push_back(12);
			vct_array.push_back(35);
			vct_array.push_back(17);
			vct_array.push_back(95);
			vct_array.push_back(28);
			vct_array.push_back(58);

			bubbleSort<int>(vct_array);
			for each (int var in vct_array)
			{
				printf("%d ", var);
			}
			printf("\n");
		}

				{
					printf("测试内部排序： 简单选择排序\n");
					vector<int> vct_array;
					vct_array.push_back(81);
					vct_array.push_back(94);
					vct_array.push_back(11);
					vct_array.push_back(12);
					vct_array.push_back(35);
					vct_array.push_back(17);
					vct_array.push_back(95);
					vct_array.push_back(28);
					vct_array.push_back(58);

					selectSort<int>(vct_array);
					for each (int var in vct_array)
					{
						printf("%d ", var);
					}
					printf("\n");
				}

	{
		printf("测试内部排序： 堆排序\n");
		vector<int> vct_array;
		vct_array.push_back(81);
		vct_array.push_back(94);
		vct_array.push_back(11);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(95);
		vct_array.push_back(28);
		vct_array.push_back(58);
		heapSort<int>(vct_array);
		for each (int var in vct_array)
		{
			printf("%d ", var);
		}
		printf("\n");
	}

	{
		printf("测试内部排序： 归并排序\n");
		vector<int> vct_array;
		vct_array.push_back(81);
		vct_array.push_back(94);
		vct_array.push_back(11);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(95);
		vct_array.push_back(28);
		vct_array.push_back(58);
		mergeSort<int>(vct_array);
		for each (int var in vct_array)
		{
			printf("%d ", var);
		}
		printf("\n");
	}
	{
		printf("测试内部排序： 快速排序\n");
		vector<int> vct_array;
		vct_array.push_back(81);
		vct_array.push_back(94);
		vct_array.push_back(11);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(95);
		vct_array.push_back(28);
		vct_array.push_back(58);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);
		vct_array.push_back(12);
		vct_array.push_back(35);
		vct_array.push_back(17);

		quicksort<int>(vct_array);
		for each (int var in vct_array)
		{
			printf("%d ", var);
		}
		printf("\n");
	}

	{
		printf("测试内部排序： 间接排序\n");
		vector<int> vct_array;
		vct_array.push_back(81);
		vct_array.push_back(94);
		vct_array.push_back(11);

		largeObjectSort<int>(vct_array);
		for each (int var in vct_array)
		{
			printf("%d ", var);
		}
		printf("\n");
	}
}