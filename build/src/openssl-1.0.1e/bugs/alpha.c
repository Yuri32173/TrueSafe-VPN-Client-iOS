#include <stdio.h>

#define TYPE unsigned int

void func(TYPE *a, TYPE b)
{
    printf("%u - 1 == %u\n", a[0], b);
}

int main()
{
    TYPE data[5] = {1, 2, 3, 4, 5};
    TYPE *p = data;

    for (int i = 0; i < 4; i++)
    {
        func(p, *p++);
    }

    return 0;
}
