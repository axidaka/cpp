#ifndef __LIB_DUOWAN_HELPER_SPLITSTR_H__
#define __LIB_DUOWAN_HELPER_SPLITSTR_H__

#include <string>
#include <vector>
#include <list>

class CSplitStr
{
private:
	//切分的标志符号
	std::wstring m_sSplitFlag;

	//连续的切分的标志符号当成一个标志处理
	bool m_bSequenceAsOne;

	//被切分的文本
	std::wstring m_sData;

public:

	//得到切分好的文本串
	void GetSplitStrArray(std::vector<std::wstring> &array);
	void GetSplitStrArray(std::list<std::wstring> &array);

	//得到被切分的文本
	std::wstring GetData();

	//设置被切分的文本
	void SetData(std::wstring sData);

	//得到切分参数
	bool GetSequenceAsOne() {return m_bSequenceAsOne;};

	//设置切分参数
	void SetSequenceAsOne(bool bSequenceAsOne) {m_bSequenceAsOne = bSequenceAsOne;};

	//得到切分标志
	std::wstring GetSplitFlag() {return m_sSplitFlag;};

	//设置切分标志
	void SetSplitFlag(std::wstring sSplitFlag) {m_sSplitFlag = sSplitFlag;};

	void Split(std::wstring sData, std::wstring sSplitFlag, std::vector<std::wstring> &array);

	CSplitStr();
	virtual ~CSplitStr();
};

#endif