#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE PIPE_BUF
#define TEN_MEG (1024*1024*10)

// 生产者程序， 负责往命名管道写入数据，有fifo_consumer读取数据
int main()
{
	int pipe_fd;
	int res;
	int open_mode = O_WRONLY;
	int byte_send = 0;
	char buffer[BUFFER_SIZE + 1] = { "production" };

	if (access(FIFO_NAME, F_OK) == -1)
	{// 管道文件不存在
		res = mkfifo(FIFO_NAME, 0777);
		if (0 != res)
		{
			fprintf(stderr, "Could not create fifo %s \n", FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	printf("Process %d openning FIFO O_WRONLY \n", getpid());
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	if (-1 != pipe_fd)
	{
		while (byte_send < TEN_MEG)
		{
			res = write(pipe_fd, buffer, strlen(buffer));
			//printf("write %d\n", res);
			if (-1 == res)
			{
				fprintf(stderr, "Write error on pipe\n");
				exit(EXIT_FAILURE);
			}
			byte_send += res;
			//sleep(1);
		}
		close(pipe_fd);
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	printf("Process %d finished \n", getpid());
	exit(EXIT_SUCCESS);
}