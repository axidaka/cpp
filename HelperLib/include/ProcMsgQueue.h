#pragma once
#include <string>

class CShareMem;

typedef struct ProcMsgItem
{
	DWORD dwSize;	//��Ϣ���ȿɱ䣬�������������
	DWORD dwSrcPid;	//src process Id	
	char buf[0];	//��Ϣ�ľ�������
};

/*

������Ϣ���еķ�װ�����������Ϣ������Ҫ���������̷��ʣ����Եײ��ù����ڴ�ʵ�֡�

������Ҫ˵����һ���ǣ����ڶ��еĿ�ʼλ�ã�����λ�õȶ���Ҫ����̷��ʣ�������Щ����
���������ڹ����ڴ��С�

�����ڴ��еĽṹ���� �ܳ���(4) + semophore name(16) + mutex name(16) + msg count(4) + front pos(4) + tail pos(4) + msgs
mutex����;��Ϊ�˻������Ϣ���н���д������
semophore����;��Ϊ��ģ��������ʽGetMessage,����������ϢΪ��ʱ��GetMessage������

��ô�ж϶����Ѿ������أ�
msg count > 0 && front pos == tail pos

����ѭ������
1����ʼ��ʱfront pos=tail pos=0��
2��front pos�ڲ�Ϊ��ʱ����ָ����п�ʼ��Ԫ�ء�
3��tail pos����ָ�����ĩβ����λ�ã�Ҳ����˵tail pos��ָ��λ��û�зŶ�����

*/

class CProcMsgQueue
{
public:
	CProcMsgQueue();
	~CProcMsgQueue();

	//Create����Ϣ���е�ӵ���ߵ���
	HRESULT Create(std::wstring const& strName, DWORD dwQueueSize = 64 * 1024);

	//Open��������Ҫ��������̷���Ϣ�Ľ��̵���
	HRESULT Open(std::wstring const& strName);

	//��Ӧcreate��open
	void Close();

public:
	HRESULT EnQueue(ProcMsgItem const& pmsg);
	HRESULT DeQueue(ProcMsgItem*& pmsg, DWORD dwWaitMS = INFINITE);

protected:

	DWORD GetTotalSize() const;
	DWORD GetMsgCount() const;
	DWORD GetFrontPos() const;
	DWORD GetTailPos() const;
	DWORD GetMsgSize(DWORD MsgPos, DWORD TotalSize) const;
	HRESULT GetMutexName(std::wstring& MutexName) const;
	HRESULT GetSemophoreName(std::wstring& SemoName) const;

	HRESULT SetMsgCount(DWORD dw);
	HRESULT SetFrontPos(DWORD dw);
	HRESULT SetTailPos(DWORD dw);
	HRESULT SetMutexName(GUID const& g);
	HRESULT SetSemophoreName(GUID const& g);

private:

	HANDLE m_hSemophore;  ///< ����Ϊ��ʱ����DeQueue
	HANDLE m_hMutex;      ///<���ڻ�����ʹ����ڴ棬����ΪGUID
	CShareMem* m_pShMem; 
};