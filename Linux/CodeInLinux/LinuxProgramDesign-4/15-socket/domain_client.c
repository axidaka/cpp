#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

// 本地客户端程序， 不需要经过网络协议栈，不需要打包拆包、计算校验和、维护序号和应答等，只是将应用层数据从一个进程拷贝到另一个进程

#define  DOMAIN_SERVER_NAME "sever_socket"

int main()
{
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	//AF_UNIX用于同一台机器上的进程间通信, 使用本地文件， 类似于管道
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	// 根据服务器的情况给套接字命名
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, DOMAIN_SERVER_NAME);
	len = sizeof(address);
	
	// 连接服务器
	result = connect(sockfd, (struct sockaddr*)&address, len);
	if (-1 == result)
	{
		perror("domain_client oops: clinet");
		exit(EXIT_FAILURE);
	}

	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("domain_client char from server = %c\n", ch);
	close(sockfd);
	exit(EXIT_SUCCESS);
}