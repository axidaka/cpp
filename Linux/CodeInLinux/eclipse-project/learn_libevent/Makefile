export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I$(make_basedir)/common/libevent/include/ -I$(make_basedir)/common/libevent/build/include \
			-I$(make_basedir)/common/Sock
LDFLAGS := $(LDFLAGS) -L$(make_basedir)/common/libevent/build/lib  -L$(make_basedir)/lib
LIBS := $(LIBS) -levent -levent_core -levent_extra -lsock

LEARN_LIBEVENT=learn_libevent
SIMPLE_SERVER=simple_server
SIMPLE_CLIENT=simple_client
USER_BUFFEREVENT=use_bufferevent
USE_BUFFEREVENT_SERVER=use_bufferevent_server
USE_BUFFEREVENT_HTTP_CLIENT=use_bufferevent_http_client

all:  $(LEARN_LIBEVENT) $(SIMPLE_SERVER) $(SIMPLE_CLIENT) $(USER_BUFFEREVENT) $(USE_BUFFEREVENT_SERVER) $(USE_BUFFEREVENT_HTTP_CLIENT)

src_main := src/main.cpp
obj_main := $(src_main:.cpp=.o)
obj_main := $(obj_main:.c=.o)

src_simple_server := src/simple_server.cpp
obj_simple_server := $(src_simple_server:.cpp=.o)
obj_simple_server := $(obj_simple_server:.c=.o)

src_simple_client := src/simple_client.cpp
obj_simple_client := $(src_simple_client:.cpp=.o)
obj_simple_client := $(obj_simple_client:.c=.o)

src_use_bufferevent := src/use_bufferevent.cpp
obj_use_bufferevent := $(src_use_bufferevent:.cpp=.o)
obj_use_bufferevent := $(obj_use_bufferevent:.c=.o)

src_use_bufferevent_server := src/use_bufferevent_server.cpp
obj_use_bufferevent_server := $(src_use_bufferevent_server:.cpp=.o)
obj_use_bufferevent_server := $(obj_use_bufferevent_server:.c=.o)

src_use_bufferevent_http_client := src/use_bufferevent_http_client.cpp
obj_use_bufferevent_http_client := $(src_use_bufferevent_http_client:.cpp=.o)
obj_use_bufferevent_http_client := $(obj_use_bufferevent_http_client:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(LEARN_LIBEVENT): $(obj_main)
	$(CXX32) -o $@ $(obj_main) $(LDFLAGS) $(LIBS)

$(SIMPLE_SERVER): $(obj_simple_server)
	$(CXX32) -o $@ $(obj_simple_server) $(LDFLAGS) $(LIBS)
	
$(SIMPLE_CLIENT): $(obj_simple_client)
	$(CXX32) -o $@ $(obj_simple_client) $(LDFLAGS) $(LIBS)

$(USER_BUFFEREVENT): $(obj_use_bufferevent)
	$(CXX32) -o $@ $(obj_use_bufferevent) $(LDFLAGS) $(LIBS)

$(USE_BUFFEREVENT_SERVER): $(obj_use_bufferevent_server)
	$(CXX32) -o $@ $(obj_use_bufferevent_server) $(LDFLAGS) $(LIBS)
	
$(USE_BUFFEREVENT_HTTP_CLIENT): $(obj_use_bufferevent_http_client)
	$(CXX32) -o $@ $(obj_use_bufferevent_http_client) $(LDFLAGS) $(LIBS)
	
clean:
	rm -f $(obj_main) $(obj_simple_server) $(obj_simple_client） $(obj_use_bufferevent) $(obj_use_bufferevent_server) $(obj_use_bufferevent_http_client  \
	$(LEARN_LIBEVENT) $(SIMPLE_SERVER) $(SIMPLE_CLIENT) $(USER_BUFFEREVENT) $(USE_BUFFEREVENT_SERVER) $(USE_BUFFEREVENT_HTTP_CLIENT)