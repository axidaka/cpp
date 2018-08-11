#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// 使用信号量进行同步

void *thread_function(void* arg);
sem_t bin_sem;

#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main()
{
	int res;
	pthread_t a_thread;
	void* thread_result;

	// 参数2为0表示信号量是当前进程的局部信号量， 否则是进程间
	// 参数3为信号量默认值
	res = sem_init(&bin_sem, 0, 0);
	if (0 != res)
	{
		perror("Semaphore init failed");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&a_thread, NULL, &thread_function, NULL);
	if (0 != res)
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	printf("Input some text. Enter 'end' to finish\n");
	memset(work_area, 0, WORK_SIZE);
	while (0 != strncmp(work_area, "end", 3))
	{
		fgets(work_area, WORK_SIZE, stdin);
		sem_post(&bin_sem);
	}

	printf("\nwaiting for thread to finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if (0 != res)
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread join\n");
	sem_destroy(&bin_sem);
	exit(EXIT_SUCCESS);
}

void *thread_function(void* arg)
{
	// 信号默认值为0， 在没有调用sem_post 之前该线程在sem_wait阻塞
	sem_wait(&bin_sem);

	while (0 != strncmp(work_area, "end", 3))
	{
		printf("You input %d char\n", (int)strlen(work_area) - 1);
		sem_wait(&bin_sem);
	}

	pthread_exit(NULL);
}