#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char* test_file = "/tmp/test_lock";

int main()
{
	int file_desc;
	int byte_count;
	char* byte_to_write = "A";
	struct flock region_1;
	struct flock region_2;
	int res;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if ( file_desc == 1)
	{
		fprintf(stderr, "Unable to open %s for read/write.(%s)\n", test_file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (byte_count = 0 ; byte_count < 100; byte_count ++)	
	{
		write(file_desc, byte_to_write, 1);
	}

	// 10~30 字节设置为共享锁
	region_1.l_type = F_RDLCK;
	region_1.l_whence = SEEK_SET;
	region_1.l_start = 10;
	region_1.l_len = 20;

	// 40~50字节设置为独占锁
	region_2.l_type = F_WRLCK;
	region_2.l_whence = SEEK_SET;
	region_2.l_start = 40;
	region_2.l_len = 10;

	// 使用命令 F_SETLK 锁定文件
	printf("Process %d locking file\n", getpid());
	res = fcntl(file_desc, F_SETLK, &region_1);
	if (-1 == res)
		fprintf(stderr, "Failed to lock region 10~30 (%s)\n", strerror(errno));

	res = fcntl(file_desc, F_SETLK, &region_2);
	if (-1 == res)
		fprintf(stderr, "Failed to lock region 40~50 (%s)\n", strerror(errno));

	sleep(60);

	printf("process %d closing file \n", getpid());
	close(file_desc);


	exit(EXIT_SUCCESS);
}
