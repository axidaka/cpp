#include <unistd.h>
#include <stdlib.h>

int main()
{
	// write在标准输出
	char szOutput[] = {"Call write API:Here is some data output in standaroutput\n"};
	if ( write(1, szOutput, sizeof(szOutput) ) != sizeof(szOutput))
		{
			char szErr[] = {"A write error has opened on file descriptor 1\n"};
			write(2, szErr, sizeof(szErr));
		}
	exit(0);
}
