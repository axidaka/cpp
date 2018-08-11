ALL_PROGRAM= sem1 shm_consumer shm_producer msgqueue_recv msgqueue_send

all: $(ALL_PROGRAM)

# 所有的.o文件都是依赖于对应的.c文件
.c.o:
	gcc -c $<

sem1.o: semun.h
shm_consumer.o: shm_consumer.c shm_com.h
shm_producer.o: shm_producer.c shm_com.h
msgqueue_recv.o: msgqueue_recv.c
msgqueue_send.o: msgqueue_send.c

sem1: sem1.o
	gcc -o sem1 sem1.o

shm_consumer: shm_consumer.o
	gcc -o shm_consumer shm_consumer.o 

shm_producer: shm_producer.o
	gcc -o shm_producer shm_producer.o

msgqueue_recv: msgqueue_recv.o
	gcc -o msgqueue_recv msgqueue_recv.o
	
msgqueue_send: msgqueue_send.o
	gcc -o msgqueue_send msgqueue_send.o
	
cleanall: cleanobj 
	rm -rf $(ALL_PROGRAM)

cleanobj:
	rm -Rf *.o