#include <syslog.h>

int main(int argc, char** argv)
{
    openlog(0, LOG_CONS | LOG_PID, 0);
    syslog(LOG_DEBUG, "This is a syslog test debug log generateed by prgram %s\n"
            , argv[0]);
    closelog();
    return 0;
}
