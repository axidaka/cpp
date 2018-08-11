#include <unistd.h>

char *ps_argv[] = {"ps", "ax", 0};
char *ps_envp[] = {"PATH=/bin;/usr/bin", "TERM=console", 0};

int main()
{
	execl("/bin/ps", "ps", "a", NULL);
	execlp("ps", "ps", "a", NULL);
	execle("/bin/ps", "ps", "ax", NULL, ps_envp);

	execv("/bin/ps", ps_argv);
	execvp("ps", ps_argv);
	execve("/bin/ps", ps_argv, ps_envp);
	
	return 0;
}
