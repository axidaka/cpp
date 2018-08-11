#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MY_BUFSIZ  255
int main()
{
	FILE* read_fp = NULL;
	char buffer[MY_BUFSIZ + 1];
	int chars_read = 0;
	memset(buffer, '\0', sizeof(buffer));
	read_fp = popen("ps ax", "r");
	if (read_fp != NULL)
	{
		chars_read = fread(buffer, sizeof(char), MY_BUFSIZ, read_fp);
		while (chars_read > 0)
		{
			buffer[chars_read + 1] = '\0';

			printf("Reading %d : -\n %s\n", chars_read, buffer);
			chars_read = fread(buffer, sizeof(char), MY_BUFSIZ, read_fp);
		}
		pclose(read_fp);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}