
.PHONY: cleanall cleanobj 

include ./fifo_server_client/fifo_s_c.mk

ALL_PROGRAM =  popen_read popen_write  pipe_readwrite pipe_fork fifo1 openfifo \
	 fifo_producer fifo_consumer 

all: $(ALL_PROGRAM)  

popen_read: popen_read.o
	gcc -o popen_read popen_read.o 

popen_read.o: popen_read.c 
	gcc -c popen_read.c

popen_write: popen_write.o
	gcc -o popen_write popen_write.o

popen_write.o: popen_write.c
	gcc -c popen_write.c

popen_readhuge: popen_readhuge.o
	gcc -o popen_readhuge popen_readhuge.o

popen_readhuge.o: popen_readhuge.c
	gcc -c popen_readhuge.c

pipe_readwrite: pipe_readwrite.o
	gcc -o pipe_readwrite pipe_readwrite.o

pipe_readwrite.o: pipe_readwrite.c
	gcc -c pipe_readwrite.c

pipe_fork: pipe_fork.o
	gcc -o pipe_fork pipe_fork.o

pipe_fork.o: pipe_fork.c
	gcc -c pipe_fork.c
	
fifo1: fifo1.o

openfifo: openfifo.o
	gcc -o openfifo openfifo.o

openfifo.o: openfifo.c
	gcc -c openfifo.c

fifo_producer: fifo_producer.o
	gcc -o fifo_producer fifo_producer.o

fifo_producer.o: fifo_producer.c
	gcc -c fifo_producer.c

fifo_consumer: fifo_consumer.o
	gcc -o fifo_consumer fifo_consumer.o

fifo_consumer.o: fifo_consumer.c
	gcc -c fifo_consumer.c


cleanall: cleanobj 
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o
