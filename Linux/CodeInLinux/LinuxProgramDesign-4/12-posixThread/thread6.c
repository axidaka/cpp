#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 线程回调函数
void *thread_function(void* arg);

// 全局共享变量
char message[] = "Hello World";

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, &thread_function, (void*)message);
	if (0 != res)
	{
		perror("Thread createion falied");
		exit(EXIT_FAILURE);
	}

	sleep(4);
	printf("Canceling thread...\n");

	res = pthread_cancel(a_thread);
	if (0 != res)
	{
		perror("Thread cancelation failed");
		exit(EXIT_FAILURE);
	}
	

	printf("Waiting for thread to fininsh...\n");
	res = pthread_join(a_thread, &thread_result);
	if (0 != res)
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined, it return %s\n", (char*)thread_result);
	printf("Message is now %s\n", message);
	exit(EXIT_SUCCESS);
}

void *thread_function(void* arg)
{
	printf("thread_function is running. Argument was %s\n", (char*)arg);

	int i, res;
	// 允许线程接收取消请求
	res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (0 != res)
	{
		perror("Thread pthread_setcancelstate failed");
		exit(EXIT_FAILURE);
	}
	//线程接收到取消请求后， 一直等待到执行了 pthread_join pthread_cond_wait等函数才采取行动
	res = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	if (0 != res)
	{
		perror("Thread pthread_setcanceltype failed");
		exit(EXIT_FAILURE);
	}

	printf("thread_function is running");
	for (i = 0; i < 10; i++)
	{
		printf("Thread is still running(%d)...\n", i);
		sleep(1);
	}
	strcpy(message, "Bye!");
	pthread_exit(NULL);
}
