#include "stdafx.h"
#include "D3DScreenShot.h"


//CD3DScreenShot::CD3DScreenShot()
//:m_pD3D(NULL)
//, m_pd3dDevice(NULL)
//, m_pd3dSurface(NULL)
//{
//}
//
//
//CD3DScreenShot::~CD3DScreenShot()
//{
//}
//
//bool CD3DScreenShot::Init()
//{
//	Uninit();
//
//	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
//	if (m_pD3D != NULL)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool CD3DScreenShot::Uninit()
//{
//	SafeRelease(m_pD3D);
//
//	return true;
//}
//
//bool CD3DScreenShot::ScreenShotByHwnd(HWND DesthWnd, bool bFullScreen /*= false*/)
//{
//	//TCHAR wndTexx[MAX_PATH] = { 0 };
//	//::GetWindowText(DesthWnd, wndTexx, MAX_PATH);
//	//_tcscat(wndTexx, _T("\n"));
//	//OutputDebugString(wndTexx);
//
//	memset(&m_displayMode, 0, sizeof(m_displayMode));
//	m_hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_displayMode);
//	if (FAILED(m_hr))
//	{
//		OutputDebugString(_T("GetAdapterDisplayMode failed\n"));
//		m_strLastErr = ToolFunc::FormatErrStr(GetLastError());
//		SafeRelease(m_pD3D);
//		return false;
//	}
//
//	cout << "AdapterDefault width :" << m_displayMode.Width<<endl;
//	cout << "AdapterDefault height :" << m_displayMode.Height<<endl;
//
//	RECT rtWnd;
//	::GetClientRect(DesthWnd, &rtWnd);
//
//	D3DPRESENT_PARAMETERS d3dParam;
//	memset(&d3dParam, 0, sizeof(d3dParam));
//
//	d3dParam.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
//	d3dParam.MultiSampleType = D3DMULTISAMPLE_NONE;
//	d3dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	//d3dParam.hDeviceWindow = DesthWnd;
//	//d3dParam.BackBufferCount = 1;
//	d3dParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
//
//	if (bFullScreen) //全屏模式即非窗口模式
//	{
//		cout << "全屏模式" << endl;
//		d3dParam.Windowed = FALSE;
//		d3dParam.BackBufferWidth = 800;
//		d3dParam.BackBufferHeight = 600;
//
//		d3dParam.BackBufferFormat = m_displayMode.Format;
//		d3dParam.FullScreen_RefreshRateInHz = m_displayMode.RefreshRate;
//		HWND hFocusWnd = ::FindWindow(L"地下城与勇士", L"地下城与勇士");
//		d3dParam.hDeviceWindow = hFocusWnd;
//		m_hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hFocusWnd,
//			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParam, &m_pd3dDevice);
//	}
//	else
//	{
//		cout << "窗口模式" << endl;
//		d3dParam.Windowed = TRUE;
//		d3dParam.BackBufferWidth = 0;
//		d3dParam.BackBufferHeight = 0;
//		d3dParam.BackBufferFormat = D3DFMT_UNKNOWN;
//		d3dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
//
//		d3dParam.hDeviceWindow = DesthWnd;
//
//		m_hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
//			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dParam, &m_pd3dDevice);
//	}
//
//	cout << "BackBufferWidth  :" << d3dParam.BackBufferWidth << endl;
//	cout << "BackBufferHeight height :" << d3dParam.BackBufferHeight << endl;
//	
//	if (FAILED(m_hr))
//	{
//		OutputDebugString(_T("CreateDevice failed\n"));
//		m_strLastErr = ToolFunc::FormatErrStr(GetLastError());
//		cout << "m_hr:" << DWORD(m_hr) << endl;
//		SafeRelease(m_pd3dDevice);
//		return false;
//	}
//
//	m_hr = m_pd3dDevice->CreateOffscreenPlainSurface(m_displayMode.Width, m_displayMode.Height,
//		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_pd3dSurface, NULL);
//	if (FAILED(m_hr))
//	{
//		OutputDebugString(_T("CreateOffscreenPlainSurface failed\n"));
//		m_strLastErr = ToolFunc::FormatErrStr(GetLastError());
//		SafeRelease(m_pd3dDevice);
//		SafeRelease(m_pd3dSurface);
//		return false;
//	}
//
//	m_pd3dDevice->GetFrontBufferData(0, m_pd3dSurface);
//
//	// area to capture
//// 	RECT* pRt = NULL;
//// 	WINDOWINFO windowInfo;
//// 	windowInfo.cbSize = sizeof(WINDOWINFO);
//// 	if (DesthWnd) // capture window
//// 	{
//// 		GetWindowInfo(DesthWnd, &windowInfo);
//// 		pRt = &windowInfo.rcWindow;
//// 	}
//	
//	RECT rtCapture;
//	::GetWindowRect(DesthWnd, &rtCapture);
//	m_hr = D3DXSaveSurfaceToFile(_T("D:\\Desktop.png"), D3DXIFF_PNG, m_pd3dSurface, NULL, &rtCapture);//s保存为png格式
//	if (FAILED(m_hr))
//	{
//		OutputDebugString(_T("D3DXSaveSurfaceToFile failed\n"));
//		m_strLastErr = ToolFunc::FormatErrStr(GetLastError());
//		SafeRelease(m_pd3dDevice);
//		SafeRelease(m_pd3dSurface);
//		return false;
//	}
//
//	SafeRelease(m_pd3dDevice);
//	SafeRelease(m_pd3dSurface);
//
//	return true;
//}

