#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	int data_processed;
	int file_pipes[2];
	const char some_date[] = "123";

	char buffer[BUFSIZ + 1];
	if (pipe(file_pipes) == 0)
	{
		data_processed = write(file_pipes[1], some_date, strlen(some_date));
		printf("Wrote %d bytes\n", data_processed);

		data_processed = read(file_pipes[0], buffer, BUFSIZ);
		printf("Read %d bytes: %s\n", data_processed, buffer);
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_FAILURE);
}