#pragma once
#include <string>

class CShareMem;

typedef struct ProcMsgItem
{
	DWORD dwSize;	//消息长度可变，故增加这个参数
	DWORD dwSrcPid;	//src process Id	
	char buf[0];	//消息的具体内容
};

/*

进程消息队列的封装，由于这个消息队列需要被其他进程访问，所以底层用共享内存实现。

这里需要说明的一点是，由于队列的开始位置，结束位置等都需要跨进程访问，所以这些数据
都必须存放在共享内存中。

共享内存中的结构如下 总长度(4) + semophore name(16) + mutex name(16) + msg count(4) + front pos(4) + tail pos(4) + msgs
mutex的用途是为了互斥对消息队列进行写操作。
semophore的用途是为了模拟阻塞方式GetMessage,当队列中消息为空时，GetMessage将阻塞

怎么判断队列已经满了呢？
msg count > 0 && front pos == tail pos

对于循环队列
1、初始化时front pos=tail pos=0。
2、front pos在不为空时总是指向队列开始的元素。
3、tail pos总是指向队列末尾后面位置，也就是说tail pos所指的位置没有放东西。

*/

class CProcMsgQueue
{
public:
	CProcMsgQueue();
	~CProcMsgQueue();

	//Create由消息队列的拥有者调用
	HRESULT Create(std::wstring const& strName, DWORD dwQueueSize = 64 * 1024);

	//Open由其他需要向这个进程发消息的进程调用
	HRESULT Open(std::wstring const& strName);

	//对应create和open
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

	HANDLE m_hSemophore;  ///< 队列为空时阻塞DeQueue
	HANDLE m_hMutex;      ///<用于互斥访问共享内存，名称为GUID
	CShareMem* m_pShMem; 
};