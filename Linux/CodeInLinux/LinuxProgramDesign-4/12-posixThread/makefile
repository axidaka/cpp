
all: thread1 thread2 thread3 thread4 thread5 thread6 cond_wait

thread1: thread1.o
	gcc -o thread1 thread1.o -lpthread

thread1.o: thread1.c 
	gcc -D _REENTRANT -I/usr/include/nptl -c thread1.c -L/usr/lib/nptl 

thread2: thread2.o
	gcc -o thread2 thread2.o -lpthread

thread2.o: thread2.c
	gcc -D _REENTRANT -I/usr/include/nptl -c thread2.c -L/usr/lib/nptl

thread3: thread3.o
	gcc -o thread3 thread3.o -lpthread

thread3.o: thread3.c
	gcc -D _REENTRANT -I/usr/include/nptl -c thread3.c -L/usr/lib/nptl

thread4: thread4.o
	gcc -o thread4 thread4.o -lpthread

thread4.o: thread4.c
	gcc -D _REENTRANT -I/usr/include/nptl -c thread4.c -L/usr/lib/nptl

thread5: thread5.o
	gcc -o thread5 thread5.o -lpthread

thread5.o: thread5.c
	gcc -D _REENTRANT -I/usr/include/nptl -c thread5.c -L/usr/lib/nptl

thread6: thread6.o
	gcc -o thread6 thread6.o -lpthread

thread6.o: thread6.c
	gcc -D _REENTRANT -I/usr/include/nptl -c thread6.c -L/usr/lib/nptl

cond_wait: cond_wait.o
	gcc -g -o cond_wait cond_wait.o -lpthread

cond_wait.o: cond_wait.c
	gcc -D _REENTRANT -I/usr/include/nptl -c -g cond_wait.c -L/usr/lib/nptl
clean:
	rm -rf *.o 