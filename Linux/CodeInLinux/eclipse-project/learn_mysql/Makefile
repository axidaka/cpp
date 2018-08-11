export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) -I./h -I$(make_basedir)/common/mysqltpl -I$(make_basedir)/common/mysql
LDFLAGS := $(LDFLAGS) -L$(make_basedir)/lib -L/usr/lib/i386-linux-gnu
LIBS := $(LIBS) -lmysqltpl -lmysqlclient_r

LEARN_MYSQL=learn_mysql_d

all:  $(LEARN_MYSQL) 

src_main := src/main.cpp src/SimpleMySQLFactory.cpp
obj_main := $(src_main:.cpp=.o)
obj_main := $(obj_main:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
$(LEARN_MYSQL): $(obj_main)
	$(CXX32) -o $@ $(obj_main) $(LDFLAGS) $(LIBS)
	
clean:
	rm -f $(obj_main) $(LEARN_MYSQL)