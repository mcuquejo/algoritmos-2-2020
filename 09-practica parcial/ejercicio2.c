//102624	me tocan los ejercicios 2	5	8
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "ejercicio2.h"

typedef enum {VACIO, OCUPADO, BORRADO} estado_t;

typedef struct multiconj {
    hash_t* hash;    
} multiconj_t;

multiconj_t* multiconj_crear() {
    multiconj_t* multiconjunto = malloc(sizeof(multiconj_t));
    if (!multiconjunto) {
        return NULL;
    }
    multiconjunto->hash = hash_crear(NULL);
    if (!multiconjunto->hash) {
        free(multiconjunto);
        return NULL;
    }
    return multiconjunto;
}

bool multiconj_guardar(multiconj_t* multiconj, char* elem) {
    size_t cantidad = 0;
    if (hash_pertenece(multiconj->hash, elem)) {
        cantidad = hash_obtener(multiconj->hash, elem);
    }
    cantidad++;
    return hash_guardar(multiconj->hash, elem, cantidad);
}

bool multiconj_pertenece(multiconj_t* multiconj, char* elem) {
    return hash_pertenece(multiconj->hash, elem);
}

bool multiconj_borrar(multiconj_t* multiconj, char* elem) {
    if (!hash_pertenece(multiconj->hash, elem)) {
        return false;
    }

    size_t cantidad = hash_obtener(multiconj->hash, elem);
    cantidad--;
    if (cantidad == 0) {
        hash_borrar(multiconj->hash, elem);
    }
    return hash_guardar(multiconj->hash, elem, cantidad);
}