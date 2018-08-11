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

// Server�� ���ڴ���Client I/O �¼�
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
	shared_ptr<TTcpSockTransport> oTransport;   // ���ڷ���thrift�������Ӧ ͨ��,��װ���Լ���sock
	shared_ptr<TBinaryProtocol>   oProtocol;    // ���ڽ�thrift�������Ӧ���л�
};

#endif
