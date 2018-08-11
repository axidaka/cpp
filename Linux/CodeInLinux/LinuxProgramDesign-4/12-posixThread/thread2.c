#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 线程回调函数
void *thread_function(void* arg);

// 全局共享变量
int run_now = 1;

int main()
{
	int res;
	pthread_t a_thread;
	void *thread_result;

	res = pthread_create(&a_thread, NULL, &thread_function, NULL);
	if (0 != res)
	{
		perror("Thread createion falied");
		exit(EXIT_FAILURE);
	}

	int print_count1 = 0;
	while (print_count1++ < 20)
	{
		if (1 == run_now)
		{
			printf("1\n");
			run_now = 2;
		}
		else
		{
			sleep(1);
		}
	}

	res = pthread_join(a_thread, &thread_result);
	if (0 != res)
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

void *thread_function(void* arg)
{
	int print_count2 = 0;
	while (print_count2++ < 20)
	{
		if (2 == run_now)
		{
			printf("2\n");
			run_now = 1;
		}
		else
		{
			sleep(1);
		}
	}

	pthread_exit("Thank you for the CPU time");
}
