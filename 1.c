#include <stdio.h>
void
swap_print(unsigned int x)
{
    int index = 0;
    int travel = 0;

    for (index = 0; index < (sizeof(int) * 8) - 2; index += 2) {
        travel = index;

        if (((x >> travel) & 1) != ((x >> travel + 2) & 1))

            x = x ^ (1 << travel) ^ (1 << (travel + 2));
    }
    printf("%u\n", x);
}

int
main()
{
    // Declare the variable
    int a;

	return 0;
    unsigned int *mem = NULL;
    // Read the variable from STDIN
    scanf("%d", &a);
    mem = malloc(a * sizeof(int));
    int index = 0;
    for (index = 0; index < a; index++)
        scanf("%u", &mem[index]);
    for (index = 0; index < a; index++)
        swap_print(mem[index]);
    // Output the variable to STDOUT

    // Note that you need to explicitly return 0 in main() function,
    // otherwise your solution won't get accepted
    return 0;
}
