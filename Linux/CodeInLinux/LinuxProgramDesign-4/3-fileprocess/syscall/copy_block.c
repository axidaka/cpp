#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char szErr[256] = {0};
	char block[1024] = {0};
	int in = -1, out = -1;
	int nread;

	in = open("file.in", O_RDONLY);
	if ( -1 == in )
	{
		strcpy(szErr, "Open file.in failed\n");
		write(2, szErr, sizeof(szErr));
		return -1;
	}

	out = open("file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (-1 == out )
	{

		strcpy(szErr, "create file.out failed\n");
		write(2, szErr, sizeof(szErr));
		return -1;
	}

	while ( (nread = read(in, block, sizeof(block))) > 0)
	{
		write(out, block, nread);
	}

	return 0;
}
