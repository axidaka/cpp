#include <stdio.h>

#include "TcpSock.h"
#include "thrift_server.h"

int AcceptHandler::onAccept(ServerSock* server_sock, int fd)
{
	// clientConn对象的delete在ClientConnHandler onClose中执行
	TcpSock* clientConn = new TcpSock(server_sock->getEvent_base(), fd);

	// clientConHandler智能指针对象存放在在clientConn中. clientConn析构的同时clientConHandler也会释放
	boost::shared_ptr<ClientConnHandler> clientConHandler(new ClientConnHandler(clientConn));
	clientConn->setTcpHandler(clientConHandler);

	printf("%s new client fd: %d\n", __func__, fd);
	return 1;
}

//======================================================================================

ClientConnHandler::ClientConnHandler(TcpSock* tcpSock)
	: oTransport(new TTcpSockTransport(tcpSock))
	, oProtocol(new TBinaryProtocol(oTransport))
{
}

int ClientConnHandler::onDataRecv(const char* data, int size)
{
	if (NULL == processor.get())
	{
		processor = shared_ptr<TProcessor>(new SharedServiceProcessor(shared_from_this()));
	}

	boost::shared_ptr<TTransport> trans(new TMemoryBuffer((uint8_t*)data, size));
	boost::shared_ptr<TBinaryProtocol> ip(new TBinaryProtocol(trans));
	processor->process(ip, oProtocol, NULL);

	return 0;
}

int ClientConnHandler::onClose(TcpSock *so, int err)
{
	printf("ClientConnHandler::%s \n", __func__);

	delete so;
	return 0;
}

void ClientConnHandler::getStruct(SharedStruct& _return, const int32_t key)
{
	_return.key = 1111;
	_return.value = "getStruct ";
	printf("thritf server::%s called\n", __func__);
}
