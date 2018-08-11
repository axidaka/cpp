#include "stdafx.h"
#include "ShareMem.h"
#include <tchar.h>
#include "ToolFunc.h"
#include <cassert>
#include "Logger.h"

CShareMem::CShareMem()
{

}

CShareMem::~CShareMem()
{
	if (m_pMapView != NULL)
	{
		UnmapViewOfFile(m_pMapView);
		m_pMapView = NULL;
	}

	if (m_hFileMap != NULL)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}
}

HRESULT CShareMem::Create(const std::wstring& strName, DWORD dwSize)
{
	m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, dwSize + sizeof(DWORD), strName.c_str());  ///<前面四个字节存放总长度

	if (m_hFileMap != NULL)
	{
		HRESULT hr = S_OK;

		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			_tprintf(_T("FileMapping already exits \n"));
			hr  = S_FALSE;
		}

		m_pMapView = (PBYTE)MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if (m_pMapView == NULL)
		{
			CloseHandle(m_hFileMap);
			m_hFileMap = NULL;
			_tprintf(_T("Can't MapViewOfFile(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
			hr = E_FAIL;
		}
		
		*(DWORD*)m_pMapView = dwSize + sizeof(DWORD);

		return hr;
		
	}
	else
	{
		_tprintf(_T("Can't create file mapping(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return E_FAIL;
	}
}

HRESULT CShareMem::Open(const std::wstring& strName)
{
	m_hFileMap = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
		FALSE, strName.c_str());
	if (m_hFileMap != NULL)
	{
		m_pMapView = (PBYTE)MapViewOfFile(m_hFileMap,
			FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		
		if (m_pMapView != NULL)
		{
			return S_OK;
		}

		_tprintf(_T("Can't MapViewOfFile(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
	}
	else
	{
		_tprintf(_T("Can't open file mapping(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
	}
	return S_OK;
}

HRESULT CShareMem::WriteMem(PBYTE const pbData, DWORD dwLength, DWORD dwOffset /*= 0*/)
{
	DWORD dwTotalSize = *(DWORD*)m_pMapView;
	if (dwOffset + dwLength > dwTotalSize)
	{
		assert(false);
		return E_OUTOFMEMORY;
	}

	::CopyMemory(m_pMapView + dwOffset, pbData, dwLength);

	return S_OK;
}

HRESULT CShareMem::ReadMem(PBYTE pbData, DWORD dwLength, DWORD dwOffset /*= 0*/)
{
	DWORD dwTotalSize = *(DWORD*)m_pMapView;
	if (dwOffset + dwLength > dwTotalSize)
	{
		assert(false);
		return E_OUTOFMEMORY;
	}
	
	::CopyMemory(pbData, m_pMapView + dwOffset, dwLength);

	return S_OK;
}
