ALL_PROGRAM=connect-timeout server-remove-nonactiveclient-timer

all: $(ALL_PROGRAM)

#Options for release
#CFLAGS = -O -Wall

#Options for development
CFLAGS = -g -Wall

.c.o:
	g++ $(CFLAGS) -c $<

connect-timeout.o: connect-timeout.c
nonactive_conn.o: nonactive_conn.c lst_timer.h

connect-timeout: connect-timeout.o
	g++ -o connect-timeout connect-timeout.o

server-remove-nonactiveclient-timer: nonactive_conn.o
	g++ -o server-remove-nonactiveclient-timer nonactive_conn.o

cleanall: cleanobj
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o
