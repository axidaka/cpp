#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"

int tcp_server_init(int port, int listen_num);
void accept_cb(int fd, short events, void* arg);
void event_cb(struct bufferevent *bev, short what, void *ctx);

struct info{
	const char* name;
	size_t total_drained;
};

void read_callback(struct bufferevent* bev, void* ctx)
{
	struct info* inf = (info*)ctx;
	struct evbuffer* input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	if (len)
	{
		char *buffer = new char[len + 1];
		memset(buffer, 0, len + 1);
		inf->total_drained += len;
		evbuffer_remove(input, buffer, len);
		printf("evbuffer_remove %lu bytes(%s) from %s\n",
			(unsigned long)len, buffer, inf->name);
		bufferevent_write(bev, "123", 3);
		delete []buffer;
	}
}
void write_callback(struct bufferevent* bev, void* ctx)
{
	struct evbuffer* output = bufferevent_get_output(bev);
	size_t len = evbuffer_get_length(output);
	printf("%s output-length(%d)\n", __func__, len);
}


void event_cb(struct bufferevent *bev, short what, void *ctx)
{
	struct info* inf = (info*)ctx;
	struct evbuffer* input = bufferevent_get_input(bev);
	int finished = 0;

	if (what & BEV_EVENT_EOF)
	{
		size_t len = evbuffer_get_length(input);
		printf("Got a close from %s . We drained %lu bytes from it, "
			"and have %lu left\n", inf->name,
			(unsigned long)inf->total_drained, (unsigned long)len);
		finished = 1;
	}
	else if (what & BEV_EVENT_ERROR)
	{
		printf("Got an error from %s: %s\n",
			inf->name, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		finished = 1;
	}

	if (finished)
	{
		free(ctx);
		bufferevent_free(bev);
	}

}

int main()
{
	struct event_base *base;
	struct event* listen_event;

	// 1.create listen socket
	int listener = tcp_server_init(9999, 10);
	if (listener == -1)
	{
		perror(" tcp_server_init error ");
		return -1;
	}

	printf("listening.....\n");

	// 2.create event_base event
	base = event_base_new();
	listen_event = event_new(base, listener, EV_READ | EV_PERSIST, accept_cb, (void*)base);
	event_add(listen_event, NULL);
	
	event_base_dispatch(base);
	return 0;
}


typedef struct sockaddr SA;
int tcp_server_init(int port, int listen_num)
{
	int errno_save;
	evutil_socket_t listener;

	listener = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		return -1;

	//允许多次绑定同一个地址。要用在socket和bind之间
	evutil_make_listen_socket_reuseable(listener);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(port);

	if (::bind(listener, (SA*)&sin, sizeof(sin)) < 0)
		goto error;

	if (::listen(listener, listen_num) < 0)
		goto error;


	//跨平台统一接口，将套接字设置为非阻塞状态
	evutil_make_socket_nonblocking(listener);

	return listener;

error:
	errno_save = errno;
	evutil_closesocket(listener);
	errno = errno_save;

	return -1;
}

void accept_cb(int fd, short events, void* arg)
{
	evutil_socket_t sockfd;

	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	sockfd = ::accept(fd, (struct sockaddr*)&client, &len);
	evutil_make_socket_nonblocking(sockfd);

	printf("accept a client %d\n", sockfd);
	//write(sockfd, "Hello client", sizeof("Hello client"));
	struct event_base* base = (event_base*)arg;

	struct bufferevent* bev = bufferevent_socket_new(base, sockfd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setwatermark(bev, EV_READ, 25, 0);
	
	struct info * info1;
	info1 = (info*)malloc(sizeof(struct info));
	info1->name = "Buffer 1";
	info1->total_drained = 0;

	bufferevent_setcb(bev, read_callback, write_callback, event_cb, info1);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}