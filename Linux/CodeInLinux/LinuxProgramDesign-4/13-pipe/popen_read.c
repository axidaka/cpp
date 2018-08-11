#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	FILE* read_fp = NULL;
	char buffer[BUFSIZ + 1];
	int chars_read = 0;
	memset(buffer, '\0', sizeof(buffer));
	read_fp = popen("uname -a", "r");
	if (read_fp != NULL)
	{
		chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
		if (chars_read > 0)
		{
			printf("Ouput was: -\n %s\n", buffer);
		}
		pclose(read_fp);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}