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

// 消费者程序， 负责往命名管道读取数据，写入数据有fifo_producer
int main()
{
	int pipe_fd;
	int res;
	int open_mode = O_RDONLY;
	int byte_read = 0;
	char buffer[BUFFER_SIZE + 1] = { 0 };

	printf("Process %d openning FIFO O_RDONLY \n", getpid());
	pipe_fd = open(FIFO_NAME, open_mode);
	printf("Process %d result %d\n", getpid(), pipe_fd);

	if (-1 != pipe_fd)
	{
		do 
		{
			res = read(pipe_fd, buffer, BUFFER_SIZE);
			//printf("%d:read: %d- %s\n", getpid(), res, buffer);
			byte_read += res;
		} while (res > 0);

		close(pipe_fd);
	}
	else
	{
		exit(EXIT_FAILURE);
	}

	printf("Process %d finished %d bytes read\n", getpid(), byte_read);
	exit(EXIT_SUCCESS);
}