#include <string>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include "threadpool.h"

using namespace std;

class ThreadTask{

public:
	ThreadTask(const string& taskName)
	:_taskName(taskName)
	{}
	~ThreadTask(){}

	// 用于被threadpoll调用的函数
	bool process(){
		printf("ThreadTask process: %s\n", _taskName.c_str());
		return true;
	}

private:
	string _taskName;
};

void* worker(void* arg)
{
	threadpool<ThreadTask>* pTp = (threadpool<ThreadTask>*)arg;
	vector<ThreadTask*> vct_threadtask;
	for (int i = 0; i < 100; i ++)
	{
		char buf[255] = { 0 };
		sprintf(buf,  "TaskName_%d", i);
		vct_threadtask.push_back(new ThreadTask(buf));
		printf("create %s\n", buf);
		pTp->append(vct_threadtask[i]);
	}
	
	while (!pTp->IsQueueEmpty())
	{
		continue;
	}

	for (vector<ThreadTask*>::iterator iter_vctBegin = vct_threadtask.begin();
		iter_vctBegin != vct_threadtask.end(); iter_vctBegin++)
	{
		delete *iter_vctBegin;
	}
	
	return pTp;
}

int main()
{
	threadpool<ThreadTask> *pThreadPooll = NULL;
	
	try
	{
		pThreadPooll = new threadpool<ThreadTask>();
	}
	catch (...)
	{
		printf("threadpool create fail\n");
		return 1;
	}

	pthread_t pthreadId_;
	pthread_create(&pthreadId_, NULL, worker, pThreadPooll);
	pthread_join(pthreadId_, NULL);
	
	delete pThreadPooll;
    return 0;
}