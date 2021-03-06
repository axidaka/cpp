﻿#include <stdio.h>
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
	sleep(3);
	strcpy(message, "Bye!");
	pthread_exit("Thank you for the CPU time");
}
