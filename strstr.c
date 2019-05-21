#include <stdio.h>

char *_strstr(const char *s1, const char *s2)
{
    size_t l1, l2;

    l2 = strlen(s2); //raju
    if (!l2)
        return (char *)s1;
    l1 = strlen(s1); //umaraju
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}


int main (int argc, char *argv[])
{
	printf ("%s \n", _strstr(argv[1], argv[2]) );
	return 0;
}

