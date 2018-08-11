#pragma once

#include <string>

namespace ToolFunc
{
	class CRootDirectory
	{
	public:

		~CRootDirectory();
		CRootDirectory();

		void Init(const std::wstring& directory);

		std::wstring GetRootDirectory() const;

		std::wstring GetSubDirectory(const std::wstring& sub_dir, bool create = false) const;

		std::wstring GetSubDirFilePath(const std::wstring& sub_dir, const std::wstring& file_name) const;

		std::wstring GetFullPath(const std::wstring& relative_path) const;


	private:

		std::wstring root_dir;

	};

	extern CRootDirectory Self;
}
