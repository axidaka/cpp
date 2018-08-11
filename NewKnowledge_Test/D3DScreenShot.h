#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3d9helper.h>
#include <atltypes.h>
#include "stldefine.h"
#include "ToolFunc.h"


//#ifdef _DEBUG
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9d.lib")
//#else
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//#endif
//
//template <typename T>
//inline void SafeRelease(T* &p)
//{
//	if (NULL != p)
//	{
//		p->Release();
//		p = NULL;
//	}
//}
//
//class CD3DScreenShot
//{
//public:
//	CD3DScreenShot();
//	~CD3DScreenShot();
//
//	bool Init();
//
//	bool Uninit();
//
//	bool ScreenShotByHwnd(HWND DesthWnd, bool bFullScreen = false);
//
//	tstring GetLastErr(){ return m_strLastErr; }
//
//private:
//
//	IDirect3D9*         m_pD3D;
//	IDirect3DDevice9*   m_pd3dDevice;
//	IDirect3DSurface9*  m_pd3dSurface; 
//	D3DDISPLAYMODE      m_displayMode;
//
//	tstring m_strLastErr;
//	HRESULT m_hr;
//};


