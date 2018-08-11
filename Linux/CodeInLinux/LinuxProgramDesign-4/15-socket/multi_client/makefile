ALL_PROGRAM=fork_server client select_server

all: $(ALL_PROGRAM)

# Options for development
CFLAGS = -g -Wall -std=gnu89

# Options for release
# CFLAGS = -O -Wall -std=gnu89

.c.o: 
	gcc $(CFLAGS) -c $<

fork_server.o: fork_server.c
client.o: client.c
select_server.o: select_server.c

fork_server: fork_server.o
	gcc -o fork_server fork_server.o

client: client.o
	gcc -o client client.o

select_server: select_server.o
	gcc -o select_server select_server.o
	
cleanall: cleanobj 
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o
