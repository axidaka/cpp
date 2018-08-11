#pragma once

#include <wtypes.h>

#ifdef UNICODE
#define ToUpper   ToUpperW
#define ToLower   ToLowerW
#define Format	  FormatW
#else
#define ToUpper   ToUpperA
#define ToLower   ToLowerA
#define Format	  FormatA


#endif // !UNICODE
namespace StringHelper
{
	std::wstring ToUpperW(const std::wstring& str);
	std::wstring ToLowerW(const std::wstring& str);

    std::string ToUpperA(const std::string& str);
    std::string ToLowerA(const std::string& str);

	std::wstring FormatW(const wchar_t* format, ...);
    std::string FormatA(const char* format, ...);

	std::wstring SubString(const std::wstring& str, std::wstring::size_type size = 0);
	std::wstring LeaveString(std::wstring str, std::wstring::size_type size);
	int   GetBytesSize(const std::wstring &str);
	bool isChineseChar(const wchar_t wch);

	//中英文字定长分行处理，处理部分首字符不能为部分标点的情况
	void StringSplitLine(std::wstring w_input, std::vector<std::wstring> &outstr, int howlongth);

	std::wstring Bin2Hex(const unsigned char* bin, int len);
	int Hex2Bin(const std::wstring& hex, unsigned char* buf, int len);
	std::vector<unsigned char> Hex2Bin(const std::wstring& hex);

	std::wstring TrimSring(std::wstring sData, wchar_t flag);
	std::string TrimSring(std::string sData, char flag);

	int CompareIgnore(const std::string &op1, const std::string &op2);

	std::string StrW2A(const std::wstring &src, int to = CP_ACP);
	std::wstring StrA2W(const std::string &src, int from = CP_ACP);
	std::string StrA2A(const std::string &src, int from = CP_ACP, int to = CP_UTF8);
}



