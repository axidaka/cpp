#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 线程回调函数
void *thread_function(void* arg);

// 全局共享变量
char message[] = "Hello World";
int thread_finished = 0;

int main()
{
	int res;
	pthread_t a_thread;
	pthread_attr_t thread_attr;

	res = pthread_attr_init(&thread_attr);
	if (0 != res)
	{
		perror("Attribute creation failed");
		exit(EXIT_FAILURE);
	}

	// 设置子线程可分离， 不能调用pthread_join 来获取另一个线程的退出状态
	res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if (0 != res)
	{
		perror("Setting detached attribute failed");
		exit(EXIT_FAILURE);
	}
	
	res = pthread_create(&a_thread, NULL, &thread_function, (void*)message);
	if (0 != res)
	{
		perror("Thread createion falied");
		exit(EXIT_FAILURE);
	}

	(void)pthread_attr_destroy(&thread_attr);

	while (!thread_finished)
	{
		printf("Waiting for thread to say it finish...\n");
		sleep(1);
	}

	printf("Other thread finished. byte\n");
	exit(EXIT_SUCCESS);
}

void *thread_function(void* arg)
{
	printf("thread_function is running. Argument was %s\n", (char*)arg);
	sleep(3);
	printf("Second thread setting finished flag, and exiting now\n");
	thread_finished = 1;
	pthread_exit(NULL);
}
