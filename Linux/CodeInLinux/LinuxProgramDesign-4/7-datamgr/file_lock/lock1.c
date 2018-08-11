#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
	int file_desc;
	int save_errno;

	// O_CREAT 文件不存在就创建， O_EXCL文件存在返回-1
	// 第一次执行的程序能创建成功，后续的程序如何在不删除文件的情况下时执行失败的。
	// 这样的文件可以用作 “锁指示器”
	file_desc = open("/tmp/LCK.test", O_RDWR | O_CREAT | O_EXCL, 0444);
	if (file_desc == -1)
	{
		save_errno = errno;
		printf("Open failed with error %d(%s)\n", save_errno, strerror(save_errno));
	}
	else
	{
		printf("Open succeeded\n");
	}
	exit(EXIT_SUCCESS);
}


