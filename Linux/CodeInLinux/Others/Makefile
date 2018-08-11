.PHONY: cleanall cleanobj

ALL_PROGRAM = main

all: ${ALL_PROGRAM}

CC = gcc
FLAGS = -g -Wall -ansi
PTHREAD_INCLUDE = /usr/include/nptl


src_others := main.cpp
src_others += use_pthread_key.cpp

obj_others := $(src_others:.cpp=.o)
obj_others := $(obj_others:.c=.o)

.cpp.o:
	${CC} -c -I${PTHREAD_INCLUDE} ${FLAGS} $<

.c.o:
	${CC} -c -I${PTHREAD_INCLUDE} ${FLAGS} $<
	
main: $(obj_others)
	${CC} -o $@ $(obj_others) -lpthread

cleanall: cleanobj
	rm -rf $(ALL_PROGRAM)

cleanobj: 
	rm -Rf $(obj_others) 