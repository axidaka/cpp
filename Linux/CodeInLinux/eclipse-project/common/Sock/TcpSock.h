#ifndef __TCP_SOCK_HEADER__
#define __TCP_SOCK_HEADER__

#include <boost/enable_shared_from_this.hpp>

#include "SockHandlerIf.h"

// libevent
#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"

template<class T, void (T::*method)(int, short)> 
void ev_io_cb (evutil_socket_t fd, short events, void *arg)
{
	T *o = static_cast<T*>(arg);
	(o->*method)(fd, events);
}

template<class T, void (T::*method)(struct bufferevent*)>
void buffer_ev_io_cb(struct bufferevent *bev, void *ctx)
{
	T* o = static_cast<T*>(ctx);
	(o->*method)(bev);
}

template<class T, void(T::*method)(struct bufferevent* , short)>
void buffer_ev_cb(struct bufferevent *bev, short what, void *ctx)
{
	T* o = static_cast<T*>(ctx);
	(o->*method)(bev, what);
}

class TcpSock
{
public:
	TcpSock(struct event_base* _base, int _fd);
	~TcpSock(){ printf("~TcpSock\n"); }

	void setTcpHandler(boost::shared_ptr<ITcpHandler> tif){
		tcpConHandler = tif;
	}

	/*
	= 0 send ok
	<  errno occur
	*/
	virtual int sendMsg(const void* data, uint32_t length);

protected:

	void socket_read_cb (struct bufferevent *bev);
	void socket_write_cb(struct bufferevent *bev);
	void socket_event_cb(struct bufferevent *bev, short what);

private:

	struct event_base* base;
	struct bufferevent* buff_ev_io;

	evutil_socket_t fd;
	boost::shared_ptr<ITcpHandler> tcpConHandler;
};

// Tcp Server 将客户端的连接对象管理交托给accepthandler
class ServerSock
{
public:
	ServerSock();
	~ServerSock();
	
	int listenOn(int localIp, int localPort, int maxConn = SOMAXCONN);
	void setTcpAccept(ITcpAccept *tif){
		accepthandler = tif;
	}
	void setEvent_base(struct event_base* _base){
		base = _base;
	}
	struct event_base* getEvent_base(){
		return base;
	}
	int start();

protected:
	void accept_cb(int fd, short events);
private:

	struct event_base* base;
	struct event* ev_listen;
	evutil_socket_t listener;
	ITcpAccept* accepthandler;

};


#endif