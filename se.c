#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

void spawn_daemon()
{
}

void
run_updater()
{
    int ssocket, csock;
    char buf[512];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    ssocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = INADDR_ANY ;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(ssocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    listen(ssocket, 5);

    addr_size = sizeof serverStorage;

    while (1) {

        csock = accept(ssocket, (struct sockaddr *)&serverStorage, &addr_size);

		memset(buf,0, sizeof buf);

		read(csock, buf, 512);

        pid_t pid = -1;

        pid = fork();

        if (pid == 0) {
            execlp("/bin/echo", "echo", buf, NULL);
        } else if (pid > 0) {
            continue;
        } else {
        }
    }
#if 0
    int fd = open("/dev/kmsg", O_WRONLY | O_APPEND);

    printf("open kmsg %d\n", fd);

    write(fd, "hello", 5);
#endif

    return 0;
}

int 
main()
{
	spawn_daemon();
	
	run_updater();
	
	return 0;
}
}
