#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


const char* test_file = "/tmp/test_lock";
#define SIZE_TO_TRY 5

void show_lock_info(struct flock* to_show);

int main()
{
	int file_desc;
	int res;
	struct flock region_to_test;
	int start_byte;

	file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
	if (-1 == file_desc)
	{
		fprintf(stderr, "Failed to open %s to read.write(%s)\n", test_file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (start_byte = 0; start_byte < 99; start_byte += SIZE_TO_TRY)
	{
		region_to_test.l_type = F_WRLCK;
		region_to_test.l_whence = SEEK_SET;
		region_to_test.l_start = start_byte;
		region_to_test.l_len = SIZE_TO_TRY;
		region_to_test.l_pid = -1;

		printf("Testing F_WRLCK on region from %d to %d \n", start_byte, start_byte + SIZE_TO_TRY);

		res = fcntl(file_desc, F_GETLK, &region_to_test);
		if (-1 == res)
		{
			fprintf(stderr, "F_GETLK failed(%s)\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (region_to_test.l_pid != - 1)
		{
			// 通过比较 l_pid 可以得知测试区域是否已经被锁定， 结构返回是该锁信息
			printf("Lock would fail F_GETLK return:\n");
			show_lock_info(&region_to_test);
		}
		else
		{
			printf("F_WRLCK - Lock would succeed\n");
		}

		// 使用共享重复测试一次， 还是该片区域
		region_to_test.l_type = F_RDLCK;
		region_to_test.l_whence = SEEK_SET;
		region_to_test.l_start = start_byte;
		region_to_test.l_len = SIZE_TO_TRY;
		region_to_test.l_pid = -1;

		printf("Testing F_RDLCK on region from %d to %d \n", start_byte, start_byte + SIZE_TO_TRY);

		res = fcntl(file_desc, F_GETLK, &region_to_test);
		if (-1 == res )
		{
			fprintf(stderr, "F_GETLK failed(%s)\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (region_to_test.l_pid != -1)
		{
			printf("Lcok would fail F_GETLK return:\n");
			show_lock_info(&region_to_test);
		}
		else
		{
			printf("F_RDLCK - Lock would succeed\n");
		}

	}

	close(file_desc);
	exit(EXIT_SUCCESS);
}


void show_lock_info(struct flock *to_show)
{
	printf("\t l_type(1:wrlck 0:rdlck) %d, ", to_show->l_type);
	printf("\t l_whence %d, ", to_show->l_whence);
	printf("\t l_start %d, ", to_show->l_start);
	printf("\t l_len %d, ", to_show->l_len);
	printf("\t l_pid %d\n", to_show->l_pid);
}
