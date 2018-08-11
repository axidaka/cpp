export make_basedir = ..

include $(make_basedir)/Makefile.inc

INCLUDE := $(INCLUDE) ./gen-cpp -I$(make_basedir)/common -I$(make_basedir)/common/thrift-0.9.1/ 
LDFLAGS := $(LDFLAGS) -L$(make_basedir)/common/thrift-0.9.1/lib
LIBS := $(LIBS) -lthrift

PROGRAM= sharedClient sharedServer CppClient  CppServer

all:  $(PROGRAM) 

gen_cpp :=  gen-cpp/shared_types.cpp  gen-cpp/SharedService.cpp  gen-cpp/shared_constants.cpp \
			gen-cpp/tutorial_constants.cpp gen-cpp/tutorial_types.cpp gen-cpp/Calculator.cpp 

src_CppClient := cpp/CppClient.cpp $(gen_cpp)
obj_CppClient := $(src_CppClient:.cpp=.o)
obj_CppClient := $(obj_CppClient:.c=.o)

src_CppServer := cpp/CppServer.cpp $(gen_cpp)
obj_CppServer := $(src_CppServer:.cpp=.o)
obj_CppServer := $(obj_CppServer:.c=.o)

src_sharedClient := cpp/sharedClient.cpp $(gen_cpp)
obj_sharedClient := $(src_sharedClient:.cpp=.o)
obj_sharedClient := $(obj_sharedClient:.c=.o)

src_sharedServer := gen-cpp/SharedService_server.skeleton.cpp $(gen_cpp)
obj_sharedServer := $(src_sharedServer:.cpp=.o)
obj_sharedServer := $(obj_sharedServer:.c=.o)

.cpp.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<

.c.o:
	$(CXX32) -g $(INCLUDE) $(CXXFLAGS) -c -o $@ $<
	
CppClient: $(obj_CppClient)
	$(CXX32) -o $@ $(obj_CppClient) $(LDFLAGS) $(LIBS)

CppServer: $(obj_CppServer)
	$(CXX32) -o $@ $(obj_CppServer) $(LDFLAGS) $(LIBS) -lpthread

sharedServer: $(obj_sharedServer)
	$(CXX32) -o $@ $(obj_sharedServer) $(LDFLAGS) $(LIBS)

sharedClient: $(obj_sharedClient)
	$(CXX32) -o $@ $(obj_sharedClient) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(PROGRAM) $(obj_sharedClient) $(obj_sharedServer) $(obj_CppClient) $(obj_CppServer) 