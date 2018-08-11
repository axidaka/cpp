#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <errno.h>

int main()
{
	int c;
	FILE* pFin = NULL , *pFout = NULL;
	
	pFin = fopen("file.in", "r");
	if (!pFin)
	{
		printf("fopen file.in failed: %s\n", strerror(errno));
		return -1;
	}

	pFout = fopen("file.out", "w");
	if (!pFout)
	{
		printf("fopen file.out falied: %s \n", strerror(errno));
		return -1;
	}

	while ( (c = fgetc(pFin)) != EOF)
	{
		fputc(c, pFout);
	}
	
	printf("copy end\n");
	return 0;
}
