export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I. -I/usr/include -I$(make_basedir)/common
LIBS := $(LIBS) -lpthread

PROGRAM=main

all:  $(PROGRAM) 

src_main := src/main.cpp src/Thread.cpp
obj_main := $(src_main:.cpp=.o)
obj_main := $(obj_main:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(PROGRAM): $(obj_main)
	$(CXX32) -o $@ $(obj_main) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(obj_main) $(PROGRAM) $(HIREDIS_SO)