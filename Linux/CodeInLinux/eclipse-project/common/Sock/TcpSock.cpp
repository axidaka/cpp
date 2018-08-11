#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "TcpSock.h"
typedef struct sockaddr SA;

TcpSock::TcpSock(struct event_base* _base, int _fd)
	:base(_base)
	, buff_ev_io(NULL)
	, fd(_fd)
{
	if (NULL != base)
	{
		// 如何释放 buff_ev_read
		buff_ev_io = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		//size_t low, high;
		//bufferevent_getwatermark(buff_ev_read, EV_READ, &low, &high);
		//printf("bufferevent_getwatermark: %d--%d\n", low, high);
		//bufferevent_setwatermark(buff_ev_read, EV_READ, 25, 0);
		bufferevent_setcb(buff_ev_io, &(buffer_ev_io_cb<TcpSock, &TcpSock::socket_read_cb>), 
			&(buffer_ev_io_cb<TcpSock, &TcpSock::socket_write_cb>), &(buffer_ev_cb<TcpSock, &TcpSock::socket_event_cb>), this);
		bufferevent_enable(buff_ev_io, EV_READ);

	}
}

void TcpSock::socket_read_cb(struct bufferevent *bev)
{
	printf("TcpSock::%s\n", __func__);
	struct evbuffer* input = bufferevent_get_input(bev);
	size_t len = evbuffer_get_length(input);
	if (len)
	{
		char *buffer = new char[len + 1];
		memset(buffer, 0, len + 1);
		evbuffer_remove(input, buffer, len);
		
		if (NULL != tcpConHandler.get())
		{
			tcpConHandler->onDataRecv(buffer, len);
		}
		delete[]buffer;
	}
}

void TcpSock::socket_write_cb(struct bufferevent *bev)
{
	struct evbuffer* output = bufferevent_get_output(bev);
	size_t len = evbuffer_get_length(output);
	printf("TcpSock::%s output-length(%d)\n", __func__, len);
}

void TcpSock::socket_event_cb(struct bufferevent *bev, short what)
{
	printf("TcpSock::%s\n", __func__);
	struct evbuffer* input = bufferevent_get_input(bev);
	int finished = 0;

	if (what & BEV_EVENT_EOF)
	{
		size_t len = evbuffer_get_length(input);
		printf("TcpSock::%s BEV_EVENT_EOF  have %lu left\n", __func__, (unsigned long)len);
		finished = 1;
	}
	else if (what & BEV_EVENT_ERROR)
	{
		printf("TcpSock::%s  Got an error %s\n", __func__, evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
		finished = 1;
	}

	if (finished)
	{
		evutil_closesocket(fd);
		bufferevent_free(bev);
		if (NULL != tcpConHandler.get())
		{
			tcpConHandler->onClose(this, 0);
		}
	}

}

int TcpSock::sendMsg(const void* data, uint32_t length)
{
	if (NULL != buff_ev_io)
	{
		return bufferevent_write(buff_ev_io, data, length);
	}
	
	return -1;
}

//======================================================================================
ServerSock::ServerSock() :base(NULL), ev_listen(NULL), listener(-1), accepthandler(NULL)
{

}

ServerSock::~ServerSock()
{
	printf("~ServerSock\n");
    if (NULL != ev_listen)
    {
        event_free(ev_listen);
    }

}

int ServerSock::listenOn(int localIp, int localPort, int maxConn)
{
    int errno_save;
	
	listener = ::socket(AF_INET, SOCK_STREAM, 0);
	if (listener == -1)
		return -1;

	//允许多次绑定同一个地址。要用在socket和bind之间
	evutil_make_listen_socket_reuseable(listener);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = localIp;
	sin.sin_port = htons(localPort);

	if (::bind(listener, (SA*)&sin, sizeof(sin)) < 0)
		goto error;

	if (::listen(listener, maxConn) < 0)
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

int ServerSock::start()
{
    if ((NULL == base) || (-1 == listener))
        return -1;
	
	event_callback_fn pf = &(ev_io_cb<ServerSock, &ServerSock::accept_cb>);
	ev_listen = event_new(base, listener, EV_READ | EV_PERSIST, pf, this);
	event_add(ev_listen, NULL);
	return 0;
}

void ServerSock::accept_cb(int fd, short events)
{
    evutil_socket_t sockfd;

	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	sockfd = ::accept(fd, (struct sockaddr*)&client, &len);

	if (sockfd != -1)
	{
		printf("accept a client %d\n", sockfd);

		evutil_make_socket_nonblocking(sockfd);
		if (!accepthandler || accepthandler->onAccept(this, sockfd) < 0){
			::close(sockfd);
		}
	}

}