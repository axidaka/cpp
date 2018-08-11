#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern char** environ;

int main(int argc, char* argv[])
{
	printf("All Environ \n");
	char **env = environ;
	while (*env)
	{
		printf("%s\n", *env);
		env ++;
	}


	char* var = NULL, *value=NULL;

	if (argc == 1 || argc > 3)
	{
		fprintf(stderr, "usage:environ var [value]\n");
		exit(-1);
	}

	var = argv[1];

	if (2 == argc)
	{
		value = getenv(var);
		if (value)
		{
			printf("Variable %s has value %s\n", var, value);
		}
		else
		{
			printf("Variable %s has no value\n", var);
		}
	}
	else if(3 == argc)
	{
		char* string = NULL;
		value = argv[2];
		string = malloc(strlen(var) + strlen(value) + 2);
		if (!string)
		{
			fprintf(stderr, "out of memory\n");
			exit(1);
		}
		
		strcpy(string, var);
		strcat(string, "=");
		strcat(string, value);
		printf("Calling putenv with: %s\n", string);
		if (putenv(string) != 0)
		{
			fprintf(stderr, "putenv failed\n");
			free(string);
			exit(1);
		}

		value = getenv(var);
		if (value)
		{
			printf("New value of %s is %s \n", var, value);
		}
		else 
		{
			printf("New value of %s is null\n", var);
		}

		free(string);
	}


	return 0;
}
