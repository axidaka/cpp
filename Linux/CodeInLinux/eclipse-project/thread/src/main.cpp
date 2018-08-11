/*
 * main.cpp
 *
 *  Created on: 2016年12月31日
 *      Author: zhengqingsong
 */

#include <stdio.h>
#include <pthread.h>

#include "h/Thread.h"
#include "h/CurrentThread.h"

void thread_function()
{
	 printf("tname=%s tid=%d \n", CurrentThread::name(),CurrentThread::tid());
	printf("thread_function is running.\n");
}

int main(int argc, char** argv)
{
	 printf("pid=%d, tname=%s,tid=%d\n", ::getpid(), CurrentThread::name(),CurrentThread::tid());
	Thread thobj(thread_function, "haha");
	thobj.start();
	thobj.join();
	return 0;
}

