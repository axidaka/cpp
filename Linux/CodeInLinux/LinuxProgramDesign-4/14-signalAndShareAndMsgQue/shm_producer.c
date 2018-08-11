#include "shm_com.h"


int main()
{
	int shmid;
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if (-1 == shmid)
	{
		fprintf(stderr, "shm_producer shmget failed\n");
		exit(EXIT_FAILURE);
	}

	void *shared_memory = (void*)0;
	// 将共享内存连接到一个进程的地址空间，否则无法访问
	shared_memory = shmat(shmid, (void*)0, 0);
	if ((void*)-1 == shared_memory)
	{
		fprintf(stderr, "shm_producer shmat failed\n");
		exit(EXIT_FAILURE);
	}

	printf("shm_producer Memory attached at %x\n", (int)shared_memory);
	struct shared_use_st *shared_stuff;
	shared_stuff = (struct shared_use_st *)shared_memory;
	int running = 1;
	char buffer[BUFSIZ] = { 0 };
	while (running)
	{
		while (1 == shared_stuff->written_by_you)
		{
			sleep(1);
			printf("shm_producer waiting for client...\n");
		}

		printf("shm_producer Enter some text:");
		fgets(buffer, BUFSIZ, stdin);

		strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
		shared_stuff->written_by_you = 1;

		if (0 == strncmp(buffer, "end", 3))
		{
			running = 0;
		}

	}

	// 分离共享内存
	if (-1 == shmdt(shared_memory))
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}