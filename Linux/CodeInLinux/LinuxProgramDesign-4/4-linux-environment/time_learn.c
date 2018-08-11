#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	time_t the_time;
	int i;
	for (i =0 ;i <= 10; i++)
	{
		the_time = time((time_t*)0);
		printf("The time is %ld\n", the_time);
		sleep(2);

	}

	sleep(1);
	printf("-------------------\n");
	{
		struct tm* tm_ptr = NULL;
		time_t the_time;

		(void)time(&the_time);
		tm_ptr = gmtime(&the_time);

		printf("Raw time is %ld\n", the_time);
		printf("gmtime gives:\n");
		printf("date: %02d/%02d/%02d\n", 
				tm_ptr->tm_year + 1900, tm_ptr->tm_mon +1, tm_ptr->tm_mday);
		printf("time:%2d:%2d:%2d\n", 
				tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);


	}
	printf("------------------------\n");
	{
		struct tm* tm_ptr = NULL;
		time_t the_time;

		(void)time(&the_time);
		tm_ptr =localtime(&the_time);

		printf("Raw time is %ld\n", the_time);
		printf("locatime gives:\n");
		printf("date: %02d/%02d/%02d\n", 
				tm_ptr->tm_year + 1900, tm_ptr->tm_mon +1, tm_ptr->tm_mday);
		printf("time:%2d:%2d:%2d\n", 
				tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);


		printf("asctime: %s\n", asctime(tm_ptr));
		printf("ctime: %s\n", ctime(&the_time));
	}
	
	printf("------------------------\n");

	{
		struct tm* tm_ptr, timestuct;
		time_t the_time;
		char buf[256] = {0};
		char* result = NULL;

		(void)time(&the_time);
		tm_ptr = localtime(&the_time);

		strftime(buf, sizeof(buf), "%A %d %B, %I:%S:%p", tm_ptr);
		printf("strftime gives: %s\n", buf);

	}
	exit(0);
}
