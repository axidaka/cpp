#include "stdafx.h"
#include "FileHelper.h"
#include "StringHelper.h"

namespace FileHelper
{
	BOOL FileHelper::MyReadFile(LPCTSTR szFileName, std::string &szBuffer)
	{
		szBuffer.clear();

		HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		DWORD dwFileSize = GetFileSize(hFile, NULL);
		char* pBuffer = new char[dwFileSize + 1];  // 最后一位为'\0,C-style 字符串的结束符
		DWORD dwReadSize = 0;

		if (!ReadFile(hFile, pBuffer, dwFileSize, &dwReadSize, NULL))
		{
			delete[]pBuffer;
			pBuffer = NULL;

			CloseHandle(hFile);
			return FALSE;
		}

		if (dwReadSize <= 0)
		{
			delete[]pBuffer;
			pBuffer = NULL;

			CloseHandle(hFile);
			return FALSE;
		}

		pBuffer[dwReadSize] = '\0';
		szBuffer = pBuffer;

		delete[]pBuffer;
		pBuffer = NULL;

		CloseHandle(hFile);
		return  TRUE;
	}

	BOOL MyWriteFile(LPCTSTR szFileName, const char *pBuffer, int nSize, bool bAppend/* = false*/)
	{
		HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, (bAppend? OPEN_ALWAYS: CREATE_ALWAYS),
			FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			DWORD dwErr = GetLastError();
			return FALSE;
		}

		if (pBuffer == NULL || nSize == 0)
		{
			return TRUE;
		}

		DWORD dwWriteSize = 0;
		
		if (bAppend)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
		}
		BOOL bRet = ::WriteFile(hFile, pBuffer, nSize, &dwWriteSize, NULL);
		
		CloseHandle(hFile);

		return bRet;
	}

	std::list<std::wstring> GetDirList(std::wstring searchPath)
	{
		std::list<std::wstring> dirList;

		if (searchPath[searchPath.size() - 1] != L'\\')
		{
			searchPath = searchPath + L"\\";
		}
		std::wstring filedir = searchPath + L"*.*";

		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(filedir.c_str(), &wfd);

		if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
			return dirList;

		do
		{
			if ((0 == lstrcmp(wfd.cFileName, L".")) || (0 == lstrcmp(wfd.cFileName, L"..")));
			else
			{
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirList.push_back(wfd.cFileName);
				}
			}
		} while (FindNextFile(hFind, &wfd));

		FindClose(hFind); // 关闭查找句柄

		return dirList;
	}

	std::list<std::wstring> GetFileList(std::wstring searchPath, wstring fileExtension)
	{
		std::list<std::wstring> fileList;

		if (searchPath[searchPath.size() - 1] != L'\\')
		{
			searchPath = searchPath + L"\\";
		}
		std::wstring filedir = searchPath + L"*.*";

		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(filedir.c_str(), &wfd);

		if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败
			return fileList;

		do
		{
			if ((0 == lstrcmp(wfd.cFileName, L".")) || (0 == lstrcmp(wfd.cFileName, L"..")));
			else
			{
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					std::list<std::wstring> tempList = GetFileList(searchPath + L"\\" + wfd.cFileName, fileExtension);
					fileList.insert(fileList.end(), tempList.begin(), tempList.end());
				}
				else
				{
					std::wstring filename = wfd.cFileName;
					std::wstring::size_type pos = filename.find_last_of(L".");
					if (pos != std::wstring::npos)
					{
						wstring filetype = filename.substr(pos, filename.size());
						filetype = StringHelper::ToUpper(filetype);
						fileExtension = StringHelper::ToUpper(fileExtension);
						if (filetype == fileExtension)
						{
							fileList.push_back(searchPath + wfd.cFileName);
						}
					}
				}
			}
		} while (FindNextFile(hFind, &wfd));

		FindClose(hFind); // 关闭查找句柄


		return fileList;
	}
}