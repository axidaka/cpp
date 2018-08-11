#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	openlog("logmask", LOG_PID | LOG_CONS, LOG_USER);

	FILE* pfile = NULL;
	pfile = fopen("nothere", "r");
	if (!pfile)
		syslog(LOG_ERR | LOG_USER, "oops - %m\n");

	syslog(LOG_INFO, "information message, pid = %d", getpid());
	syslog(LOG_DEBUG, "debug message, shold appear");
	exit(0);
}
