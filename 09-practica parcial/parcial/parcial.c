/****************************************************************************************/
Ejercicio 2:
a) para implementar una primitiva que permita cambiar la funcion de prioridad de un Heap,
 la única implementacion viable es O(nlogn), sacando todos los elementos de dicho 
 heap, cambiando la funcion y volviendo a guardarlos.

Falso:
En términos estrictos, para notación Oh, la primitiva sería O(nlogn), aunque terminaría 
siendo O(n), si en vez de sacar los elementos del heap y guardarlos, se aplicara
Heapify. Es decir, hacer downheap desde el último elemento del arreglo hasta el primero.

b) Si en un heap cuyo estado inicial es [10 7 5 5 6 3 4 3 2] se encola un 8, su estado 
final será [10 8 5 5 7 3 4 3 2 6]

Verdadero:
Su padre es (n - 1) / 2 => 6. Hace upheap. Luego vuelve a hacer upheap con su nuevo padre,
el 7. Se queda como hijo izquierdo del 10, que es la raíz.

c) Me convendria tener una implementacion de cola de prioridad cuyo encolar 
sea O(1) aunque desencolar sea O(n), en vez de la implementacion usando Heaps.

Falso:
Entiendo que la magia de la cola de prioridad es poder obtener el máximo o el minimo
en tiempo constante.

/****************************************************************************************/
3)a)

#include "lista.h"
#include <string.h>
//y aca va lo de posix.

//esta es la estructura del ab provista por lxs profesorxs.
typedef struct ab {
    struct ab* izq;
    struct ab* der;
    char* clave;
} ab_t;

//funcion auxiliar para obtener puntero a la persona buscada. 
//Es preorder porque me interesa primero consultar en donde estoy
//parado, antes que recorrer el arbol.
//PRE: El arbol existe.
//POST: Retorna puntero al nodo correspondiente, siempre que exista. Sino, retorna NULL.
ab_t* obtener_ab(ab_t* actual, const char* clave) {
    if (!actual) {
        return NULL;
    }

    if (strcmp(actual->clave,clave) == 0) {
        return actual;
    }
    obtener_ab(actual->izq, clave);
    obtener_ab(actual->der, clave);
    
    return NULL;
}

//Creo la funcion con la firma solicitada. La voy a utilizar para verificar que exista el elemento y luego
//llamar a la funcion auxiliar recursiva.
//PRE: Existe el arbol binario.
//POST: Retorna puntero a lista con los elementos a partir de la persona solicitada, en orden Post Order. 
//Retorna NULL si falla o si no existe la persona.
lista_t* ab_debajo_de_postorder(ab_t* ab, const char* clave) {    
    lista_t* lista_debajo_de = lista_crear();
    if (!lista_debajo_de) {
        return NULL;
    }
    ab_t* persona_piramide = obtener_ab(ab, clave);
    if (!persona_piramide) {
        free(lista_debajo_de);
        return NULL;
    }
    ab_debajo_de_postorder_rec(persona_piramide, lista_debajo_de);

    return lista_debajo_de;
}

//funcion auxiliar recursiva para mi primitiva. Simplemente se encarga de guardar al final de la lista
//los elementos del subarbol que reciba como parametro.
//POST: Actualiza la lista con los valores del subarbol que vaya recorriendo en post order.
void ab_debajo_de_postorder_rec(ab_t* actual, lista_t* lista) {
    if (!actual) {
        return;
    }
    ab_debajo_de_postorder_rec(actual->izq, lista);
    ab_debajo_de_postorder_rec(actual->der, lista);
    lista_insertar_al_final(lista, (void*)actual->clave);
}



la funcion aux obtener_ab() es O(n) porque tiene que recorrer todos los nodos, para buscar a la persona.

la funcion aux ab_debajo_de_postorder_rec() es O(n) porque el peor caso es que se busque la raiz y 
tenga que recorrer todo.

la primitiva ab_debajo_de_postorder() es O(n), porque llama a las funciones auxiliares, que son O(n),
y el resto de las operaciones son O(1)



/****************************************************************************************/
4) Obtener una lista con las claves y valores existentes dentro del hash.

#include <stdlib.h>
#include "lista.h"
#include "hash.h"

//estructura para guardar claves y valores en la lista. Provisto por lxs profesorxs.
typedef struct hash_par {
    const char* clave;
    void* dato;
} hash_par_t;

// struct de mi hash abierto:
typedef struct hash {
    lista_t** tabla;
    size_t cant;
    size_t tam;
    hash_destruir_dato_t* destruir;
} hash_t;

typedef struct campo_hash {
    char* clave;
    void* valor;
} campo_hash_t;


lista_t* hash_claves_valores(const hash_t* hash) {
    lista_t* claves_valores = lista_crear();
    if (!claves_valores) return NULL;

    for (size_t i = 0; i < hash->tam; i++) {
        lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
        while (!lista_iter_al_final(iter)) {
            campo_hash_t* campo = lista_iter_ver_actual(iter);
            hash_par_t* campo_clave = malloc(sizeof(hash_par_t));
            if (!campo_clave) {
                lista_destruir(claves_valores, free);
                lista_iter_destruir(iter);
                return NULL;
            }
            strcpy(campo_clave->clave, campo->clave);
            campo_clave->dato = campo->valor;            

            lista_insertar_ultimo(claves_valores, campo_clave);
            lista_iter_avanzar(iter);
        }
        lista_iter_lista_destruir(iter);
    }
    return claves_valores;
}