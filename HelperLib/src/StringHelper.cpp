#include "StdAfx.h"
#include "StringHelper.h"
#include <algorithm>
#include "UnicodeConv.h"

namespace StringHelper
{
	template<class string_type, typename func>
	inline string_type ToConvert(const string_type& str, func op)
	{
		if (str.empty())
		{
			return str;
		}

		string_type strTmp;
		strTmp.resize(str.size());
		std::transform(str.begin(), str.end(), strTmp.begin(), op);
		return strTmp;
	}

	std::wstring ToUpperW(const std::wstring& str)
	{
		return ToConvert(str, ::towupper);
	}

	std::wstring ToLowerW(const std::wstring& str)
	{
		return ToConvert(str, ::towlower);
	}

	std::string ToUpperA(const std::string& str)
	{
		return ToConvert(str, ::toupper);
	}

	std::string ToLowerA(const std::string& str)
	{
		return ToConvert(str, ::tolower);
	}

	std::wstring FormatW(const wchar_t* format, ...)
	{
		std::wstring var_str;

		va_list ap;
		va_start(ap, format);
		int len = _vscwprintf(format, ap);
		if (len > 0)
		{
			std::vector<wchar_t> buf(len + 1);  //_vscwprintf didn't include 终止 null 字符
			vswprintf_s(&buf.front(), buf.size(), format, ap);
			var_str.assign(buf.begin(), buf.end() - 1);
		}
		va_end(ap);

		return var_str;
	}

	std::string FormatA(const char* format, ...)
	{
		std::string var_str;

		va_list ap;
		va_start(ap, format);
		int len = _vscprintf(format, ap);
		if (len > 0)
		{
			std::vector<char> buf(len + 1);
			vsprintf_s(&buf.front(), buf.size(), format, ap);
			var_str.assign(buf.begin(), buf.end() - 1);
		}
		va_end(ap);

		return var_str;
	}

	bool isChineseChar(const wchar_t wch)
	{
		if ((wch >= 0) && (wch <= 0x7F))
		{
			return false;
		}

		return true;
	}
	

	std::wstring SubString(const std::wstring& str, std::wstring::size_type size)
	{
		if (size <= 0)
		{
			return str;
		}
		std::string tempstr = ConvertUnicodeToMultiBytes(str);
		tempstr = tempstr.substr(0, size);
		std::wstring var_str = ConvertMultiBytesToUnicode(tempstr);
		return var_str;
	}

	int   GetBytesSize(const std::wstring &str)
	{
		return WideCharToMultiByte(GetACP(), 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
	}

	std::wstring LeaveString(std::wstring str, std::wstring::size_type size)
	{
		if (size <= 3)
			return str;

		if ((std::wstring::size_type)GetBytesSize(str) > size)
		{
			str = SubString(str, size - 3);
			if (str[str.size()] == L'\0')
			{
				str = str.substr(0, str.size() - 1) + L"...";
			}
			else
			{
				str = str + L"...";
			}
		}

		return str;
	}

	void StringSplitLine(std::wstring w_input, std::vector<std::wstring> &outstr, int len)
	{
		if (StringHelper::GetBytesSize(w_input) <= len * 2)
		{
			outstr.push_back(w_input);
			return;
		}
		//标点
		std::wstring punctuation_Z = L"。，、；：？！”）…—》";
		std::wstring punctuation_E = L".,;:?!)>]}";

		std::wstring output[200];
		int howlongth = len * 2;
		int longth = 0;
		for (int i = 0; i < (int)w_input.size(); ++i)
		{
			wchar_t inputCell = w_input[i];

			int a_len = StringHelper::GetBytesSize(output[longth]);
			if (a_len <= (howlongth - 3))
			{
				output[longth] += inputCell;
				//
				std::wstring s = output[longth];
				//
				continue;
			}
			else if (a_len == (howlongth - 2))
			{
				if ((i + 1) >= (int)w_input.size())
				{
					output[longth] += inputCell;
					break;
				}
				//如果后一字符是否为英文标点
				if (punctuation_E.find(w_input[i + 1]) != std::wstring::npos)
				{
					//判断当前是中文还是英文
					if (isChineseChar(inputCell))
					{
						output[longth + 1] += inputCell;
					}
					else
					{
						output[longth] += inputCell;
						output[longth] += w_input[i + 1];
						i++;

					}

					longth++;
				}
				//如果后一字符是否为中文标点
				else if (punctuation_Z.find(w_input[i + 1]) != std::wstring::npos)
				{
					output[longth + 1] += inputCell;
					longth++;
				}
				else
				{
					output[longth] += inputCell;
				}
			}
			else if (a_len == (howlongth - 1))
			{
				//判断当前是中文还是英文
				if (isChineseChar(inputCell))
				{
					output[longth + 1] += inputCell;
				}
				else
				{
					if ((i + 1) >= (int)w_input.size())
					{
						output[longth] += inputCell;
						break;
					}

					//如果后一字符是否为英文标点
					if (punctuation_E.find(w_input[i + 1]) != std::wstring::npos)
					{
						output[longth + 1] += inputCell;
					}
					//如果后一字符是否为中文标点
					else if (punctuation_Z.find(w_input[i + 1]) != std::wstring::npos)
					{
						output[longth + 1] += inputCell;
					}
					else
					{
						output[longth] += inputCell;
					}
				}
				longth++;
			}
			else if (a_len == howlongth)
			{
				output[longth + 1] += inputCell;
				longth++;
			}
		}

		for (int i = 0; i <= longth; i++)
		{
			outstr.push_back(output[i]);
		}
	}

	std::wstring TrimSring(std::wstring sData, wchar_t flag)
	{
		std::wstring::size_type ipos = sData.find_first_not_of(flag);
		std::wstring::size_type enpos = sData.find_last_not_of(flag);

		if (ipos == enpos && ipos == std::wstring::npos)
			return L"";

		sData = sData.substr(ipos, enpos - ipos + 1);

		return sData;
	}

	std::string TrimSring(std::string sData, char flag)
	{
		std::string::size_type ipos = sData.find_first_not_of(flag);
		std::string::size_type enpos = sData.find_last_not_of(flag);

		if (ipos == enpos && ipos == std::string::npos)
			return "";

		sData = sData.substr(ipos, enpos - ipos + 1);

		return sData;
	}


	int CompareIgnore(const std::string &op1, const std::string &op2)
	{
		std::string temp1 = ToLowerA(op1);
		std::string temp2 = ToLowerA(op2);

		return temp1.compare(temp2);
	}

	std::string StrW2A(const std::wstring &src, int to)
	{
		return ConvertUnicodeToMultiBytes(src, to);
	}

	std::wstring StrA2W(const std::string &src, int from)
	{
		return ConvertMultiBytesToUnicode(src, from);
	}

	std::string StrA2A(const std::string &src, int from, int to)
	{
		wstring strResult = ConvertMultiBytesToUnicode(src, from);
		return ConvertUnicodeToMultiBytes(strResult, to);
	}
}