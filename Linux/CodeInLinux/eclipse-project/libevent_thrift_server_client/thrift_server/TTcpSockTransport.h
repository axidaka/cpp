#ifndef __TTCPSOCK_TRANSPORT_HEADER__ 
#define __TTCPSOCK_TRANSPORT_HEADER__

#include <thrift/transport/TBufferTransports.h>

class TcpSock;

class TTcpSockTransport : public apache::thrift::transport::TTransport
{
public:
	TTcpSockTransport(TcpSock* tcpsock);
	~TTcpSockTransport();

	virtual void write_virt(const uint8_t*  buf, uint32_t len) {
		buffer.write(buf, len);
	}

	virtual uint32_t writeEnd();

	apache::thrift::transport::TMemoryBuffer *getBufferPtr(){
		return &buffer;
	}

private:
	TcpSock* tcpsock;
	apache::thrift::transport::TMemoryBuffer buffer;
};


#endif

