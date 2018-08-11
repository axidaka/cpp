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
		inf->total_drained += len;
		evbuffer_drain(input, len);
		printf("Drained %lu bytes from %s\n",
			(unsigned long)len, inf->name);
	}
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
	struct bufferevent *bev;
	struct sockaddr_in sin;

	base = event_base_new();
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(9999);
	
	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setwatermark(bev, EV_READ, 25, 0);

	struct info * info1;
	info1 = (info*)malloc(sizeof(struct info));
	info1->name = "Buffer 1";
	info1->total_drained = 0;

	bufferevent_setcb(bev, read_callback, NULL, event_cb, info1);

	if (bufferevent_socket_connect(bev, (struct sockaddr*)&sin, sizeof(sin)) < 0)
	{
		bufferevent_free(bev);
		return -1;
	}

	bufferevent_enable(bev, EV_READ);

	event_base_dispatch(base);
	return 0;
}