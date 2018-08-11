#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ONE_X  (1024)   // 每次申请1K空间

int main()
{
	char* some_memory = NULL;
	int size_to_allocate = ONE_X;
	int megs_obtained = 0;
	int ks_obtained = 0;

	while (1)
	{
		for (ks_obtained  = 0; ks_obtained < 1024; ks_obtained ++)
		{
			some_memory = (char*)malloc(size_to_allocate);
			if (some_memory)
			{
				sprintf(some_memory, "hello world! use malloc 1k\n");
			}
			else
			{
				exit(EXIT_FAILURE);
			}
		}

		megs_obtained ++;
		printf("Now allocated %d Megabytes\n", megs_obtained);
	}

	exit(EXIT_SUCCESS);
	
}
