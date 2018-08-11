#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/sem.h>
#include "semun.h"

int set_semvalue(void);
void del_semvalue(void);
int semaphore_p(void);
int semaphore_v(void);

int sem_id;

int main(int argc, char* argv[])
{
	int i;
	int pause_time;
	char op_char = 'O';

	srand((unsigned int)getpid());

	// 用于进程间同步的信号量
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

	if (argc > 1)
	{
		if (!set_semvalue())
		{
			fprintf(stderr, "Failed to initalize semaphore\n");
			exit(EXIT_FAILURE);
		}

		op_char = 'X';
		sleep(2);
	}

	for ( i = 0; i < 10; i++)
	{
		if (!semaphore_p()) exit(EXIT_FAILURE);
		printf("%c", op_char); fflush(stdout);
		pause_time = rand() % 3;
		sleep(pause_time);
		printf("%c", op_char); fflush(stdout);

		if (!semaphore_v()) exit(EXIT_FAILURE);
		pause_time = rand() % 2;
		sleep(pause_time);
	}

	printf("\n%d - finished\n", getpid());

	if (argc > 1)
	{
		sleep(10);
		del_semvalue();
	}

	exit(EXIT_SUCCESS);
}

int set_semvalue(void)
{
	union semun sem_union;

	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
	return(1);
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
the command IPC_RMID to remove the semaphore's ID. */

void del_semvalue(void)
{
	union semun sem_union;

	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */

int semaphore_p(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n");
		return(0);
	}
	return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
so that the semaphore becomes available. */

int semaphore_v(void)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
		return(0);
	}
	return(1);
}

