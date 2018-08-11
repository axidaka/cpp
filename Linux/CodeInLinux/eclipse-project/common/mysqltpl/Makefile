topdir=../..
toplib=$(topdir)/lib


ARRU = ar -ru

CXXFLAGS =  -pthread -Wall -ggdb -I$(topdir) -I$(topdir)/common
CXXFLAGS_R = -pthread -Wall -O2 -O3 -I$(topdir)  -I$(topdir)/common
#-fomit-frame-pointer

SRCS = 	MySQLDriver.cpp MySQLFactory.cpp MySQLTemplate.cpp


OBJS = $(SRCS:.cpp=.o)

OBJS_R = $(SRCS:.cpp=.ro)


.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.SUFFIXES: .o .cpp
.cpp.ro:
	$(CXX32) $(CXXFLAGS_R) -c -o $@ $<

all: libmysqltpl.a
release: mysqltpl_r.a

libmysqltpl.a: $(OBJS) 
	$(ARRU) $(toplib)/$@ $(OBJS)
	ranlib $(toplib)/$@
	
mysqltpl_r.a: $(OBJS_R) 
	$(ARRU) $< $(toplib)/$@ $(OBJS_R)
	ranlib $(toplib)/$@

clean:
	rm -f *.o
	rm -f *.ro
	rm -f $(toplib)/libmysqltpl.a $(toplib)/libmysqltpl_r.a

install:

distclean: clean
	rm -f .depend

#	vim: set ts=4 sts=4 syn=make :
