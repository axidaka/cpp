ALL_PROGRAM=signal-epoll-server

all:$(ALL_PROGRAM)

#Options for development
CFLAGS = -g -Wall

#Options for release
#CFLAGS = -O -Wall

.c.o:
	g++ $(CFLAGS) -c $<

signal-epoll-server.o: signal-epoll-server.c

signal-epoll-server: signal-epoll-server.o
	g++ -o signal-epoll-server signal-epoll-server.o

cleanall: cleanobj
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o

	