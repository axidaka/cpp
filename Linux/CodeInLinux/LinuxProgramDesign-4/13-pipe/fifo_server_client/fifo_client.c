#include "datatype.h"
#include <ctype.h>

int main()
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st my_data;
	int time_to_send;
	char client_fifo[256] = { 0 };

	// 以写方式打开服务端命名管道
	server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
	if (-1 == server_fifo_fd)
	{
		fprintf(stderr, "Sorry, no server\n");
		exit(EXIT_FAILURE);
	}

	my_data.client_pid = getpid();
	sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);
	// 创建本进程的命名管道
	if (mkfifo(client_fifo, 0777) == -1)
	{
		fprintf(stderr, "Sorry, cann't make %s", client_fifo);
		close(server_fifo_fd);
		exit(EXIT_FAILURE);
	}

	for (time_to_send = 0; time_to_send < 5; time_to_send++)
	{
		sprintf(my_data.some_data, "Hello from %d-%d", my_data.client_pid, time_to_send);
		write(server_fifo_fd, &my_data, sizeof(my_data));
		
		// 读取服务端返回的处理结果
		client_fifo_fd = open(client_fifo, O_RDONLY);
		if (-1 != client_fifo_fd)
		{
			if (read(client_fifo_fd, &my_data, sizeof(my_data))> 0)
			{
				printf("Received: %d-%s\n", my_data.client_pid, my_data.some_data);
			}
			close(client_fifo_fd);
		}
	}

	close(server_fifo_fd);
	unlink(client_fifo);
	exit(EXIT_FAILURE);
}