#include "stdafx.h"
#include "ProcMsgQueue.h"
#include "ShareMem.h"
#include "ToolMacros.h"
#include <cassert>
#include <tchar.h>
#include "MutexLock.h"

#define TOTALSIZE_OFFSET	(0)
#define SEMOPHORE_OFFSET	(sizeof(DWORD))
#define MUTEX_OFFSET		(SEMOPHORE_OFFSET+sizeof(GUID))
#define MSG_COUNT_OFFSET	(MUTEX_OFFSET+sizeof(GUID))
#define FRONT_POS_OFFSET	(MSG_COUNT_OFFSET+sizeof(DWORD))
#define TAIL_POS_OFFSET		(FRONT_POS_OFFSET+sizeof(DWORD))
#define MSG_BEGIN_OFFSET	(TAIL_POS_OFFSET+sizeof(DWORD))

CProcMsgQueue::CProcMsgQueue()
:m_pShMem(NULL)
, m_hMutex(NULL)
, m_hSemophore(NULL)
{

}

CProcMsgQueue::~CProcMsgQueue()
{
	this->Close();
}

HRESULT CProcMsgQueue::Create(std::wstring const& strName, DWORD dwQueueSize /*= 64 * 1024*/)
{
	// 创建共享内存->创建互斥对象
	if (m_pShMem)
	{
		return S_FALSE;
	}

	m_pShMem = new CShareMem;
	if (m_pShMem == NULL)
	{
		assert(false);
		return E_OUTOFMEMORY;
	}

	HRESULT hr = m_pShMem->Create(strName, dwQueueSize);
	if (FAILED(hr))
	{
		assert(false);
		Delete_IFPtr(m_pShMem);
		return hr;
	}
	else if (hr == S_OK)
	{
		///< 创建唯一的UUID，作为互斥对象的名称
		GUID g;
		CoCreateGuid(&g);
		TCHAR buf[MAX_PATH] = { 0 };
		StringFromGUID2(g, buf, MAX_PATH);
		
		///<　创建互斥对象
		m_hMutex = CreateMutex(NULL, TRUE, buf);
		if (m_hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
		{
			assert(false);
			Delete_IFPtr(m_pShMem);
			Close_IFHandle(m_hMutex);

			return E_FAIL;
		}
		

		///< 创建信标
		GUID semophoreGUID;
		CoCreateGuid(&semophoreGUID);
		StringFromGUID2(semophoreGUID, buf, MAX_PATH);
		m_hSemophore = CreateSemaphore(NULL, 0, 4096, buf); ///< 初始化为0，只有在EnQueue +1
		if (m_hSemophore == NULL ||
			GetLastError() == ERROR_ALREADY_EXISTS)
		{
			assert(false);
			Delete_IFPtr(m_pShMem);
			Close_IFHandle(m_hMutex);
			Close_IFHandle(m_hSemophore);
			
			return E_FAIL;
		}

		SetMutexName(g);
		SetSemophoreName(semophoreGUID);

		///< 初始化位置信息
		SetMsgCount(0);
		SetFrontPos(MSG_BEGIN_OFFSET);
		SetTailPos(MSG_BEGIN_OFFSET);

		///< 使用完毕记得释放互斥锁
		ReleaseMutex(m_hMutex);
	}
	else if (hr = S_FALSE)
	{
		// 共享内存已经打开过
		std::wstring MutexName;
		GetMutexName(MutexName);
		m_hMutex = OpenMutex(MUTEX_MODIFY_STATE | SYNCHRONIZE, FALSE, MutexName.c_str());
		if (m_hMutex == NULL)
		{
			assert(false);
			Delete_IFPtr(m_pShMem);

			return E_FAIL;
		}

		std::wstring SemophoreName;
		GetSemophoreName(SemophoreName);
		m_hSemophore = OpenSemaphore(SEMAPHORE_ALL_ACCESS | SYNCHRONIZE, FALSE, SemophoreName.c_str());
		if (m_hSemophore == NULL)
		{
			assert(false);
			Delete_IFPtr(m_pShMem);
			Delete_IFPtr(m_hMutex);

			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CProcMsgQueue::Open(std::wstring const& strName)
{
	if (m_pShMem)
	{
		return S_FALSE;
	}

	m_pShMem = new CShareMem;
	if (m_pShMem == NULL)
	{
		assert(false);
		return E_OUTOFMEMORY;
	}

	HRESULT hr = m_pShMem->Open(strName);
	if (FAILED(hr))
	{
		assert(false);
		Delete_IFPtr(m_pShMem);
		return hr;
	}

	//打开Mutex
	std::wstring strMutex;
	GetMutexName(strMutex);
	m_hMutex = ::OpenMutex(MUTEX_MODIFY_STATE | SYNCHRONIZE, FALSE, strMutex.c_str());
	if (m_hMutex == NULL)
	{
		assert(false);
		Delete_IFPtr(m_pShMem);
		return E_FAIL;
	}

	return S_OK;

}

void CProcMsgQueue::Close()
{
	Delete_IFPtr(m_pShMem);
	Close_IFHandle(m_hMutex);
}

HRESULT CProcMsgQueue::EnQueue(ProcMsgItem const& msg)
{
	///< 进程间同步需要加锁
	CMutexLock lock(m_hMutex);

	// 获取当前 队列长度  frontpos tailpos，判断是否已满
	// 获取共享内存总长度及剩余空间长度是否足够
	
	DWORD dwMsgCount = GetMsgCount();
	DWORD dwFrontPos = GetFrontPos();
	DWORD dwTailPos = GetTailPos();

	///< 1.判断当前队列是否已满
	if (dwMsgCount > 0 && dwFrontPos == dwTailPos)
	{
		_tprintf(_T("ProcMsgQueue Full\n"));
		return E_OUTOFMEMORY;
	} 
	else
	{
		///< 2.判断剩余空间
		DWORD dwTotalSize = GetTotalSize();
		DWORD dwLeft = 0;
		if (dwTailPos >= dwFrontPos) ///< 尾指针在头指针后面
		{
			dwLeft = dwFrontPos - MSG_BEGIN_OFFSET + dwTotalSize - dwTailPos;
		}
		else
		{
			dwLeft = dwFrontPos - dwTailPos;
		}

		if (dwLeft < msg.dwSize)
		{
			_tprintf(_T("ProcMsgQueue not enough space\n"));
			return E_OUTOFMEMORY;
		}

		///< 3开始写入
		///< a 元素数目加1
		dwMsgCount++;
		SetMsgCount(dwMsgCount);

		///< b将msg写入，要注意
		if (dwTailPos + msg.dwSize > dwTotalSize)
		{
			DWORD dwRightSize = dwTotalSize - dwTailPos;
			DWORD dwLeftSize = msg.dwSize - dwRightSize;
			PBYTE const pData = (PBYTE)&msg;
			m_pShMem->WriteMem(pData , dwRightSize, dwTailPos);
			m_pShMem->WriteMem(pData + dwRightSize, dwLeftSize, MSG_BEGIN_OFFSET);

			dwTailPos = MSG_BEGIN_OFFSET + dwLeftSize;

			///< 不可能写到dwFrontPos;
			assert(dwTailPos < dwFrontPos);
		}
		else
		{
			m_pShMem->WriteMem((PBYTE const)&msg, msg.dwSize, dwTailPos);
			dwTailPos += msg.dwSize;
		}

		///< c 更新 位置
		SetTailPos(dwTailPos);

		///< 通知接受方，队列新增消息
		::ReleaseSemaphore(m_hSemophore, 1, NULL);
	}

	return S_OK;

}

HRESULT CProcMsgQueue::DeQueue(ProcMsgItem*& pmsg, DWORD dwWaitMS /*= INFINITE*/)
{
	pmsg = NULL; 

	///< 在消息队列等待
	DWORD dwRet = WaitForSingleObject(m_hSemophore, dwWaitMS);
	{
		///< 进程间同步需要加锁
		CMutexLock lock(m_hMutex);

		// 队列是否为空，不空，去除front元素，front移位更新msgcount

		///< 1队列为空
		DWORD dwMsgCount = GetMsgCount();
		if (dwMsgCount <= 0)
		{
			///< 没有消息，就用一个空消息代替
			///TODO
			return S_FALSE;
		}

		///< 2获取消息长度
		DWORD dwFrontPos = GetFrontPos();
		DWORD dwTotalSize = GetTotalSize();
		DWORD dwMsgSize = GetMsgSize(dwFrontPos, dwTotalSize);

		if (dwMsgSize < (sizeof(DWORD)* 2))
		{
			assert(false);
			return E_FAIL;
		}

		///< 3给消息分配空间
		pmsg = reinterpret_cast<ProcMsgItem*>(new char[dwMsgSize]);
		if (pmsg == NULL)
		{
			assert(false);
			return E_OUTOFMEMORY;
		}

		///< 4读取消息，注意判断消息是否连续
		if (dwFrontPos + dwMsgSize > dwTotalSize)
		{
			///< 消息分成两边
			DWORD dwRightSize = dwTotalSize - dwFrontPos;
			DWORD dwLeftSize = dwMsgSize - dwRightSize;

			PBYTE pData = (PBYTE)pmsg;
			m_pShMem->ReadMem(pData, dwRightSize, dwFrontPos);
			m_pShMem->ReadMem(pData + dwRightSize, dwLeftSize, MSG_BEGIN_OFFSET);

			dwFrontPos = MSG_BEGIN_OFFSET + dwLeftSize;
		}
		else
		{
			m_pShMem->ReadMem((PBYTE)pmsg, dwMsgSize, dwFrontPos);
			dwFrontPos += dwMsgSize;
		}

		///< 5更新frontpos
		SetFrontPos(dwFrontPos);

		///< 6更新msgcount
		dwMsgCount--;
		SetMsgCount(dwMsgCount);

		
	}

	return S_OK;

}

DWORD CProcMsgQueue::GetTotalSize() const
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return 0;
	}

	DWORD dwCount = 0;
	m_pShMem->ReadMem((BYTE*)&dwCount, sizeof(DWORD), TOTALSIZE_OFFSET);

	return dwCount;
}

DWORD CProcMsgQueue::GetMsgCount() const
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return 0;
	}

	DWORD dwCount = 0;
	m_pShMem->ReadMem((BYTE*)&dwCount, sizeof(DWORD), MSG_COUNT_OFFSET);

	return dwCount;
}

DWORD CProcMsgQueue::GetFrontPos() const
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return 0;
	}

	DWORD dwCount = 0;
	m_pShMem->ReadMem((BYTE*)&dwCount, sizeof(DWORD), FRONT_POS_OFFSET);

	return dwCount;
}

DWORD CProcMsgQueue::GetTailPos() const
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return 0;
	}

	DWORD dwCount = 0;
	m_pShMem->ReadMem((PBYTE)&dwCount, sizeof(DWORD), TAIL_POS_OFFSET);

	return dwCount;
}

DWORD CProcMsgQueue::GetMsgSize(DWORD MsgPos, DWORD TotalSize) const
{
	DWORD dwSize = 0;
	if (MsgPos + sizeof(DWORD) > TotalSize)//长度DWORD被分开在队列尾部与队列前部
	{
		BYTE* ptr = reinterpret_cast<BYTE*>(&dwSize);
		int nFirst = TotalSize - MsgPos;
		int nSecond = sizeof(DWORD)-nFirst;
		m_pShMem->ReadMem(ptr, nFirst, MsgPos);
		m_pShMem->ReadMem(ptr + nFirst, nSecond, MSG_BEGIN_OFFSET);
	}
	else
	{
		m_pShMem->ReadMem((BYTE*)&dwSize, sizeof(DWORD), MsgPos);
	}

	return dwSize;
}

HRESULT CProcMsgQueue::GetMutexName(std::wstring& MutexName) const
{
	if (m_pShMem != NULL)
	{
		GUID g;
		HRESULT hr = m_pShMem->ReadMem((PBYTE)&g, sizeof(GUID), MUTEX_OFFSET);

		TCHAR buf[MAX_PATH] = { 0 };
		StringFromGUID2(g, buf, MAX_PATH);

		MutexName.append(buf);

		return hr;
	}
	else
	{
		assert(false);
		return S_FALSE;
	}
}

HRESULT CProcMsgQueue::GetSemophoreName(std::wstring& SemoName) const
{
	if (m_pShMem != NULL)
	{
		GUID g;
		HRESULT hr = m_pShMem->ReadMem((PBYTE)&g, sizeof(GUID), SEMOPHORE_OFFSET);

		TCHAR buf[MAX_PATH] = { 0 };
		StringFromGUID2(g, buf, MAX_PATH);

		SemoName.append(buf);

		return hr;
	}
	else
	{
		assert(false);
		return S_FALSE;
	}
}

HRESULT CProcMsgQueue::SetFrontPos(DWORD dw)
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return S_FALSE;
	}

	return m_pShMem->WriteMem((PBYTE)&dw, sizeof(DWORD), FRONT_POS_OFFSET);
}

HRESULT CProcMsgQueue::SetMsgCount(DWORD dw)
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return S_FALSE;
	}

	return m_pShMem->WriteMem((PBYTE)&dw, sizeof(DWORD), MSG_COUNT_OFFSET);;
}

HRESULT CProcMsgQueue::SetTailPos(DWORD dw)
{
	if (m_pShMem == NULL)
	{
		assert(false);
		return S_FALSE;
	}

	return m_pShMem->WriteMem((PBYTE)&dw, sizeof(DWORD), TAIL_POS_OFFSET);
}

HRESULT CProcMsgQueue::SetMutexName(GUID const& g)
{
	if (m_pShMem != NULL)
	{
		return m_pShMem->WriteMem((PBYTE)&g, sizeof(GUID), MUTEX_OFFSET);
	}
	else
	{
		assert(false);
		return S_FALSE;
	}
}

HRESULT CProcMsgQueue::SetSemophoreName(GUID const& g)
{
	if (m_pShMem != NULL)
	{
		return m_pShMem->WriteMem((PBYTE)&g, sizeof(GUID), SEMOPHORE_OFFSET);
	}
	else
	{
		assert(false);
		return S_FALSE;
	}
}
