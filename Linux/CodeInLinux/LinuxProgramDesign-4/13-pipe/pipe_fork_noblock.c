#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_date[] = "123";

	char buffer[BUFSIZ + 1];
	memset(buffer, '\0', sizeof(buffer));
	pid_t fork_result;
       
    // 管道创建时默认打开文件描述符, 且默认是阻塞(block)模式打开
	/*当没有数据可读时

	O_NONBLOCK disable：read调用阻塞，即进程暂停执行，一直等到有数据来到为止。
	O_NONBLOCK enable：read调用返回-1，errno值为EAGAIN。
	当管道满的时候

	O_NONBLOCK disable： write调用阻塞，直到有进程读走数据
	O_NONBLOCK enable：调用返回-1，errno值为EAGAIN
	如果所有管道写端对应的文件描述符被关闭，则read返回0

	如果所有管道读端对应的文件描述符被关闭，则write操作会产生信号SIGPIPE

	当要写入的数据量不大于PIPE_BUF时，linux将保证写入的原子性。

	当要写入的数据量大于PIPE_BUF时，linux将不再保证写入的原子性。*/
	if (pipe(file_pipes) == 0)
	{
		fork_result = fork();
		if (-1 == fork_result)
		{
			perror("Fork failure");
			exit(EXIT_FAILURE);
		}
		else if (0 == fork_result)
		{
			close(file_pipes[1]); // 子进程关闭写通道	

			printf("Child: [0]-%d [1]-%d\n", file_pipes[0], file_pipes[1]);
			int flags = fcntl(file_pipes[0], F_GETFL); // 先获取原先的flags
			fcntl(file_pipes[0],F_SETFL, flags | O_NONBLOCK);  // 设置fd为非阻塞模式
			data_processed = read(file_pipes[0], buffer, BUFSIZ);
			printf("Read %d bytes: %s\n", data_processed, buffer);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(file_pipes[0]); // 父进程关闭读通道
			sleep(10);
			printf("Parent: [0]-%d [1]-%d\n", file_pipes[0], file_pipes[1]);
			data_processed = write(file_pipes[1], some_date, strlen(some_date));
			printf("Wrote %d bytes\n", data_processed);
		}
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_FAILURE);
}
