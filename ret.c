//test1.cc
//compile with
//g++ -g test1.cc -o test1 

#include <stdio.h>

void
print_function(void *p) {
    char cmd[128];
    FILE *fp;

    snprintf(cmd, sizeof(cmd), "addr2line -e %s -f %p", "test1", p);
    fp = popen(cmd, "r");
    if (fp) {
    char buf[128];
    while (fgets(buf, sizeof(buf), fp)) {
        printf("%s", buf); 
    }
    }
}

void
f2(void) {
    print_function(__builtin_return_address(0));
}

void
f1(void) {
    f2();
}

int
main(int argc, char *argv[]) {
    f1();
    return(0);
}
