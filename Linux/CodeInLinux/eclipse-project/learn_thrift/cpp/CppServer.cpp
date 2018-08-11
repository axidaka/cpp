#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
//#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "../gen-cpp/Calculator.h"
#include "../gen-cpp/tutorial_types.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace tutorial;
using namespace shared;

using boost::shared_ptr;

class CalculatorHandler : public CalculatorIf {
public:
	CalculatorHandler() {}

	void ping() { cout << "ping()" << endl; }

	int32_t add(const int32_t n1, const int32_t n2) {
		cout << "add(" << n1 << ", " << n2 << ")" << endl;
		return n1 + n2;
	}

	int32_t calculate(const int32_t logid, const Work& work) {
	//	cout << "calculate(" << logid << ", " << work << ")" << endl;
		int32_t val;

		switch (work.op) {
		case Operation::ADD:
			val = work.num1 + work.num2;
			break;
		case Operation::SUBTRACT:
			val = work.num1 - work.num2;
			break;
		case Operation::MULTIPLY:
			val = work.num1 * work.num2;
			break;
		case Operation::DIVIDE:
			if (work.num2 == 0) {
				InvalidOperation io;
				io.whatOp = work.op;
				io.why = "Cannot divide by 0";
				throw io;
			}
			val = work.num1 / work.num2;
			break;
		default:
			InvalidOperation io;
			io.whatOp = work.op;
			io.why = "Invalid Operation";
			throw io;
		}

		SharedStruct ss;
		ss.key = logid;
		//ss.value = to_string(val);

		log[logid] = ss;

		return val;
	}

	void getStruct(SharedStruct& ret, const int32_t logid) {
		cout << "getStruct(" << logid << ")" << endl;
		ret = log[logid];
	}

	void zip() { cout << "zip()" << endl; }

protected:
	map<int32_t, SharedStruct> log;
};

static TServer* s_server = NULL;

class CalculatorCloneFactory : virtual public CalculatorIfFactory {
public:
	virtual ~CalculatorCloneFactory() {}
	virtual CalculatorIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
	{
		boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
		cout << "Incoming connection\n";
		cout << "\tSocketInfo: " << sock->getSocketInfo() << "\n";
		cout << "\tPeerHost: " << sock->getPeerHost() << "\n";
		cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
		cout << "\tPeerPort: " << sock->getPeerPort() << "\n";
		return new CalculatorHandler;
	}
	virtual void releaseHandler(::shared::SharedServiceIf* handler) {
		delete handler;
	}
};

void *wrap(void*)
{
	int port = 9090;
	shared_ptr<CalculatorHandler> handler(new CalculatorHandler());
	shared_ptr<TProcessor> processor(new CalculatorProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	//TThreadPoolServer server(processor, serverTransport, transportFactory, protocolFactory);


	TThreadedServer server(
		boost::make_shared<CalculatorProcessorFactory>(boost::make_shared<CalculatorCloneFactory>()),
		serverTransport, //port
		transportFactory,
		protocolFactory);

	s_server = &server;
	printf("Stating the server....\n");
	server.serve();

	printf("End the server....\n");
	s_server = NULL;

	return NULL;
}

int main(int argc, char **argv) {

	pthread_t pth = 0;
	pthread_create(&pth, 0, wrap, 0);
	sleep(3);

	printf("MainThread Run 1 min.......\n");
	sleep(60);	
	printf("MainThread End ....\n");
	
	//s_server->stop();
	pthread_join(pth, 0);
 	return 0;
}

