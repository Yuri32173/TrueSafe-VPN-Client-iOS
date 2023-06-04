#include <stdio.h>

void inc(int *a)
{
    (*a)++;
}

int main()
{
    int p = 0;
#ifdef FIXBUG
    int dummy;
#endif

    while (p < 3)
    {
        fprintf(stderr, "%08X\n", p);
        inc(&p);
#ifdef FIXBUG
        dummy += p;
#endif
    }

    return 0;
}
