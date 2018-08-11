#include "stdafx.h"
#include "splitstr.h"
#include "UnicodeConv.h"
using namespace std;

//构造函数
CSplitStr::CSplitStr()
{
	SetData(L"");
	SetSequenceAsOne(TRUE);
	SetSplitFlag(L",");
}

CSplitStr::~CSplitStr()
{
}

//设置文本函数
void CSplitStr::SetData(std::wstring sData)
{
	m_sData = sData;

	std::wstring::size_type ipos = sData.find_first_not_of(L' ');
	std::wstring::size_type enpos = sData.find_last_not_of(L' ');
	if(ipos == enpos && ipos == std::wstring::npos)
		m_sData = L"";
	else
		sData = sData.substr(ipos, enpos - ipos + 1);
}

std::wstring CSplitStr::GetData()
{
	return m_sData;
}

void CSplitStr::Split(std::wstring sData, std::wstring sSplitFlag, std::vector<std::wstring> &array)
{
	SetSplitFlag(sSplitFlag);
	SetData(sData);
	GetSplitStrArray(array);
}
//切分操作函数（很简单实用吧）
void CSplitStr::GetSplitStrArray(vector<std::wstring> &array)
{
	array.clear();

	std::wstring sData = GetData();
	std::wstring sSplitFlag = GetSplitFlag();

	if (sData.empty())
		return;

	if (sData.substr(sData.size() -1 , 1) != sSplitFlag)
		sData += sSplitFlag;

	std::wstring sTemp;
	std::wstring::size_type pos =-1;
	while ((pos=sData.find(sSplitFlag,0)) != std::wstring::npos)
	{
		
		sTemp = sData.substr(0, pos);

		if (!GetSequenceAsOne())
		{
			array.push_back(sTemp);
		}
		else
		{
			//if (!sTemp.empty() && sTemp !=L"") 
			//{
				array.push_back(sTemp);
			//}
		}
		
		sData = sData.substr(pos + sSplitFlag.length(), sData.length() - pos);
	}
}

//切分操作函数（很简单实用吧）
void CSplitStr::GetSplitStrArray(list<std::wstring> &array)
{
	array.clear();

	std::wstring sData = GetData();
	std::wstring sSplitFlag = GetSplitFlag();

	if (sData.substr(sData.size() -1 , 1) != sSplitFlag)
		sData += sSplitFlag;

	std::wstring sTemp;
	std::wstring::size_type pos =-1;
	while ((pos=sData.find(sSplitFlag)) != std::wstring::npos)
	{
		sTemp = sData.substr(0, pos);

		if (!GetSequenceAsOne())
		{
			array.push_back(sTemp);
		}
		else
		{
			if (!sTemp.empty() && sTemp != L"") ////连续的分隔符视为单个处理
			{
				array.push_back(sTemp);
			}
		}
		sData = sData.substr(pos + 1, sData.length() - pos);
	}
}