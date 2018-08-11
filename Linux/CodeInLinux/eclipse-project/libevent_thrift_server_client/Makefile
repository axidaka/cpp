export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I. -I./thrift_server -I$(make_basedir)/common -I$(make_basedir)/common/libevent/include/ \
			-I$(make_basedir)/common/libevent/build/include \
			-I$(make_basedir)/common/Sock -I$(make_basedir)/common/thrift-0.9.1/ 

LDFLAGS := $(LDFLAGS) -L$(make_basedir)/common/libevent/build/lib  -L$(make_basedir)/lib \
			-L$(make_basedir)/common/thrift-0.9.1/lib

LIBS := $(LIBS) -lsock -levent -levent_core -levent_extra -lthrift

SERVER=server
CLIENT=client

all:   $(SERVER) $(CLIENT) 

gen_cpp :=  gen-cpp/shared_types.cpp  gen-cpp/SharedService.cpp  gen-cpp/shared_constants.cpp 

src_server := src/server.cpp thrift_server/thrift_server.cpp thrift_server/TTcpSockTransport.cpp $(gen_cpp)
obj_server := $(src_server:.cpp=.o)
obj_server := $(obj_server:.c=.o)

src_client := src/client.cpp $(gen_cpp)
obj_client := $(src_client:.cpp=.o)
obj_client := $(obj_client:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(SERVER): $(obj_server)
	$(CXX32) -o $@ $(obj_server) $(LDFLAGS) $(LIBS)

$(CLIENT): $(obj_client)
	$(CXX32) -o $@ $(obj_client) $(LDFLAGS) $(LIBS)
	

clean:
	rm -f $(obj_server) $(obj_client)  $(SERVER) $(CLIENT)