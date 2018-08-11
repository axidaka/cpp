#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

char* menu[] = {
	"a - add new record",
	"d - delete record",
	"q - quit",
	NULL,
};

int getchoice(char* greet, char* choices[], FILE* in, FILE* out)
{
	int chosen = 0;
	int selected;
	char** option;

	do
	{
		fprintf(out,"Choice:%s\n", greet);
		option = choices;
		while (*option)
		{
			fprintf(out, "%s\n", *option);
			option ++;
		}

		do 
		{
			selected = fgetc(in);
		}while (selected == '\n');

		option = choices;

		while (*option)
		{
			if (selected == *option[0])
			{
				chosen = 1;
				break;
			}
			option++;
		}

		if (!chosen)
		{
			fprintf(out,"%c-%d-Incorrect choice, select again\n", selected, selected);
		}

	}while(!chosen);

	return selected;
}

int main()
{
	if (!isatty(fileno(stdout)))
	{
		fprintf(stderr, "You are not a terminal!\n");
	}
	
	// 当程序的输出被重定向到文件时， 不希望与用户交互的部分被重定向
	// 通过设置/dev/tty 得到当前终端或者当前的登陆会话
	FILE* input = fopen("/dev/tty", "r");
	FILE* output = fopen("/dev/tty", "w");
	if (!input || !output)
	{
		fprintf(stderr, "Unable to open /dev/tty(%s)\n", strerror(errno));
		exit(1);
	}

	int choice = 0;

	do
	{
		choice = getchoice("Please select an action", menu, input, output);
		printf("You have chosen: %c\n", choice);
	}while(choice != 'q');

	exit(0);
}
