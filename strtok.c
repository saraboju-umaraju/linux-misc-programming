#include <stdio.h>

char *strpbrk(const char *cs, const char *ct)
{
    const char *sc1, *sc2;

    for (sc1 = cs; *sc1 != '\0'; ++sc1) {
        for (sc2 = ct; *sc2 != '\0'; ++sc2) {
            if (*sc1 == *sc2)
                return (char *)sc1;
        }
    }
    return NULL;
}
char *strsep(char **s, const char *ct)
{
    char *sbegin = *s;
    char *end;

    if (sbegin == NULL)
        return NULL;

    end = strpbrk(sbegin, ct);
    if (end)
        *end++ = '\0';
    *s = end;
    return sbegin;
}

int main (int argc, char *argv[])
{
	char *str = strdup("umarajaauarajuumaraju");
	char *found = NULL;
	while( (found = strsep(&str,argv[1])) != NULL )
        printf("%s\n",found);
	return 0;
}

