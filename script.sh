(gcc -o "dabba" -xc - ) <<- __EOF__
        #include <stdlib.h>
	#include <stdio.h>

        int main(int argc, char *argv[])
        {
	printf("hello");
                return 1;

        }
__EOF__
(gcc -o dabba2 -xc -) <<- __EOF__

	#include <stdio.h>
	int main( void )
	{
		printf("hello 2");
		return 0;
	}
__EOF__
