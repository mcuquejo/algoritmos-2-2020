#include <stdlib.h>
#include "cola.h"

typedef struct nodo {
    struct nodo* siguiente;
    void** dato;
} nodo_t;

struct cola {
    nodo_t* primero;
    nodo_t* ultimo;    
};

/* ******************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/
nodo_t* crear_nodo(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->siguiente = NULL;
    return nodo;
}


/* ******************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/
cola_t* cola_crear(void) {
    cola_t* cola = malloc(sizeof(cola_t));
    if(!cola) {
        return NULL;
    }
    cola->primero=NULL;
    cola->ultimo=NULL;
    return cola;
}


void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)) {
    while (!cola_esta_vacia(cola)) {
        void* dato = cola_desencolar(cola);
        if (destruir_dato) {
            destruir_dato(dato);
        }
    }
    free(cola);
}


bool cola_esta_vacia(const cola_t *cola) {
    return !(cola->primero && cola->ultimo);
}


bool cola_encolar(cola_t *cola, void* valor) {
    nodo_t* nodo = crear_nodo(valor);
    if (!nodo) {
        return false;
    }

    if (cola_esta_vacia(cola)) {
        cola->primero = nodo;
    } else {
        nodo_t* auxiliar = cola->ultimo;
        auxiliar->siguiente = nodo;
    }    
    cola->ultimo = nodo;
    return true;
}


void* cola_ver_primero(const cola_t *cola) {
    return cola_esta_vacia(cola) ? NULL : cola->primero->dato;
}


void* cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    nodo_t* auxiliar = cola->primero;
    void* dato = auxiliar->dato;
    cola->primero = auxiliar->siguiente;    
    if (!auxiliar->siguiente) {
        cola->ultimo = NULL;
    }
    free(auxiliar);

    return dato;    
}