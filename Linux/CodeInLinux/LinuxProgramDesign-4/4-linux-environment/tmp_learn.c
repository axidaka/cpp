#include <stdio.h>
#include <stdlib.h>

int main()
{
	char tmpname[L_tmpnam];
	char* filename = NULL;
	FILE* tmpfp;

	filename = tmpnam(NULL);
	printf("tmpnam: %s\n", filename);
	printf("tmpnam: %s\n", tmpnam(NULL));
	printf("filename:%s\n", filename);

	filename = tmpnam(tmpname);
	printf("param tmpname: %s- %s\n", tmpname, filename);

	tmpfp = tmpfile();
	if (tmpfp)
	{
		printf("opened a temp file OK \n");
	}
	else
	{
		perror("tmpfile");
	}

	exit(0);

}
