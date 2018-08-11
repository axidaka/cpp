#pragma once

namespace processfunc
{
	std::wstring GetFileByProcessId(DWORD processId);
	BOOL CloseProcessByFile(const std::wstring &filepath);
	BOOL IsProcessExist(const std::wstring &name);
	void CloseProcess(const std::wstring &name);
	BOOL CreateProcess(const std::wstring &strCmd, const std::wstring &strDir);
}