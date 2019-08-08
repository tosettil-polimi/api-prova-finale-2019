#include "hashtable.c"
#include "utente.c"
#include <stdlib.h>
#include <stdio.h>

// Primo test di struttura dati

void main() {

    int *v, i;

    v = (int*) malloc(sizeof(int) * 3);

    for(i = 0; i < 3; i++) {
        v[i] = i;
        printf("%d\n", i);
    }

    printf("size: %ls\n", v);

    v = (int*) realloc(v, sizeof(int) * 4);

    v[3] = 3;

    printf("size: %ls\n", v);

    for(i = 0; i < 4; i++) {
        printf("%d\n", v[i]);
    }

    free(v);
}