#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"
#include "hash.h"
#define TAM_INICIAL 5
#define CARGA_MAXIMA_HASH 3
#define CARGA_MINIMA_HASH 3
#define TAM_REDIMENSION 2
#define NO_DATA_FOUND -1

/* ******************************************************************
 *                           STRUCTS
 * *****************************************************************/
struct hash {
    lista_t** tabla;
    size_t cant;
    size_t tam;
    hash_destruir_dato_t destruir_dato;
};

typedef struct campo_hash {
    char* clave;
    void* valor;
} campo_hash_t;

struct hash_iter{
  const hash_t* hash;
  size_t posicion;
  lista_iter_t* iter_lista;

};

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/
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
    size_t count = 0;
    for (size_t i = 0; i < tam; i++) {
        hash_tabla[i] = lista_crear();
        count++;
    }
}

//Funcion auxiliar que recibe la posicion e itera hasta encontrar el campo, lo elimina de la y devuelve el campo_hash_t o NULL en caso de fallar.
//PRE: El campo existe en la lista pasada por parametro
//POST: Eliminó el elemento de la lista del hash y retorno la posicion de memoria del campo_hash_t, o NULL si falló.
campo_hash_t* hash_eliminar_campo(const hash_t* hash, size_t posicion, const char* clave) {
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
    while (!lista_iter_al_final(iter_lista_hash) && strcmp(clave, campo_actual->clave) != 0) {
        lista_iter_avanzar(iter_lista_hash);
        campo_actual = lista_iter_ver_actual(iter_lista_hash);
    }
    void* resultado = lista_iter_borrar(iter_lista_hash);
    if (!resultado) {
        return NULL;
    }
    lista_iter_destruir(iter_lista_hash);
    return campo_actual;
}

//Funcion auxiliar que recibe la posicion e itera hasta encontrar el campo, y devuelve puntero a dicho campo_hash_t o NULL en caso de fallar.
//PRE: El campo existe en la lista pasada por parametro
//POST: Retornó la posicion de memoria del campo_hash_t, o NULL si falló.
campo_hash_t* hash_obtener_campo(const hash_t* hash, size_t posicion, const char* clave) {
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
    while (!lista_iter_al_final(iter_lista_hash) && strcmp(clave, campo_actual->clave) != 0) {
        lista_iter_avanzar(iter_lista_hash);
        campo_actual = lista_iter_ver_actual(iter_lista_hash);
    }
    lista_iter_destruir(iter_lista_hash);
    return campo_actual;
}

//Funcion auxiliar que recibe la clave e itera hasta encontrar el campo, y devuelve la posicion del hash donde se encuentra dicho campo_hash_t o -1
//en caso de fallar.
//PRE: El hash está creado.
//POST: Retornó la posicion del campo buscado o -1 si falló.
int hash_obtener_posicion(const hash_t* hash, const char* clave) {
    size_t posicion = fnv_hash(clave, hash->tam);
    if (lista_esta_vacia(hash->tabla[posicion])) {
        return NO_DATA_FOUND;
    }
    int pos = (int)posicion;
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    while (!lista_iter_al_final(iter_lista_hash)) {
        campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
        if (strcmp(clave, campo_actual->clave) == 0) {
            lista_iter_destruir(iter_lista_hash);
            return pos;
        }
        lista_iter_avanzar(iter_lista_hash);
    }
    lista_iter_destruir(iter_lista_hash);
    return NO_DATA_FOUND;
}

//funcion auxiliar que recibe el campo_hash_t a destruir y funcion de destruccion de dato, si correspondiera. Destruye el dato si se pasó la misma
// y libera la clave y el campo_hash_t.
//PRE: El campo_hash_t existe y la funcion destruir_dato es una funcion válida.
//POST: Se eliminó del heap  el campo_hash_t, su clave y el dato que contenía.
void destruir_campo(campo_hash_t* campo, hash_destruir_dato_t destruir_dato) {
    if (destruir_dato) {
        destruir_dato(campo->valor);
    }
    free(campo->clave);
    free(campo);
}

//Funcion auxiliar que recibe una clave y un dato y retorna un puntero a un campo_hash_t con una copia de la clave y un puntero al dato, o NULL si falla.
//PRE:
//POST: Se devolvió un puntero a un campo_hash_t o NULL en caso de fallo.
campo_hash_t* crear_campo(const char* clave, void* dato) {
    campo_hash_t* campo_nuevo = malloc(sizeof(campo_hash_t));
    if (!campo_nuevo) {
        return NULL;
    }
    campo_nuevo->clave = strdup(clave);
    if (!campo_nuevo->clave) {
        free(campo_nuevo);
        return NULL;
    }
    campo_nuevo->valor = dato;
    return campo_nuevo;
}

//Funcion auxiliar que recibe un hash y se encarga de redimensionar para aumentar o disminuir su capacidad, segun corresponda. Si falla, vuelve los cambios atras.
//PRE: Se recibe un hash valido y con datos.
//POST: Retorna true si pudo aumentar o disminuir la capacidad del hash, segun corresponda. Vuelve atras los cambios y retorna false, en caso de error.
bool hash_redimensionar(hash_t* hash){
    size_t tam = 0;
    if (hash->cant > hash->tam * CARGA_MAXIMA_HASH) {
        tam = hash->tam * TAM_REDIMENSION;
    }
    if (hash->cant < hash->tam / CARGA_MINIMA_HASH && hash->tam / TAM_REDIMENSION >= TAM_INICIAL) {
        tam = hash->tam / TAM_REDIMENSION;
    }
    lista_t** tabla = malloc(sizeof(lista_t*) * tam);
    if (!tabla) {
        return false;
    }
    lista_t** tabla_actual = hash->tabla;
    size_t tam_actual = hash->tam;
    size_t cant_actual = hash->cant;
    hash->tam = tam;
    inicializar_hash_tabla(tabla, hash->tam);
    hash->tabla = tabla;
    hash->cant = 0;
    for (size_t i=0; i < tam_actual; i++){
        lista_iter_t* iter_lista = lista_iter_crear(tabla_actual[i]);
        if (!iter_lista) {
            free(hash->tabla);
            hash->tabla = tabla_actual;
            hash->tam = tam_actual;
            hash->cant = cant_actual;
            return false;
        }
        while (!lista_iter_al_final(iter_lista)) {
            campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista);
            bool resultado = hash_guardar(hash, campo_actual->clave, campo_actual->valor);
            if (!resultado) {
                free(hash->tabla);
                hash->tabla = tabla_actual;
                hash->tam = tam_actual;
                hash->cant = cant_actual;
                return false;
            }
            lista_iter_avanzar(iter_lista);
        }
        lista_iter_destruir(iter_lista);
    }
    for (size_t i = 0; i < tam_actual; i++) {
        while (!lista_esta_vacia(tabla_actual[i])) {
            campo_hash_t* campo_a_borrar = lista_borrar_primero(tabla_actual[i]);
            if (campo_a_borrar) {
                free(campo_a_borrar->clave);
                free(campo_a_borrar);
            }
        }
        lista_destruir(tabla_actual[i], NULL);
    }
    free(tabla_actual);
    return true;
}
/* ******************************************************************
 *                       PRIMITIVAS DEL HASH
 * *****************************************************************/
/* Crea el hash */
hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) {
        return NULL;
    }
    lista_t** tabla = malloc(sizeof(lista_t*) * TAM_INICIAL);
    if (!tabla) {
        free(hash);
        return NULL;
    }
    hash->cant = 0;
    hash->tam = TAM_INICIAL;
    inicializar_hash_tabla(tabla, hash->tam);
    hash->destruir_dato = destruir_dato;
    hash->tabla = tabla;
    return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    int posicion = hash_obtener_posicion(hash, clave);
    if (posicion == NO_DATA_FOUND) {
        size_t pos = fnv_hash(clave, hash->tam);
        campo_hash_t* campo_nuevo = crear_campo(clave, dato);
        if (!campo_nuevo) {
            return false;
        }
        bool resultado_insertar = lista_insertar_primero(hash->tabla[pos], campo_nuevo);
        if (!resultado_insertar) {
            free(campo_nuevo);
            return false;
        }
        hash->cant++;
        if (hash->cant > hash->tam * CARGA_MAXIMA_HASH) {
            bool resultado_redimensionar = hash_redimensionar(hash);
            if (!resultado_redimensionar) {
                //deberia volver atras los cambios si falla la redimension??
                return false;
            }
        }
    } else {
        campo_hash_t* campo_a_guardar = hash_obtener_campo(hash, (size_t)posicion, clave);
        if (hash->destruir_dato) {
            campo_hash_t* campo_anterior = campo_a_guardar;
            hash->destruir_dato(campo_anterior->valor);
        }
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
    int posicion = hash_obtener_posicion(hash, clave);
    if (posicion == NO_DATA_FOUND) {
        return NULL;
    }
    campo_hash_t* campo_a_borrar = hash_eliminar_campo(hash, (size_t)posicion, clave);
    if (!campo_a_borrar) {
        return NULL;
    }
    void* dato_campo_borrado = campo_a_borrar->valor;
    free(campo_a_borrar->clave);
    free(campo_a_borrar);
    hash->cant--;
    if (hash->cant < hash->tam / CARGA_MINIMA_HASH && hash->tam / TAM_REDIMENSION >= TAM_INICIAL) {
        bool resultado_redimensionar = hash_redimensionar(hash);
        //Deberia volver atras los cambios al fallar la redimension??
        if (!resultado_redimensionar) {
            return NULL;
        }

    }
    return dato_campo_borrado;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave) {
    int posicion = hash_obtener_posicion(hash, clave);
    if (posicion == NO_DATA_FOUND) {
        return NULL;
    }
    campo_hash_t* campo_buscado = hash_obtener_campo(hash, (size_t)posicion, clave);
    if (campo_buscado) {
        void* dato_campo_buscado = campo_buscado->valor;
        return dato_campo_buscado;
    }
    return NULL;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave) {
    return (hash_obtener_posicion(hash, clave) != NO_DATA_FOUND) ? true : false;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada
 */
size_t hash_cantidad(const hash_t *hash) {
    return hash->cant;
}

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida
 */
void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->tam; i++) {
        while (!lista_esta_vacia(hash->tabla[i])) {
            campo_hash_t* campo_a_borrar = lista_borrar_primero(hash->tabla[i]);
            if (campo_a_borrar) {
                destruir_campo(campo_a_borrar, hash->destruir_dato);
            }
        }
        lista_destruir(hash->tabla[i], NULL);
    }
    free(hash->tabla);
    free(hash);
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* iter_hash = malloc(sizeof(hash_iter_t));
    if(!iter_hash) {
        return NULL;
    }
    iter_hash->hash = hash;
    size_t pos = 0;
    while(lista_esta_vacia(iter_hash->hash->tabla[pos]) && pos < iter_hash->hash->tam - 1) {
        pos++;
    }
    iter_hash->posicion = pos;
    lista_iter_t* iter_lista = lista_iter_crear(iter_hash->hash->tabla[pos]);
    if (!iter_lista) {
        free(iter_hash);
        return NULL;
    }
    iter_hash->iter_lista = iter_lista;
    return iter_hash;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }
    bool avanzar = lista_iter_avanzar(iter->iter_lista);
    if (!avanzar) {
        return false;
    }
    if (lista_iter_al_final(iter->iter_lista)) {
        if (iter->posicion < iter->hash->tam - 1) {
            iter->posicion++;
        } else {
            return true;
        }
        lista_iter_destruir(iter->iter_lista);
        while (lista_esta_vacia(iter->hash->tabla[iter->posicion]) && iter->posicion < iter->hash->tam - 1) {
            iter->posicion++;
        }
        lista_iter_t* iter_lista = lista_iter_crear(iter->hash->tabla[iter->posicion]);
        if (!iter_lista) {
            return false;
        }
        iter->iter_lista = iter_lista;
    }
    return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter->iter_lista);
    if (!campo_actual) {
        return NULL;
    }
    return campo_actual->clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter) {
    bool pos_al_final = (iter->posicion == iter->hash->tam - 1) ? true : false;
    bool esta_al_final = lista_iter_al_final(iter->iter_lista);
    return (esta_al_final && pos_al_final);
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter) {
    lista_iter_destruir(iter->iter_lista);
    free(iter);
}