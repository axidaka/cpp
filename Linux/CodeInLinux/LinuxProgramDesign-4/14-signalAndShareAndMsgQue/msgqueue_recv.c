#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>

struct my_msg_st
{
	long int my_msg_type;
	char some_text[BUFSIZ];
};

int main()
{
	int running = 1;
	int msgid;
	struct my_msg_st some_data;
	long int msg_to_receive =0;
	
	// 建立消息队列
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if (-1  == msgid)
	{
		fprintf(stderr, "msgqueue_recv msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	
	// 参数msg_to_receive为0, 就获取队列中的第一个可用消息
	while (running)
	{
		/*
		ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
		
	   The argument msgsz specifies the maximum size in bytes for the member
       mtext of the structure pointed to by the msgp argument.  If the
       message text has length greater than msgsz, then the behavior depends
       on whether MSG_NOERROR is specified in msgflg.  If MSG_NOERROR is
       specified, then the message text will be truncated (and the truncated
       part will be lost); if MSG_NOERROR is not specified, then the message
       isn't removed from the queue and the system call fails returning -1
       with errno set to E2BIG.
	   
	   msgflg：这个参数依然是是控制函数行为的标志，取值可以是：
	   0,表示忽略；
	   IPC_NOWAIT，如果消息队列为空，则返回一个ENOMSG，并将控制权交回调用函数的进程。如果不指定这个参数，
	   那么进程将被阻塞直到函数可以从队列中得到符合条件的消息为止。如果一个client 正在等待消息的时候队列被删除，
	   EIDRM 就会被返回。如果进程在阻塞等待过程中收到了系统的中断信号，EINTR 就会被返回。MSG_NOERROR，
	   如果函数取得的消息长度大于msgsz，将只返回msgsz 长度的信息，剩下的部分被丢弃了。如果不指定这个参数，E2BIG 将被返回，
	   而消息则留在队列中不被取出。当消息从队列内取出后，相应的消息就从队列中删除了。
		*/
		if (msgrcv(msgid, (void*)&some_data, BUFSIZ, msg_to_receive, 0) == -1)
		{
			fprintf(stderr, "msgqueue_recv msgrcv failed with errnor: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		
		printf("msgqueue_recv Recv: %s(type: %d)", some_data.some_text, some_data.my_msg_type);
		if (strncmp(some_data.some_text, "end", 3) == 0)
		{
			running = 0;
		}
	}
	
	// 删除队列
	if (msgctl(msgid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "msgqueue_recv msgctl IPC_RMID failed\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}
