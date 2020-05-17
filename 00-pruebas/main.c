#include "hola_mundo.h"
#include "dyc_00.h"

int main(void) {
    int arreglo1[] = {-3, 0, 1, 3, 7, 9};
    int tam1 = 6;
    int arreglo2[] = {1, 2, 4, 6, 7, 9};
    int tam2 = 6;


    hola_mundo();
    probar_es_magico(arreglo1, tam1);
    probar_es_magico(arreglo2, tam2);
    return 0;
}