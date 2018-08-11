#pragma once
#include <windows.h>
#include <string>

/************************************************************************/
//filename: ShareMem.h
//author  : BigSong
//function: ��װ�򵥵Ĺ����ڴ�������̰߳�ȫ
/************************************************************************/
class CShareMem
{
public:
	CShareMem();
	~CShareMem();

	HRESULT Create(const std::wstring& strName, DWORD dwSize);
	HRESULT Open(const std::wstring& strName);

	HRESULT WriteMem(PBYTE const pbData, DWORD dwLength, DWORD dwOffset = 0);
	HRESULT ReadMem(PBYTE pbData, DWORD dwLength, DWORD dwOffset = 0);

private:

	HANDLE m_hFileMap;
	PBYTE  m_pMapView;
};
