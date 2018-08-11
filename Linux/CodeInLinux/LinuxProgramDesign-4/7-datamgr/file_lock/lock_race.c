#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


const char* test_file = "/tmp/test_lock";


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

	region_to_test.l_type = F_RDLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 10;
	region_to_test.l_len = 5;
	
	printf("Process %d, trying F_RDLCK, region %d t %d \n", getpid(), 
			(int)region_to_test.l_start, (int)(region_to_test.l_start + region_to_test.l_len)); 

	res = fcntl(file_desc, F_SETLK, &region_to_test);
	if (-1 == res)
	{
		fprintf(stderr, "Prcess %d - failed to lock region(%s)\n", getpid(), strerror(errno));
	}
	else
	{
		printf("Process %d -obtained lock region\n", getpid());
	}

	region_to_test.l_type = F_UNLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 10;
	region_to_test.l_len = 5;

	printf("Process %d, tryint F_UNLCK, region %d to %d \n", getpid(), (int)region_to_test.l_start, 
			(int)(region_to_test.l_start + region_to_test.l_len));

	res = fcntl(file_desc, F_SETLK, &region_to_test);
	if (-1 == res )
	{
		fprintf(stderr, "Process %d - failed to lock region(%s)\n", getpid(), strerror(errno));
	}
    else
	{
		printf("Process %d -  unlock region \n", getpid());
	}


	region_to_test.l_type = F_UNLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 0;
	region_to_test.l_len = 50;

	printf("Process %d, tryint F_UNLCK, region %d to %d \n", getpid(), (int)region_to_test.l_start, 
			(int)(region_to_test.l_start + region_to_test.l_len));

	res = fcntl(file_desc, F_SETLK, &region_to_test);
	if (-1 == res )
	{
		fprintf(stderr, "Process %d - failed to lock region(%s)\n", getpid(), strerror(errno));
	}
    else
	{
		printf("Process %d -  unlock region \n", getpid());
	}


	region_to_test.l_type = F_WRLCK;
	region_to_test.l_whence = SEEK_SET;
	region_to_test.l_start = 16;
	region_to_test.l_len = 5;

	printf("Process %d, tryint F_WRLCK, region %d to %d \n", getpid(), (int)region_to_test.l_start, 
			(int)(region_to_test.l_start + region_to_test.l_len));

	res = fcntl(file_desc, F_SETLK, &region_to_test);
	if (-1 == res )
	{
		fprintf(stderr, "Process %d - failed to lock region(%s)\n", getpid(), strerror(errno));
	}
    else
	{
		printf("Process %d - obtained lock region \n", getpid());
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
