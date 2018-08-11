#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define  FIFO_NAME "/tmp/my_fifo"

int main(int argc, char* argv[])
{
	int res;
	int open_mode = 0;
	int i;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s<some combination> of O_RDONLY O_WRONLY O_NONBLOCK",*argv);
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++)
	{
		printf("param: %s\n", argv[i]);

		if (strncmp(argv[i], "O_RDONLY", 8) == 0)
		{
			open_mode |= O_RDONLY;
			printf("catch O_RDONLY\n");
		}
		else if (strncmp(argv[i], "O_WRONLY", 8) == 0)
		{
			open_mode |= O_WRONLY;
			printf("catch O_WRONLY\n");
		}
		else if (strncmp(argv[i], "O_NONBLOCK", 10) == 0)
		{
			open_mode |= O_NONBLOCK;
			printf("catch O_NONBLOCK\n");
		}
	}

	if (access(FIFO_NAME, F_OK) == -1)
	{
		printf("run mkfifo\n");
		res = mkfifo(FIFO_NAME, 0777);
		if (res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	printf("Process %d openning FIFO\n", getpid());
	res = open(FIFO_NAME, open_mode);
	printf("Process %d result %d", getpid(), res);
	sleep(5);
	if (res != -1) close(res);
	printf("Process %d fininshed\n", getpid());
	exit(EXIT_FAILURE);
}