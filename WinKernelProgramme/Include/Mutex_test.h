#pragma once

///////////////////////////////////////////////////////////////////////////////


class CQueue {
public:
	struct ELEMENT {
		int m_nThreadNum, m_nRequestNum;
		// Other element data should go here
	};
	typedef ELEMENT* PELEMENT;

private:
	PELEMENT m_pElements;        // Array of elements to be processed
	int      m_nMaxElements;     // Maximum # of elements in the array
	HANDLE   m_h[2];             // Mutex & semaphore handles
	HANDLE   &m_hmtxQ;           // Reference to m_h[0]
	HANDLE   &m_hsemNumElements; // Reference to m_h[1]

public:
	CQueue(int nMaxElements);
	~CQueue();

	BOOL Append(PELEMENT pElement, DWORD dwMilliseconds);
	BOOL Remove(PELEMENT pElement, DWORD dwMilliseconds);
};


void Mutex_TestMain();
