#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// 使用互斥量进行同步

void *thread_function(void* arg);
pthread_mutex_t work_mutex; 

#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int time_to_exit = 0;

int main()
{
	int res;
	pthread_t a_thread;
	void* thread_result;

	res = pthread_mutex_init(&work_mutex, NULL);
	if (0 != res)
	{
		perror("Mutext init failed");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&a_thread, NULL, &thread_function, NULL);
	if (0 != res)
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_lock(&work_mutex);
	printf("Input some text. Enter 'end' to finish\n");
	memset(work_area, 0, WORK_SIZE);
	while (!time_to_exit)
	{
		fgets(work_area, WORK_SIZE, stdin);
		printf("fgets: %s", work_area);
		pthread_mutex_unlock(&work_mutex);

		// 要等到子线程输出字符个数才继续获取用户输入
		while (1)
		{
			pthread_mutex_lock(&work_mutex);
			if (work_area[0] != '\0')
			{
				pthread_mutex_unlock(&work_mutex);
				sleep(1);
			}
			else
			{
				break;
			}
		} 
	}

	pthread_mutex_unlock(&work_mutex);
	printf("\nwaiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if (0 != res)
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread join\n");
	pthread_mutex_destroy(&work_mutex);
	exit(EXIT_SUCCESS);
}

void *thread_function(void* arg)
{
	sleep(1);
	pthread_mutex_lock(&work_mutex);
	while (0 != strncmp(work_area, "end", 3))
	{
		printf("You input %d char \n", (int)strlen(work_area) - 1);

		// 找到可以避免 在用户重新输入之前， 不会重新进入while
		work_area[0] = '\0';
		pthread_mutex_unlock(&work_mutex);

		sleep(1);
		pthread_mutex_lock(&work_mutex);
		while (work_area[0] == '\0')
		{
			// 一直等待到用户重新输入
			pthread_mutex_unlock(&work_mutex);

			sleep(1);

			pthread_mutex_lock(&work_mutex);
		}	
	}

	time_to_exit = 1;
	work_area[0] = '\0';
	pthread_mutex_unlock(&work_mutex);
	pthread_exit(NULL);
}