#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int collatz(int n)
{
    if (n == 1)
        return 0;
    else if ((n % 2) == 0)
        return collatz(n / 2) + 1;
    else
        return collatz(3*n+1) + 1;
}

int main(void)
{
int number = get_int("#: ");

printf("%i\n", collatz(number));
}