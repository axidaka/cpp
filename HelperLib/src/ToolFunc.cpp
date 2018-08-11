#include "stdafx.h"
#include "ToolFunc.h"
#include <gdiplus.h>
using namespace Gdiplus;

namespace ToolFunc
{
	tstring FormatErrStr(const DWORD dwError)
	{
		HLOCAL hlocal = NULL;
		DWORD systemLocal = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
		BOOL bRtn = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL, dwError, systemLocal, (PTSTR)&hlocal, 0, NULL);
		if (!bRtn)
		{
			HMODULE hDll = LoadLibraryEx(_T("netmsg.dll"), NULL, DONT_RESOLVE_DLL_REFERENCES);
			if (hDll != NULL)
			{
				bRtn = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					hDll, dwError, systemLocal, (PTSTR)&hlocal, 0, NULL);
				FreeLibrary(hDll);
			}
		}
		if (!bRtn || (hlocal == NULL))
		{
			return _T("δ֪�Ĵ����");
		}

		tstring strRtn = (const TCHAR*)LocalLock(hlocal);
		LocalUnlock(hlocal);

		return strRtn;
	}

	std::wstring GetSubDirectory(const std::wstring& parent, const std::wstring& dir, bool create)
	{
		std::wstring path;
		if (parent.empty() || (*parent.rbegin() == L'\\'))
		{
			path = parent + dir;
		}
		else
		{
			path = parent + L"\\" + dir;
		}

		if (create && !::PathFileExistsW(path.c_str()))
			CreateDirectoryW(path.c_str(), NULL);

		return path;
	}

	std::wstring GetProgramDirectory()
	{
		WCHAR szBuffer[MAX_PATH];
		::GetModuleFileName(NULL, szBuffer, MAX_PATH);
		std::wstring path = szBuffer;
		std::wstring::size_type enpos = path.find_last_of('\\');
		path = path.substr(0, enpos);

		return path;
	}

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT num = 0;                     // number of image encoders   
		UINT size = 0;                   // size of the image encoder array in bytes   
		ImageCodecInfo* pImageCodecInfo = NULL;
		GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;     //   Failure   

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;     //   Failure   

		GetImageEncoders(num, size, pImageCodecInfo);
		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;     //   Success   
			}
		}
		free(pImageCodecInfo);
		return -1;     //   Failure   
	}

	bool CreateMultipleDirectory(std::wstring path)
	{
		if ((path[path.size() - 1] != L'\\') && (path[path.size() - 1] != L'/'))
		{
			path = path + L"\\";
		}

		if (PathIsDirectory(path.c_str()) == FILE_ATTRIBUTE_DIRECTORY)
		{
			return true;
		}

		std::wstring strDir = path;//���Ҫ������Ŀ¼�ַ���
		std::vector<std::wstring> vPath;//���ÿһ��Ŀ¼�ַ���
		std::wstring strTemp;//һ����ʱ����,���Ŀ¼�ַ���
		bool bSuccess = false;//�ɹ���־
		std::wstring::const_iterator sIter;//�����ַ���������
		//����Ҫ�������ַ���
		for (sIter = strDir.begin(); sIter != strDir.end(); sIter++) {
			if ((*sIter != '\\') && (*sIter != '/')) {//�����ǰ�ַ�����'\\','/'
				strTemp += (*sIter);
			}
			else {//�����ǰ�ַ���'\\'
				vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
				strTemp += '\\';
			}
		}

		//�������Ŀ¼������,����ÿ��Ŀ¼
		std::vector<std::wstring>::const_iterator vIter;
		for (vIter = vPath.begin(); vIter != vPath.end(); ++vIter)
		{
			bSuccess = CreateDirectory((*vIter).c_str(), NULL) ? true : false;
		}

		return bSuccess;
	}
}