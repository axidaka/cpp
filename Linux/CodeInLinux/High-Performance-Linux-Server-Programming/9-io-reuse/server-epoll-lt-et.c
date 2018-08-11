#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

/*将文件描述符设置成非阻塞*/
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

/*将文件描述符fd上的EPOLLIN注册到epollfd指示的内核事件表中,enable_set指定是否对fd启动ET模式*/
void addfd(int epollfd, int fd, bool enabel_et)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if (enabel_et)
	{
		event.events |= EPOLLET;
	}

	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

/*LT模式的工作流程*/
void lt(epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE] = { 0 };
	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			// 监听套接字上有新客户接入
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);

			// 对新客户连接警用ET模式
			addfd(epollfd, connfd, false);
		}
		else if (events[i].events & EPOLLIN)
		{
			// 只要socket读缓存中还有未读出的数据， 这段代码就被触发
			printf("lt:EPOLLIN event trigger once\n");
			memset(buf, '\0', BUFFER_SIZE);
			int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
			if (ret <= 0)
			{
                printf("lt:EPOLLIN socket disconnect\n");
				close(sockfd);
				continue;
			}
			printf("lt:EPOLLIN get %d bytes of content（%s）\n", ret, buf);
		}
		else
		{
			printf("lt:something else happened\n");
		}
	}
}

/*ET模式的工作流程*/
void et(epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE] = { 0 };
	for (int i = 0; i < number; i++)
	{
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd)
		{
			// 监听套接字上有新客户接入
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);

			// 对新客户连接开启ET模式
			addfd(epollfd, connfd, true);
		}
		else if (events[i].events & EPOLLIN)
		{
			// 这段代码不会被重复触发， 所以我们循环读取数据， 以确保把socket读取缓存中的所有数据
			printf("et:EPOLLIN event trigger once\n");
			while (1)
			{
				memset(buf, '\0', BUFFER_SIZE);
				int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
				if (ret < 0)
				{
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
					{
						printf("et:EPOLLIN read end\n");
						break;
					}
					close(sockfd);
					break;
				}
				else if (ret == 0)
				{
                    printf("et:EPOLLIN socket disconnect\n");
					close(sockfd);
				}
				else
				{
					printf("et:EPOLLIN get %d bytes of content（%s）\n", ret, buf);
				}
			}
		}
		else
		{
			printf("et:something else happened\n");
		}
	}
}

int main(int argc, char*argv[])
{
	if (argc <= 3)
	{
		printf("usage: %s ip_address port_number lt-or-et\n", basename(argv[0]));
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi(argv[2]);
	const char* choice = argv[3]; // lt or et

	int ret = 0;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);

	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);
	ret = listen(listenfd, 5);
	assert(ret != -1);

	epoll_event events[MAX_EVENT_NUMBER];
	int epollfd = epoll_create(5);
	assert(epollfd != -1);
	addfd(epollfd, listenfd, true);

	while (1)
	{
		int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
		if (ret < 0)
		{
			printf("epoll_wait ret < 0 \n");
			break;
		}

		if (0 == strcmp(choice, "lt"))
		{
			lt(events, ret, epollfd, listenfd);
		}
		else if (0 == strcmp(choice, "et"))
		{
			et(events, ret, epollfd, listenfd);
		}
		else
		{
			printf("choice error\n");
			break;
		}
	}

	close(listenfd);
	return 0;
}


