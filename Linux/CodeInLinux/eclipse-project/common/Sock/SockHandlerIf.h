#ifndef __SOCK_HANDLER_IF_HEADER__
#define __SOCK_HANDLER_IF_HEADER__

class ServerSock;
class TcpSock;


// Client_Connection I/O event handler
struct ITcpHandler
{
	virtual ~ITcpHandler(){ printf("~ITcpHandler\n"); };

	/*
	return >=0 the cosume bytes
	-1 the upper layer has close the socket so the lower should do nothing
	*/
	virtual int onDataRecv(const char* ,int) = 0;
	virtual int onClose(TcpSock *, int err) = 0;
};

// Tcp Server Accept CallBack 
struct ITcpAccept
{
	virtual ~ITcpAccept(){ printf("~ITcpAccept\n"); };
	virtual int onAccept(ServerSock* server_sock, int fd) = 0;
};

#endif