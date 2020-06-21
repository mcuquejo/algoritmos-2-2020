#include "lista.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo* proximo;
} nodo_t;

typedef struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
} lista_t;

/* ******************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/
nodo_t* crear_nodo(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->proximo = NULL;
    return nodo;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
lista_t *lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));
    if(!lista) {
        return NULL;
    }
    lista->primero=NULL;
    lista->ultimo=NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return !(lista->primero && lista->ultimo);
}
bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (!nodo) {
        return false;
    }
    
    if (lista_esta_vacia(lista)) {
        lista->ultimo = nodo;
    }

    lista->primero = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t* nodo = crear_nodo(dato);
    if (!nodo) {
        return false;
    }
    
    if (lista_esta_vacia(lista)) {
        lista->primero = nodo;
    }

    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    nodo_t* auxiliar = lista->primero;
    void* dato = auxiliar->dato;

    lista->primero = lista->primero->proximo;
    if (!lista->primero->proximo) {
        lista->ultimo = lista->primero;
    }
    lista->largo--;
    free(auxiliar);        
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->primero;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->ultimo;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato) {
            destruir_dato(dato);
        }
    }
    free(lista);
}
