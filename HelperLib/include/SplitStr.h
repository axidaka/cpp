#ifndef __LIB_DUOWAN_HELPER_SPLITSTR_H__
#define __LIB_DUOWAN_HELPER_SPLITSTR_H__

#include <string>
#include <vector>
#include <list>

class CSplitStr
{
private:
	//�зֵı�־����
	std::wstring m_sSplitFlag;

	//�������зֵı�־���ŵ���һ����־����
	bool m_bSequenceAsOne;

	//���зֵ��ı�
	std::wstring m_sData;

public:

	//�õ��зֺõ��ı���
	void GetSplitStrArray(std::vector<std::wstring> &array);
	void GetSplitStrArray(std::list<std::wstring> &array);

	//�õ����зֵ��ı�
	std::wstring GetData();

	//���ñ��зֵ��ı�
	void SetData(std::wstring sData);

	//�õ��зֲ���
	bool GetSequenceAsOne() {return m_bSequenceAsOne;};

	//�����зֲ���
	void SetSequenceAsOne(bool bSequenceAsOne) {m_bSequenceAsOne = bSequenceAsOne;};

	//�õ��зֱ�־
	std::wstring GetSplitFlag() {return m_sSplitFlag;};

	//�����зֱ�־
	void SetSplitFlag(std::wstring sSplitFlag) {m_sSplitFlag = sSplitFlag;};

	void Split(std::wstring sData, std::wstring sSplitFlag, std::vector<std::wstring> &array);

	CSplitStr();
	virtual ~CSplitStr();
};

#endif