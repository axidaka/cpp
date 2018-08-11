#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int res = mkfifo("/tmp/my_fifo", 0777);
	if (0 == res)
		printf("FIFO successed\n");
	exit(EXIT_SUCCESS);
}