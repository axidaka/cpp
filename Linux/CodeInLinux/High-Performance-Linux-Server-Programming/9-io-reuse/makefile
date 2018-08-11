ALL_PROGRAM=server-select-oob server-epoll-lt-et poll-chat-client poll-chat-server

all:$(ALL_PROGRAM)

#Options for development
CFLAGS = -g -Wall

#Options for release
#CFLAGS = -O -Wall

.c.o:
	g++ $(CFLAGS) -c $<

server-select-oob.o: server-select-oob.c
server-epoll-lt-et.o: server-epoll-lt-et.c
poll-chat-client.o: poll-chat-client.c
poll-chat-server.o: poll-chat-server.c

server-select-oob: server-select-oob.o
	g++ -o server-select-oob server-select-oob.o

server-epoll-lt-et: server-epoll-lt-et.o
	g++ -o server-epoll-lt-et server-epoll-lt-et.o

poll-chat-client: poll-chat-client.o
	g++ -o poll-chat-client poll-chat-client.o

poll-chat-server: poll-chat-server.o
	g++ -o poll-chat-server poll-chat-server.o

cleanall: cleanobj
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o

	