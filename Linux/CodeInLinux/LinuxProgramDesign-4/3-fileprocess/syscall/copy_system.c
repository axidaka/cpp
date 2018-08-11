#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	// 使用系统调用，底层完成文件拷贝
	char c;
	int in = 0, out = 0;
	char szErr [255] = {0};

	in = open("file.in", O_RDONLY);
	if (-1 == in)
	{
		strcpy(szErr, "open file.in failed\n");
		write(2, szErr, sizeof(szErr));
		return -1;
	}

	out = open("file.out", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (-1 == out)
	{
		strcpy(szErr, "create file.out failed\n");
		write(2, szErr, sizeof(szErr));
		return -1;
	}

	while (read(in, &c, 1) == 1)
	{
		write(out, &c, 1);
	}		

	return 0;
}
