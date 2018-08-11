#include "TTcpSockTransport.h"
#include "TcpSock.h"

TTcpSockTransport::TTcpSockTransport(TcpSock* _tcpsock)
	:tcpsock(_tcpsock)
{
}


TTcpSockTransport::~TTcpSockTransport()
{
}

uint32_t TTcpSockTransport::writeEnd()
{
	uint8_t *buf;
	uint32_t size;
	buffer.getBuffer(&buf, &size);

	if (tcpsock)
	{
		int ret = tcpsock->sendMsg((char *)buf, size);
		if (ret >= 0){
			buffer.resetBuffer(true);
			return size;
		}
		else{
			printf("%s sendMsg error\n", __func__);
		}
	}
	else
	{
		printf("%s sock is null\n", __func__);
	}

	return 0;
}