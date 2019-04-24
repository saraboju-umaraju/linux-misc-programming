/****************** SERVER CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

int
main()
{
    int ssocket, csock;
    char buffer[1024];
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    ssocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7777);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(ssocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (listen(ssocket, 5) == 0)
        printf("Listening\n");
    else
        printf("Error\n");

    addr_size = sizeof serverStorage;
    csock =
        accept(ssocket, (struct sockaddr *)&serverStorage, &addr_size);

		printf("hello\n");
	    int fd = open("/dev/kmsg", O_WRONLY|O_APPEND);
    printf("open kmsg %d\n",fd);

    write(fd,"hello",5);


    return 0;
}
