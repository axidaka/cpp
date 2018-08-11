/*
 * Thread.cpp
 *
 *  Created on: 2016年12月31日
 *      Author: zhengqingsong
 */

#include <stdio.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

#include "h/Thread.h"
#include "h/CurrentThread.h"


namespace CurrentThread
{
  __thread int t_cachedTid = 0;
  __thread char t_tidString[32];
  __thread int t_tidStringLength = 6;
  __thread const char* t_threadName = "unknown";
}

namespace detail
{
	pid_t gettid()
	{
		return static_cast<pid_t>(::syscall(SYS_gettid));
	}

	void afterFork()
	{
	  CurrentThread::t_cachedTid = 0;
	  CurrentThread::t_threadName = "main";
	  CurrentThread::tid();
	  // no need to call pthread_atfork(NULL, NULL, &afterFork);
	}

	class ThreadNameInitializer
	{
	 public:
	  ThreadNameInitializer()
	  {
	    CurrentThread::t_threadName = "main";
	    CurrentThread::tid();
	    pthread_atfork(NULL, NULL, &afterFork);
	  }
	};

	ThreadNameInitializer init;

	struct ThreadData{
		typedef Thread::ThreadFunc ThreadFunc;
		ThreadFunc func_;
		string name_;

		ThreadData(const ThreadFunc& func,
				const string& name)
		:func_(func)
		,name_(name)
		{}

		void runInThread()
		{
		    pid_t tid = CurrentThread::tid();

		    CurrentThread::t_threadName = name_.empty() ? "muduoThread" : name_.c_str();
		    ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
		    try
		    {
		      func_();
		      CurrentThread::t_threadName = "finished";
		    }
		    catch (const std::exception& ex)
		    {
		      CurrentThread::t_threadName = "crashed";
		      fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
		      fprintf(stderr, "reason: %s\n", ex.what());
		      abort();
		    }
		    catch (...)
		    {
		      CurrentThread::t_threadName = "crashed";
		      fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
		      throw; // rethrow
		    }
		}
	};

	void* startThread(void* obj)
	{
		ThreadData* data = static_cast<ThreadData*>(obj);
		data->runInThread();
		delete data;
		return NULL;
	}
}

void CurrentThread::cacheTid(){
	  if (t_cachedTid == 0)
	  {
		  t_cachedTid = detail::gettid();
		  t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d", t_cachedTid);
	  }
 }
Thread::Thread(const ThreadFunc& func, const string& name)
:started_(false)
,joined_(false)
,func_(func)
,name_(name)
{
}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
	if (started_ && !joined_)
	{
		pthread_detach(pthreadId_);;
	}
}

void Thread::start() {
	assert(!started_);
	started_ = true;

	detail::ThreadData* data = new detail::ThreadData(func_, name_);
	if (0 != pthread_create(&pthreadId_, NULL, &detail::startThread, data))
	{
		started_ = false;
		perror("Thread creation failed");
	}
}

int Thread::join() {
	assert(started_);
	assert(!joined_);
	joined_ = true;

	return pthread_join(pthreadId_, NULL);
}
