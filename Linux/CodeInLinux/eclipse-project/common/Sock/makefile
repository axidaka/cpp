
TOPDIR=../../

include $(TOPDIR)/Makefile.inc

COMMONDIR=../
LIBDIR=$(TOPDIR)lib/
SRCDIR=$(TOPDIR)src
INCLUDE= -I$(COMMONDIR) -I$(COMMONDIR)libevent/include -I$(COMMONDIR)/libevent/build/include
LDFLAGS= -L$(COMMONDIR)libevent/build/lib
LIBS=-levent -levent_core -levent_extra

src_sock := TcpSock.cpp 

obj_sock := $(src_sock:.cpp=.o)
obj_sock := $(obj_sock:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

all: libsock.a

libsock.a: $(obj_sock)
	$(ARRU) $(LIBDIR)/$@  $^

clean:
	rm -f  $(obj_sock) $(LIBDIR)/libsox.a

