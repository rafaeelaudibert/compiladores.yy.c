#include <stdio.h>

int j = 16;
int foo(int n)
{
    j += 1;
    return n == 0 ? 1 : n * foo(n - 1);
}

int a[] = {1, 2, 3, 4};
int main()
{
    j = 1;
    a[2] = 3;
    printf("%d\n", 12);
    printf("%d\n", j);

    return 0;
}
