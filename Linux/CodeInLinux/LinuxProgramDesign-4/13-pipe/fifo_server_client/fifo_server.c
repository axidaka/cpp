#include "datatype.h"
#include <ctype.h>

int main()
{
	int server_fifo_fd, client_fifo_fd;
	struct data_to_pass_st my_data;
	int read_res;
	char client_fifo[256] = { 0 };
	char* tmp_char_ptr;

	if (access(SERVER_FIFO_NAME, F_OK) == -1)
	{
		if (0 != mkfifo(SERVER_FIFO_NAME, 0777))
		{
			fprintf(stderr, "Sever Could not create fifo %s \n", SERVER_FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}

	server_fifo_fd = open(SERVER_FIFO_NAME, O_RDONLY);
	if (-1 == server_fifo_fd)
	{
		fprintf(stderr, "Sever fifo open failure\n");
		exit(EXIT_FAILURE);
	}

	sleep(10); // wait for client

	do 
	{
		read_res = read(server_fifo_fd, &my_data, sizeof(my_data));
		if (read_res > 0)
		{
			tmp_char_ptr = my_data.some_data;
			while (*tmp_char_ptr)
			{
				*tmp_char_ptr = toupper(*tmp_char_ptr);
				tmp_char_ptr++;
			}

			sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);

			// 打开client命名管道， 将处理后的数据返回
			client_fifo_fd = open(client_fifo, O_WRONLY);
			if (-1 != client_fifo_fd)
			{
				write(client_fifo_fd, &my_data, sizeof(my_data));
				close(client_fifo_fd);
			}
		}
	} while (read_res > 0);

	close(server_fifo_fd);
	unlink(SERVER_FIFO_NAME);
	exit(EXIT_SUCCESS);
}