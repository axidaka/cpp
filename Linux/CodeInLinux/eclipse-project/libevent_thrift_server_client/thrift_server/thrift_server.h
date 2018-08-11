#ifndef __THRIFT_SERVER_HEADER__
#define __THRIFT_SERVER_HEADER__

#include "SockHandlerIf.h"
#include "thrift_server/TTcpSockTransport.h"
#include "gen-cpp/SharedService.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <boost/enable_shared_from_this.hpp>
#include <list>


using namespace std;
using namespace shared;
using boost::shared_ptr;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

class AcceptHandler : public ITcpAccept
{
public:

	AcceptHandler(){}
	~AcceptHandler(){ printf("~AcceptHandler\n"); }

	virtual int onAccept(ServerSock* server_sock, int fd);
};

// Server端 用于处理Client I/O 事件
class ClientConnHandler : public ITcpHandler
						, public SharedServiceIf
						, public boost::enable_shared_from_this<ClientConnHandler>
{
public:

	ClientConnHandler(TcpSock* tcpSock);
	virtual ~ClientConnHandler(){ printf("~ClientConnHandler\n"); }

	// override from ITcpHandler
	virtual int onDataRecv(const char*, int size);
	virtual int onClose(TcpSock *so, int err);

	// override from SharedServiceIf
	virtual void getStruct(SharedStruct& _return, const int32_t key);

private:
	shared_ptr<TProcessor> processor;
	shared_ptr<TTcpSockTransport> oTransport;   // 用于发送thrift请求的响应 通道,封装了自己的sock
	shared_ptr<TBinaryProtocol>   oProtocol;    // 用于将thrift请求的响应序列化
};

#endif
