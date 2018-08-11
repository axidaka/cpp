#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define A_MEGABYTE   (1024*1024)   // 1M 空间
#define PHY_MEM_MEGS (1024*3)      // 2G 空间

int main()
{
	char* some_memory = NULL;
	size_t size_to_allocate = A_MEGABYTE; // 每次只申请1M的空间
	int mega_obtained = 0;                // 累计申请的空间

	while (mega_obtained < PHY_MEM_MEGS)
	{
		some_memory = (char*)malloc(size_to_allocate);
		if (some_memory)
		{
			mega_obtained ++;
			sprintf(some_memory, "Hello world! use malloc 1M \n");
			printf("%s - now allocated %d Megabytes \n", some_memory, mega_obtained);
		}
		else
		{
			fprintf(stderr, "malloc failed %s", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	exit(EXIT_SUCCESS);
}
