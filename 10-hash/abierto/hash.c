#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "hash.h"
#define TAM_INICIAL 31

struct hash {
    lista_t** tabla;
    size_t cant;
    size_t tam;
    hash_destruir_dato_t destruir;
};

typedef struct campo_hash {
    char* clave;
    void* valor;
} campo_hash_t;

struct hash_iter{
  const hash_t* hash;
  campo_hash_t campo_act;
  size_t posicion;
};

size_t fnv_hash(const char *clave, size_t largo){
    size_t h = 14695981039346656037U;
    size_t n = strlen(clave);
    for (size_t i = 0; i < n; i++){
        h *= 1099511628211;
        h ^= (size_t)clave[i];
    }
    return h % largo;
}

void inicializar_hash_tabla (lista_t** hash_tabla, size_t tam) {
    for (size_t i = 0; i < tam; i++) {
        hash_tabla[i] = lista_crear();
    }
}

/* Crea el hash
 */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) {
        return NULL;
    }
    
    hash->cant = 0;
    hash->tam = TAM_INICIAL;
    hash->destruir = destruir_dato;
    inicializar_hash_tabla(hash->tabla, hash->tam);

    return hash;
}

campo_hash_t* hash_buscar(hash_t* hash, size_t posicion, const char* clave) {    
    if (lista_esta_vacia(hash->tabla[posicion])) {
        return NULL;
    }

    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    while (!lista_iter_al_final(iter_lista_hash)) {
        campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
        if (strcmp(clave, campo_actual->clave) == 0) {
            return campo_actual;
        }
    }
    return NULL;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    size_t posicion = fnv_hash(clave, hash->tam);
    campo_hash_t* campo_a_guardar = hash_buscar(hash, posicion, clave);
    campo_hash_t* campo_nuevo = malloc(sizeof(campo_hash_t));
    if (!campo_nuevo) {
        return false;
    }
    if (campo_a_guardar) {        
        campo_nuevo->clave = strdup(clave);
        campo_nuevo->valor = dato;
        lista_insertar_primero(hash->tabla[posicion], campo_nuevo);
    } else {
        campo_hash_t* campo_anterior = campo_a_guardar;
        hash->destruir(campo_anterior);
        campo_a_guardar->valor = dato;        
    }
    return true;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave) {
    size_t posicion = fnv_hash(clave, hash->tam);
    campo_hash_t* campo_a_borrar = hash_buscar(hash, posicion, clave);
    if (!campo_a_guardar) {
        return NULL;
    } else {

    }
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave);

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave);

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash);

/* Iterador del hash */

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash);

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter);

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter);

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter);