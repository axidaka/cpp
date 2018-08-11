#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void work()
{
	FILE* pFile = NULL;
	int i;
	double x= 4.5;

	pFile = tmpfile();
	for (i = 0; i< 10000; i++)
	{
		fprintf(pFile, "Do Somethings\n");
		if (ferror(pFile))
		{
			fprintf(stderr, "Erro wirting to temp file\n");
			exit(1);
		}
	}

	for (i = 0; i < 10000; i++)
	{
		x = log(x*x + 3.21);	
	}
}

int main()
{
	struct rusage r_usage;
	struct rlimit r_limit;
	int priority;

	work();

	getrusage(RUSAGE_SELF,&r_usage);
	printf("CPU usage: User = %ld.%06ld, System = %ld.%06ld\n",
		r_usage.ru_utime.tv_sec, r_usage.ru_utime.tv_usec,
		r_usage.ru_stime.tv_sec, r_usage.ru_stime.tv_usec);



	exit(0);

}
