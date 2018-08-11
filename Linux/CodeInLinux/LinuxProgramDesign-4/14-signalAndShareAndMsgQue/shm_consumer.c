#include "shm_com.h"

// �����߳��� ���ڴ��������ڴ棬 ��ȡ��������д�����Ϣ
int main()
{
	srand((unsigned int)getpid());
	int shmid;
	// ���������ڴ�id
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

	if (-1 == shmid)
	{
		fprintf(stderr, "shm_consumer shmget failed\n");
		exit(EXIT_FAILURE);
	}

	void *shared_memory = (void*)0;
	// �������ڴ����ӵ�һ�����̵ĵ�ַ�ռ䣬�����޷�����
	shared_memory = shmat(shmid, (void*)0, 0);
	if ((void*)-1 == shared_memory)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	printf("shm_consumer Memory attached at %x\n", (int)shared_memory);

	struct shared_use_st *shared_stuff;
	shared_stuff = (struct shared_use_st *)shared_memory;
	// �������߳�������Ϣʱ�� �ñ�־λΪ1
	shared_stuff->written_by_you = 0;
	int running = 1;
	while (running)
	{
		if (shared_stuff->written_by_you)
		{
			printf("shm_consumer You wrote: %s", shared_stuff->some_text);
			sleep(rand() % 4); 
			shared_stuff->written_by_you = 0;
			
			if (0 == strncmp(shared_stuff->some_text, "end", 3))
			{
				running = 0;
			}
		}
	}

	// ���빲���ڴ�
	if (-1 == shmdt(shared_memory))
	{
		fprintf(stderr, "shm_consumer shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	// ɾ��
	if (-1 == shmctl(shmid, IPC_RMID, 0))
	{
		fprintf(stderr, "shm_consumer shmctl IPC_RMID failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}