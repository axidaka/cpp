/*
 * Thread.h
 *
 *  Created on: 2016年12月31日
 *      Author: zhengqingsong
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <string>
#include <pthread.h>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

using namespace std;

class Thread
{
public:
	typedef boost::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const string& name = string());

	virtual ~Thread();

	void start();
	int join();

private:
	pthread_t pthreadId_;
	bool started_;
	bool joined_;
	ThreadFunc func_;
	string name_;
};

#endif /* THREAD_H_ */
