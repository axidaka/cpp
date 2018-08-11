export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I$(make_basedir)/common
LDFLAGS := $(LDFLAGS) -L$(make_basedir)/common/hiredis
LIBS := $(LIBS) -lhiredis 

PROGRAM=learn_redis

all:  $(PROGRAM) 

src_main := src/main.cpp
obj_main := $(src_main:.cpp=.o)
obj_main := $(obj_main:.c=.o)

.cpp.o:
	$(CXX32) $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(PROGRAM): $(obj_main)
	$(CXX32) -o $@ $(obj_main) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(obj_main) $(PROGRAM) $(HIREDIS_SO)