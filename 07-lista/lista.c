#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo* proximo;
} nodo_t;

struct lista {
    nodo_t* primero;
    nodo_t* ultimo;
    size_t largo;
};

struct lista_iter {
    lista_t* lista;
    nodo_t* anterior;
    nodo_t* actual;
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
    nodo->proximo = lista->primero;
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
    } else {
        lista->ultimo->proximo = nodo;
    }    
    lista->ultimo = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    nodo_t* auxiliar = lista->primero;
    void* dato = auxiliar->dato;

    lista->primero = auxiliar->proximo;
    if (!auxiliar->proximo) {
        lista->ultimo = NULL;
    }
    lista->largo--;
    free(auxiliar);        
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->ultimo->dato;
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

/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/
lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if(!iter) {
        return NULL;
    }
    iter->lista = lista;
    iter->anterior = NULL;
    iter->actual = lista->primero;
    return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return (iter->anterior == iter->lista->ultimo && !iter->actual) ? true : false;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->proximo;
    return true;
}
void *lista_iter_ver_actual(const lista_iter_t *iter){
    return iter->actual->dato;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato);
    if (!nodo_nuevo) {
        return false;
    }
    if (iter->actual == iter->lista->primero) {        
        nodo_nuevo->proximo = iter->lista->primero;        
        iter->lista->primero = nodo_nuevo;
        if (!iter->lista->primero->proximo) {
            iter->lista->ultimo = iter->lista->primero;
        }
        
    } else if (lista_iter_al_final(iter)) {        
        iter->anterior->proximo = nodo_nuevo;
        iter->lista->ultimo = nodo_nuevo;        
    } else {
        iter->anterior->proximo = nodo_nuevo;
        nodo_nuevo->proximo = iter->actual->proximo;
    }
    iter->actual = nodo_nuevo;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return NULL;
    }
    nodo_t* auxiliar = iter->actual;
    void* dato = auxiliar->dato;

    if (iter->actual == iter->lista->primero) {        
        iter->lista->primero = iter->actual->proximo;        
    } else {
        iter->anterior->proximo = iter->actual->proximo;
        if (iter->actual == iter->lista->ultimo) {            
            iter->lista->ultimo = iter->anterior;
        }
    }
    iter->actual = iter->actual->proximo;    
    iter->lista->largo--;
    free(auxiliar);
    return dato;
}

/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
    nodo_t* elemento = lista->primero;
    size_t contador = 0;
    while (contador < lista_largo(lista)) {
        if (visitar(elemento->dato, extra)) {
            elemento = elemento->proximo;
            contador++;
        } else {
            return;
        }
        
    }
}
