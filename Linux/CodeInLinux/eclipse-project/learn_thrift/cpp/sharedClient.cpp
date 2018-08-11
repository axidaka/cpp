#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "../gen-cpp/SharedService.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace shared;

int main() {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
	SharedServiceClient client(protocol);

	try {
		transport->open();

		// Note that C++ uses return by reference for complex types to avoid
		// costly copy construction
		SharedStruct ss;
		client.getStruct(ss, 1);
		cout << "Received log: " << ss.key << ss.value << endl;

		transport->close();
	}
	catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}
