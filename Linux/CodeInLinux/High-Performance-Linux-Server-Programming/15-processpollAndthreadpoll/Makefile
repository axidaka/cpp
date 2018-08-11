export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I. -I/usr/include
LIBS := $(LIBS) -lpthread

PROGRAM= threadpoll_main

all:  $(PROGRAM) 

src_tp_main := threadpoll_main.cpp
obj_tp_main := $(src_tp_main:.cpp=.o)
obj_tp_main := $(obj_tp_main:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(PROGRAM): $(obj_tp_main)
	$(CXX32) -o $@ $(obj_tp_main) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(obj_tp_main) $(PROGRAM) $(HIREDIS_SO)