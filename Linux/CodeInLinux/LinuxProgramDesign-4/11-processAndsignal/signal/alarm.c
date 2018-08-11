#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int b_alarm_fired = 0;

void ding(int sig)
{
	b_alarm_fired = 1;
}

int main()
{
	pid_t pid;

	printf("alarm application starting\n");

	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed\n");
			exit(1);
		case 0:
			// child
			sleep(5);
			kill(getppid(), SIGALRM);
			exit(0);
	}

	printf("waiting for alarm to go off\n");
	(void)signal(SIGALRM, ding);

	// 将程序挂起直到一个信号出现为止
	pause();

	if (b_alarm_fired)
		printf("Ding \n");

	printf("done");

	exit(0);
}
