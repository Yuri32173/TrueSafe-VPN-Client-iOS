#include <stdio.h>

typedef unsigned int TYPE;

void func(TYPE *a, TYPE b) {
    printf("%u - 1 == %u\n", a[0], b);
}

int main() {
    TYPE data[5] = { 1, 2, 3, 4, 5 };
    TYPE *p;
    int i;

    p = data;

    for (i = 0; i < 4; i++) {
        func(p, *p++);
    }

    return 0;
}
