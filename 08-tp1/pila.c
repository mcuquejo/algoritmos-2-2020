#include "pila.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAM 20
#define CAP_MIN_REDIM 0.25
#define  CAP_MAX_REDIM 0.8
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    FUNCIONES AUXILIARES DE LA PILA
 * *****************************************************************/

bool redimensionar(pila_t* pila, size_t tam_nuevo) {
    void** datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));

    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }   

    pila->datos = datos_nuevo;
    pila->capacidad = tam_nuevo;
    
    return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }
    pila->cantidad = 0;
    pila->capacidad = TAM;    
    pila->datos = malloc (pila->capacidad * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }

    return pila;

}


void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}


bool pila_esta_vacia(const pila_t *pila) {
    return (pila->cantidad == 0);
}


bool pila_apilar(pila_t *pila, void* valor) {        
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    
    if (pila->cantidad >= (double)pila->capacidad * CAP_MAX_REDIM) {
        if (!redimensionar(pila, pila->capacidad * 2)) {
            return false;
        };
    }
    return true;
}


void* pila_ver_tope(const pila_t *pila){    
    return (pila_esta_vacia(pila)) ? NULL : pila->datos[pila->cantidad - 1];
}


void* pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }    

    void* dato = pila->datos[pila->cantidad - 1];    
    pila->cantidad--;
    
    if (pila->cantidad <= (double)pila->capacidad * CAP_MIN_REDIM && pila->capacidad / 2 >= TAM) {
        if (!redimensionar(pila, pila->capacidad / 2)) {
            return false;
        };
    }
    return dato;
}

size_t pila_largo(const pila_t *pila) {
    return pila->cantidad;
}