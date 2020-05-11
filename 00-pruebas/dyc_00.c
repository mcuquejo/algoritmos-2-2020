#include <stdbool.h>
#include <stdio.h>
// Implementar un algoritmo en C que reciba un arreglo de enteros de tamaño n, 
// ordenado ascendentemente y sin elementos repetidos, y determine en O(log⁡ n)
// si es mágico. 
//Un arreglo es mágico si existe algún valor i tal que 0 ≤ i < n y arr[i] = i. 

//Justificar el orden del algoritmo.

// Ejemplos:

//     A = [ -3, 0, 1, 3, 7, 9 ] es mágico porque A[3] = 3.

//     B = [ 1, 2, 4, 6, 7, 9 ] no es mágico porque B[i] != i para todo i.

bool es_magico(int arreglo[], int tam, int inicio, int fin) {
    if (inicio > fin) return false;

    int medio = (inicio + fin) / 2;

    if (arreglo[medio] == medio) {
        return true;
    }
    bool mitad_iz = es_magico(arreglo, medio, inicio, medio);
    bool mitad_der = es_magico(arreglo, fin - medio, medio, fin);
    return false;
}

int main(void) {
    int arreglo1[] = {-3, 0, 1, 3, 7, 9};
    int tam1 = 6;
    int arreglo2[] = {1, 2, 4, 6, 7, 9};
    int tam2 = 6;
    if (es_magico(arreglo1, tam1, 0, tam1)) {
        printf("Arreglo 1 Es magico\n");
    } else {
        printf ("Arreglo 1 No es magico\n");
    }

    if (es_magico(arreglo2, tam2, 0, tam2)) {
        printf("Arreglo 2 Es magico\n");
    } else {
        printf ("Arreglo 2 No es magico\n");
    }
    return 0;
}