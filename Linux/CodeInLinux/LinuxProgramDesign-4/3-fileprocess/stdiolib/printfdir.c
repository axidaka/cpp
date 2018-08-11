#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void printdir(char* dir, int depth)
{
	DIR* pDir = NULL;
	
	if ((pDir = opendir(dir)) == NULL)
	{
		fprintf(stderr, "cannot open directory:%s(%s)\n", dir, strerror(errno));
		return;
	}

	chdir(dir);

	struct dirent* pEntry = NULL;
	struct stat    statbuf;
	while ((pEntry = readdir(pDir)) != NULL)
	{
		lstat(pEntry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode))
		{
			// found a directory, but ignore .and ..
			if (0 == strcmp(pEntry->d_name, ".") ||
				0 == strcmp(pEntry->d_name, ".."))
				{
					continue;
				}

			printf("%*s%s/\n", depth, "",pEntry->d_name);

			printdir(pEntry->d_name, depth + 4);
		}
		else printf("%*s%s\n", depth, "", pEntry->d_name);

	}

	chdir("..");
	closedir(pDir);

}

int main(int argc, char* args[])
{
	char *pTopDir = ".";
	if (argc >= 2)
		pTopDir = args[1];

	printf("Directory scan of %s\n", pTopDir);
	printdir(pTopDir, 0);
	printf("done \n");

	return 0;

}
