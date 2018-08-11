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
#include "event2/dns.h"
#include "event2/util.h"
#include "event2/buffer.h"

void readcb(struct bufferevent *bev, void *ctx)
{
	char buf[1024] = { 0 };
	int n;
	struct evbuffer* input = bufferevent_get_input(bev);
	while ((n = evbuffer_remove(input, buf, sizeof(buf))) > 0)
	{
		fwrite(buf, 1, n, stdout);
	}
}

void event_cb(struct bufferevent *bev, short what, void *ptr)
{
	if (what & BEV_EVENT_CONNECTED)
	{
		printf("%s :we connet server sucessfull\n", __FUNCTION__);
	}
	else if (what & (BEV_EVENT_ERROR | BEV_EVENT_EOF))
	{
		struct event_base *base = (event_base*)ptr;
		if (what & BEV_EVENT_ERROR)
		{
			int err = bufferevent_socket_get_dns_error(bev);
			if (err)
			{
				printf("DNS error: %s\n", evutil_gai_strerror(err));
			}

			printf("Cloing \n");
			bufferevent_free(bev);
			event_base_loopexit(base, NULL);
		}
	}
}

int main(int argc , char** argv)
{
	struct event_base *base;
	struct evdns_base *dns_base;
	struct bufferevent *bev;

	if (argc != 3)
	{
		printf("Trivial HTTP 0.x client\n"
			"Syntax: %s [hostname] [resource]\n"
			"Example: %s www.google.com /\n", argv[0], argv[0]);
		return 1;
	}

	base = event_base_new();
	dns_base = evdns_base_new(base, 1);
	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, readcb, NULL, event_cb, base);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	evbuffer_add_printf(bufferevent_get_output(bev), "GET %s\r\n", argv[2]);
	bufferevent_socket_connect_hostname(bev, dns_base, AF_UNSPEC, argv[1], 80);

	event_base_dispatch(base);
	return 0;
}
