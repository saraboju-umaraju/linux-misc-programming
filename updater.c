#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/in.h>


void skeleton_daemon()
{
    pid_t pid;
	
    /* Fork off the parent process */
    pid = fork();
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>0; x--)
    {
        close (x);
    }

}

int run()
{
	int ssocket, csock;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    ssocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = INADDR_ANY ;
    memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);

    bind(ssocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (listen(ssocket, 5) == 0) ;

    addr_size = sizeof serverStorage;

	while(1) {
		csock =
			accept(ssocket, (struct sockaddr *)&serverStorage, &addr_size);

		pid_t new = fork();
		
		if (0 == new) {
			execl("/bin/updater.sh", "updater.sh", NULL);
		} else if (new > 0) {
			continue ;
		} else {
			/* fork() failed us */
		}

	}
}
int main()
{
    skeleton_daemon();

	run();

    return EXIT_SUCCESS;
}

