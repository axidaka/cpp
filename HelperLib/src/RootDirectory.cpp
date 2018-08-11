#include "stdafx.h"
#include "RootDirectory.h"
#include "ToolFunc.h"
#include <assert.h>

namespace ToolFunc
{
	CRootDirectory Self;

	CRootDirectory::CRootDirectory()
	{
	}


	CRootDirectory::~CRootDirectory()
	{
	}

	void CRootDirectory::Init(const std::wstring& directory)
	{
		assert(!directory.empty() && PathFileExistsW(directory.c_str()));

		root_dir = directory;
	}

	std::wstring CRootDirectory::GetRootDirectory() const
	{
		return root_dir;
	}

	std::wstring CRootDirectory::GetSubDirectory(const std::wstring& sub_dir, bool create) const
	{
		assert(!root_dir.empty() && PathFileExistsW(root_dir.c_str()));

		return ToolFunc::GetSubDirectory(root_dir, sub_dir, create);
	}

	std::wstring CRootDirectory::GetSubDirFilePath(const std::wstring& sub_dir, const std::wstring& file_name) const
	{
		assert(!root_dir.empty() && PathFileExistsW(root_dir.c_str()));

		return ToolFunc::GetSubDirectory(GetSubDirectory(sub_dir, true), file_name, false);
	}

	std::wstring CRootDirectory::GetFullPath(const std::wstring& relative_path) const
	{
		assert(!root_dir.empty() && PathFileExistsW(root_dir.c_str()));

		return ToolFunc::GetSubDirectory(root_dir, relative_path, false);
	}
}
