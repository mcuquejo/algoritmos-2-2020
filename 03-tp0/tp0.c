#include "tp0.h"
#include <stdlib.h>
#include <stdio.h>

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap (int *x, int *y) {    
    int aux = *x;    
    *x = *y;
    *y = aux;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
    int pos = 0;
    while (n1 > pos && n2 > pos) {
        if (vector1[pos] > vector2[pos]) {
            return 1;
        } else if (vector1[pos] < vector2[pos]) {
            return -1;
        } else {
            pos++;
        }
    }
    if(n1 > pos) {
        return 1;
    }
    if (n2 > pos) {
        return -1;
    }
    return 0;
}

int maximo(int vector[], int n) {
    if (n == 0) {
        return -1;
    }
    
    int pos = 0;
    int maximo = vector[pos];    
    
    for(int i = 0; i < n; i++) {
        if (maximo < vector[i]) {
            maximo = vector[i];
            pos = i;
        }
    }
    return pos;
}

void seleccion(int vector[], int n) {    
    if (n == 0) return;
    for (int i = n - 1; i > 0; i--) {
        int p = maximo(vector, i);
        if (vector[i] < vector[p]) swap(&vector[p], &vector[i]);
    }
}
