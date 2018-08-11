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
#include <signal.h>

#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

/*将文件描述符设置成非阻塞*/
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

/*将文件描述符fd上的EPOLLIN注册到epollfd指示的内核事件表中*/
void addfd(int epollfd, int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;

	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

/*信号处理函数*/
void sig_handler(int sig)
{
	/*保留原来的errno, 在函数最后恢复, 以保证函数的可重入性*/
	int save_errno = errno;
	int msg = sig;
	send(pipefd[1], (char*)&msg, 1, 0); // 将信号写入管道, 以通知主循环
	errno = save_errno;
}

void addsig(int sig)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = sig_handler;
	sa.sa_flags |= SA_RESTART;
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char*argv[])
{
	if (argc <= 2)
	{
		printf("usage: %s ip_address port_number\n", basename(argv[0]));
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi(argv[2]);

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
	addfd(epollfd, listenfd);

	/*使用socketpair创建管道, 注册pipfd[0]上的可读事件*/
	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
	assert(ret != -1);
	setnonblocking(pipefd[1]);
	addfd(epollfd, pipefd[0]);

	/*设置一些信号的处理函数*/
	addsig(SIGHUP);
	addsig(SIGCHLD);
	addsig(SIGTERM);
	addsig(SIGINT); 
	
	bool stop_server = false;

	while (!stop_server)
	{
		int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
		if ((ret < 0) && (errno != EINTR))
		{
			printf("epoll_wait ret < 0 failure \n");
			break;
		}

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
				addfd(epollfd, connfd);
			}
			else if (sockfd == pipefd[0] && events[i].events & EPOLLIN)
			{
				char signals[1024];
				ret = recv(sockfd, signals, sizeof(signals), 0);
				printf("signal pipefd[0] %s\n", signals);
                if (ret == -1)
				{
					continue;
				}
				else if (ret == 0)
				{
					continue;
				}
				else
				{
					/*每个信号值占1个字节, 所以按字节来逐个接收信号*/
					for (int i = 0; i < ret; i++)
					{
						switch (signals[i])
						{
						case  SIGCHLD:
						case  SIGHUP:
						{
							continue;
						}
						case SIGTERM:
						case SIGINT:
						{
							stop_server = true;
                            printf("signals SIGTERM or SIGINT\n");
						}
						}
					}
				}
			}
			else
			{
				printf("et:something else happened\n");
			}
		}

	}

	printf("close fds\n");
	close(listenfd);
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}


