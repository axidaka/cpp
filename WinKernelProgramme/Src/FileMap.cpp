#include "stdafx.h"
#include "FileMap.h"
#include <windows.h>
#include "RootDirectory.h"
#include <string>
using namespace std;

void FileMapping_Test()
{
	std::wstring filepath = ToolFunc::Self.GetFullPath(L"MMFTest.dat");
	HANDLE hfile = CreateFile(filepath.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);

	HANDLE hfilemap = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, 100, NULL);

	CloseHandle(hfilemap);
	CloseHandle(hfile);

}

void FileMapView_Test()
{
	// Open the file that we want to map
	std::wstring filepath = ToolFunc::Self.GetFullPath(L"MMFTest.dat");
	HANDLE hfile = CreateFile(filepath.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//Create a file-mapping object for the file
	HANDLE hFileMapping = CreateFileMapping(hfile, NULL, PAGE_WRITECOPY,
		0, 0, NULL);

	// Map a copy-on-write view of the file: the system will commit \
	// enough physical storage from the paging file to accommodate 
	// the entire file. All pages in the view will initially have 
	// Page_WrietCopy access
	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_COPY,
		0, 0, 0);

	//Read a byte from the mapped view
	BYTE bSomeByte = pbFile[0];
	// When reading, the system does not touch the committed pages in
	// the paging file. The page keeps its PAGE_WRITECOPY attribute

	// Write a byte to the mapped view
	pbFile[0] = 0;
	//When writing for the first time, the system grabs a commiteted
	//page from the pagging file, copied the original contents of the 
	//page at the accessed memory address, and maps the new page
	//(the copy)into the process's address space. the new page has 
	//an attribute of PAGE_READWRITE

	//Write another byte to the mapped view
	pbFile[1] = 1;
	// Because this byte is now in a PAGE_READWRITE page,1 the system
	// simply writes the bytes to the page (backed by the paging file)

	UnmapViewOfFile(pbFile);

	CloseHandle(hFileMapping);
	CloseHandle(hfile);
}

void FileMapView_Test2()
{
	// Open the file that we want to map
	std::wstring filepath = ToolFunc::Self.GetFullPath(L"MMFTest.dat");
	HANDLE hfile = CreateFile(filepath.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	//Create a file-mapping object for the file
	HANDLE hFileMapping = CreateFileMapping(hfile, NULL, PAGE_READWRITE,
		0, 0, NULL);

	// Map a copy-on-write view of the file: the system will commit \
			// enough physical storage from the paging file to accommodate 
	// the entire file. All pages in the view will initially have 
	// Page_WrietCopy access
	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE,
		0, 0, 0);

	//Read a byte from the mapped view
	BYTE bSomeByte = pbFile[0];
	// When reading, the system does not touch the committed pages in
	// the paging file. The page keeps its PAGE_WRITECOPY attribute

	// Write a byte to the mapped view
	pbFile[0] = 'a';
	//When writing for the first time, the system grabs a commiteted
	//page from the pagging file, copied the original contents of the 
	//page at the accessed memory address, and maps the new page
	//(the copy)into the process's address space. the new page has 
	//an attribute of PAGE_READWRITE

	//Write another byte to the mapped view
	pbFile[1] = 'b';
	// Because this byte is now in a PAGE_READWRITE page,1 the system
	// simply writes the bytes to the page (backed by the paging file)

	UnmapViewOfFile(pbFile);

	CloseHandle(hFileMapping);
	CloseHandle(hfile);
}


#include <commdlg.h>
#include <stdio.h>
#include <tchar.h>
#include "ToolFunc.h"
#include "StringHelper.h"

void FileReverse()
{
	TCHAR szPathName[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.lpstrFile = szPathName;
	ofn.lpstrFile[0] = 0;
	ofn.nMaxFile = _countof(szPathName);
	ofn.lpstrTitle = TEXT("Select file for reversing");
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
	{
		return;
	}
	_tprintf(TEXT("Select file:%s"), ofn.lpstrFile);

	tstring strNewFile = ToolFunc::Self.GetFullPath(L"FileRev.dat");
	if (!CopyFile(ofn.lpstrFile, strNewFile.c_str(), FALSE))
	{
		_tprintf(_T("Copy file failed(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return;
	}

	HANDLE hFile = CreateFile(strNewFile.c_str(), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File could not be open(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return;
	}

	// Get the size of the file( assume the whole file can be mapped)
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	// Create the file-mapping object. The file-mapping object is 1 character 
	// bigger than the file size so that a zero character can be placed at the 
	// end of the file to terminate the string (file). Because I don't yet know
	// if the file contains ANSI or Unicode characters, I assume worst case
	// and add the size of a WCHAR instead of CHAR.
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		0, dwFileSize + sizeof(WCHAR), NULL);
	if (hFileMap == NULL)
	{
		_tprintf(_T("File Map could not be opened(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		CloseHandle(hFile);
		return;
	}

	// Get the address where the first byte of the file si mapped the memory
	PVOID pvFile = MapViewOfFile(hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (pvFile == NULL)
	{
		_tprintf(_T("Could not map view of the file(%s)"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return;
	}

	// Does the buffer contain ANSI or Unicode?

	// For all the file manipulations below, we explicitly use ANSI 
	// functions because we are processing an ANSI file.

	// Put a zero character at the very end of the file.
	PSTR pchANSI = (PSTR)pvFile;
	pchANSI[dwFileSize / sizeof(CHAR)] = 0;

	// Reverse the contents of the file.
	_strrev(pchANSI);

	// Convert all "\n\r" combinations back to "\r\n" to 
	// preserve the normal end-of-line sequence.
	pchANSI = strstr(pchANSI, "\n\r"); // Find first "\r\n".

	while (pchANSI != NULL) {
		// We have found an occurrence....
		*pchANSI++ = '\r';   // Change '\n' to '\r'.
		*pchANSI++ = '\n';   // Change '\r' to '\n'.
		pchANSI = strstr(pchANSI, "\n\r"); // Find the next occurrence.
	}

	// Clean up everything before exiting.
	UnmapViewOfFile(pvFile);
	CloseHandle(hFileMap);

	// Remove trailing zero character added earlier.
	SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
	SetEndOfFile(hFile);
	CloseHandle(hFile);


	// Spawn Notepad to see the fruits of our labors.
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	tstring strCmdline = StringHelper::Format(_T("Notepad %s"), strNewFile.c_str());

	if (CreateProcess(NULL, (LPTSTR)strCmdline.c_str(),
		NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {

		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}

void FileMapViewTwice()
{
	// Open an exiting file it must be bigger than 64KB
	wstring filepath = ToolFunc::Self.GetFullPath(L"biggerFile.dat");

	HANDLE hFile = CreateFile(filepath.c_str(), GENERIC_WRITE | GENERIC_READ,
		0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// create a file-mapping object backed by the data file
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE,
		0, 0, NULL);

	// map a view of the whole file into our address space
	PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMap, FILE_MAP_WRITE,
		0, 0, 0);

	// map a view of the file(starting 64KB in)into our adddress space
	PBYTE pbFile2 = (PBYTE)MapViewOfFile(hFileMap, FILE_MAP_WRITE,
		0, 65536, 0);

	if ((pbFile + 65536) == pbFile2)
	{
		_tprintf(_T("we are running under window98\n"));
	}
	else
	{
		_tprintf(_T("we are running under window2000\n"));
	}

	UnmapViewOfFile(pbFile);
	UnmapViewOfFile(pbFile2);

	CloseHandle(hFileMap);
	CloseHandle(hFile);
}

void ShareMem_Test()
{
	_tprintf(_T("Memory Mapping File Share Test\n"));
	_tprintf(_T("Input 1 to Create Mapping of Data\n"));
	_tprintf(_T("Input 2 to Close Mapping of Data\n"));
	_tprintf(_T("Open Mapping and Get Data\n"));
	_tprintf(_T("Input Ctrl + Z to exit\n"));

	int nInput = 0;
	while (cin>>nInput)
	{
		switch (nInput)
		{
		case 1:
			CreateMemMapFile();
			break;
		case 2:
			CloseMemMapFile();
			break;
		case 3:
			OpenMemMapFile();
			break;
		default:
			break;
		}

		_tprintf(_T("Input Again\n"));
	}

}

// Handle fo the open memory-mapped file
static HANDLE s_hFileMap = NULL;

bool CreateMemMapFile()
{
	// Create a paging file-backed MMF to contain the text
	// The MMF is 4KB at most and is named MMFSharedData
	s_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
		PAGE_READWRITE, 0, 4 * 1024, _T("MMFSharedData"));
	if (s_hFileMap != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			_tprintf(_T("Mapping already exits \n"));
			CloseHandle(s_hFileMap);
			s_hFileMap = NULL;
			return false;
		}
		else
		{
			// File mapping created successfully

			//Map a view of the file into the address space
			PBYTE pView = (PBYTE)MapViewOfFile(s_hFileMap,
				FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

			if (pView != NULL)
			{
				_tprintf(_T("Input the text which is putted into the MMFShare:"));
				std::wstring strInputText;
				wcin >> strInputText;
				// Put Text into MMF
				::CopyMemory(pView, strInputText.c_str(), strInputText.size()*sizeof(WCHAR));

				// Protect the MMF storage by unmapping it
				UnmapViewOfFile(pView);

				return true;
			}
		}
	}
	else
	{
		_tprintf(_T("Can't create file mapping(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return false;
	}
}

bool CloseMemMapFile()
{
	if (s_hFileMap != NULL)
	{
		CloseHandle(s_hFileMap);
		_tprintf(_T("Close Mem-mapped file\n"));
		return true;
	}
	else
	{
		_tprintf(_T("MemMapFile isn't created yet\n"));
		return false;
	}
}

bool OpenMemMapFile()
{
	// See if a Mem-mapped file named MMFSharedData already exits
	HANDLE hFileMapT = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE,
		FALSE, _T("MMFSharedData"));
	if (hFileMapT != NULL)
	{
		// The MMF does exits map it into the process's address space
		PBYTE pView = (PBYTE)MapViewOfFile(hFileMapT, FILE_MAP_WRITE | FILE_MAP_READ,
			0, 0, 0);
		if (pView != NULL)
		{
			_tprintf(_T("MMFSharedData:%s\n"), (LPTSTR)pView);
			UnmapViewOfFile(pView);
		}
		else
		{
			_tprintf(_T("Can't map view(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		}

		CloseHandle(hFileMapT);
	}
	else
	{
		_tprintf(_T("Can't open mapping(%s)\n"), ToolFunc::FormatErrStr(GetLastError()).c_str());
		return false;
	}
}

