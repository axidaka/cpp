#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char* lock_file = "/tmp/LCK.test2";

int main()
{
	int file_desc;
	int tries = 10;

	while (tries --)
	{
		// 创建锁文件后一段时间，通过unlink删除该锁文件。 这种实现可以看做是
		// 临界区
		file_desc = open(lock_file, O_RDWR | O_CREAT | O_EXCL, 0444);
		if (file_desc == -1)
		{
			printf("%d - Lock already present\n", getpid());
			sleep(3);
		}
		else
		{
			printf("%d - I have exclusive access \n", getpid());
			sleep(1);
			close(file_desc);
			unlink(lock_file);
			sleep(2);
		}
	}

	exit(EXIT_SUCCESS);

}
