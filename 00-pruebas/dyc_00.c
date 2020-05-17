#include <stdbool.h>
#include <stdio.h>
#include "dyc_00.h"
// Implementar un algoritmo en C que reciba un arreglo de enteros de tamaño n, 
// ordenado ascendentemente y sin elementos repetidos, y determine en O(log⁡ n)
// si es mágico. 
//Un arreglo es mágico si existe algún valor i tal que 0 ≤ i < n y arr[i] = i. 

//Justificar el orden del algoritmo.

// Ejemplos:

//     A = [ -3, 0, 1, 3, 7, 9 ] es mágico porque A[3] = 3.

//     B = [ 1, 2, 4, 6, 7, 9 ] no es mágico porque B[i] != i para todo i.

bool es_magico(int arreglo[], int inicio, int fin) {    
    if (inicio > fin) return false;    

    int medio = (inicio + fin) / 2;

    if (arreglo[medio] == medio) {
        return true;
    }    
    if (arreglo[medio] > medio) {
        return es_magico(arreglo, inicio, medio - 1);
    } else {
        return es_magico(arreglo, medio + 1, fin);
    }
}

void probar_es_magico(int* arreglo, int tam) {
    if (es_magico(arreglo, 0, tam)) {
        printf("Arreglo Es magico\n");
    } else {
        printf ("Arreglo No es magico\n");
    }
}
