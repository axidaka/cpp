ALL_PROGRAM =  fifo_server fifo_client

all: $(ALL_PROGRAM)

fifo_server: fifo_server_client/fifo_server.o 
	gcc -o fifo_server_client/fifo_server fifo_server_client/fifo_server.o

fifo_server.o: fifo_sever_client/fifo_server.c fifo_sever_client/datatype.h
	gcc -c fifo_server_client/fifo_server.c
	

fifo_client: fifo_server_client/fifo_client.o 
	gcc -o fifo_server_client/fifo_client fifo_server_client/fifo_client.o

fifo_client.o: fifo_sever_client/fifo_client.c fifo_sever_client/datatype.h
	gcc -c fifo_server_client/fifo_client.c