#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	char buffer[128] = {0};
	char szErr[256] = {0};
	int nRead;

	nRead = read(0, buffer, 128);
	if (nRead == -1)
	{
		strcpy(szErr, "A Read Error has happed \n");
		write(2, szErr, sizeof(szErr));
	}

	if ((write(1, buffer, nRead)) != nRead)
	{
		strcpy(szErr, "A write Error has happened \n");
		write(2, szErr, sizeof(szErr));
	}

	exit(0);

}
