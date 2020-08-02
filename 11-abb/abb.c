#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "lista.h"
enum tipo_arbol {RAIZ = 1, IZQ = 2, DER = 3};
/* ******************************************************************
 *                           STRUCTS
 * *****************************************************************/
typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    char* clave;
    void* dato;
} nodo_abb_t;

struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t comparar;
};

struct abb_iter {
    const abb_t* abb;
    //Se tiene que implementar como una pila.
    lista_t* lista_abb;
};

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/
//funcion auxiliar a partir de la cual se va a crear un nodo vacio y devolverá un puntero al mismo.
nodo_abb_t* crear_nodo_abb(const char* clave, void* dato) {
    //me guardo espacio en memoria para el nodo.
    nodo_abb_t* nodo_abb = malloc(sizeof(nodo_abb_t));
    //Si no se creó, retorno NULL.
    if (!nodo_abb) {
        return NULL;
    }
    // inicializo izq en NULL;
    nodo_abb->izq = NULL;
    // inicializo der en NULL;
    nodo_abb->der = NULL;
    // hago una copia de la clave pasada;
    nodo_abb->clave = strdup(clave);
    //si falla la copia de la clave, libero el nodo y retorno NULL
    if (!nodo_abb->clave) {
        free(nodo_abb);
        return NULL;
    }
    // guardo el puntero al dato;
    nodo_abb->dato = dato;
    //retorno el nodo inicializado.
    return nodo_abb;
}

//funcion auxiliar que busca un nodo a recibiendo un nodo
//Si encuentro el nodo a partir de la clave, retorno un puntero al mismo.
//Si no existe un nodo para la clave buscada, se retorna NULL
nodo_abb_t* rec_buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t func_comparacion, const char* clave) {
    //caso base. Si no hay nodo, retornar NULL
    if (!nodo) {
        return NULL;
    }

    //comparo la clave del nodo con la clave buscada.
    //Si la clave es igual a cero, retorno el puntero al nodo que estoy buscando.
    if (func_comparacion(nodo->clave, clave) == 0) {
        return nodo;
    }

    //Si la clave de mi nodo es mayor, la clave buscada solo puede estar a la izquierda por propiedad del abb.
    if (func_comparacion(nodo->clave, clave) > 0) {
        //busco el nodo en el hijo izquierdo.
        return rec_buscar_nodo(nodo->izq, func_comparacion, clave);
    }
    //Si la clave de mi nodo es menor, la clave buscada solo puede estar a la derecha por propiedad del abb.
    if (func_comparacion(nodo->clave, clave) < 0) {
        //busco el nodo en el hijo derecho.
        return rec_buscar_nodo(nodo->der, func_comparacion, clave);
    }

    //me pide retornar algo por defecto. Tengo que revisar.
    return NULL;
}

//funcion auxiliar que busca un nodo a recibiendo el arbol. Es un wrapper de la funcion recursiva.
nodo_abb_t* buscar_nodo(const abb_t* abb, const char* clave) {
    //retorna el nodo buscado o NULL en caso de no existir.
    return rec_buscar_nodo(abb->raiz, abb->comparar, clave);
}

//funcion auxiliar que voy a usar para buscar el padre del nodo en donde voy a insertar, actualizar o borrar.
//tengo que ver si es facilmente implementable en el resto de primitivas.
//Por ahora solo la voy a dejar en el abb_borrar, ya que necesito actualizar la referencia del padre al hijo.
nodo_abb_t* rec_buscar_nodo_padre(nodo_abb_t* nodo, abb_comparar_clave_t func_comparacion, const char* clave) {
    //caso base. Si no hay nodo, retorno NULL
    if (!nodo) {
        return NULL;
    }

    //comparo la clave del nodo con la clave buscada.
    //Si la clave es igual a cero, el nodo no tiene padre (es la raiz). Asi que retorno NULL.
    if (func_comparacion(nodo->clave, clave) == 0) {
        return NULL;
    }

    //Si mi nodo no tiene hijos, entonces es hoja, asi que tiene que ser el padre.
    if (!nodo->izq && !nodo->der) {
        return nodo;
    }

    //Si la clave provista, es menor a la de mi nodo, no tiene sentido
    // que compare contra lo que haya a la derecha, asi que me muevo por el hijo izquierdo.
    if (func_comparacion(nodo->clave, clave) > 0) {
        //Si el hijo izquierdo no existe, retorno el nodo
        if (!nodo->izq) {
            return nodo;
        }
        //si la clave de mi hijo izquierdo es la que busco, retorno el nodo.
        if (func_comparacion(nodo->izq->clave, clave) == 0) {
            return nodo;
        }
        //sino, entro a mi hijo izquierdo y continuo buscando.
        return rec_buscar_nodo_padre(nodo->izq, func_comparacion, clave);
    }

    //Si la clave provista, es mayor a la de mi nodo, no tiene sentido
    // que compare contra lo que haya a la izquierda, asi que me muevo por el hijo derecho.
    if (func_comparacion(nodo->clave, clave) < 0) {
        //Si el hijo derecho no existe, retorno el nodo
        if (!nodo->der) {
            return nodo;
        }
        //si la clave de mi hijo derecho es la que busco, retorno el nodo.
        if (func_comparacion(nodo->der->clave, clave) == 0) {
            return nodo;
        }
        //sino, entro a mi hijo derecho y continuo buscando.
        return rec_buscar_nodo_padre(nodo->der, func_comparacion, clave);
    }

    //me pide retornar algo por defecto. Tengo que revisar.
    return NULL;
}

//funcion auxiliar que busca un nodo padre recibiendo el arbol. Es un wrapper de la funcion recursiva.
nodo_abb_t* buscar_nodo_padre(abb_t* abb, const char* clave) {
    //retorna el nodo buscado o NULL en caso de no existir.
    return rec_buscar_nodo_padre(abb->raiz, abb->comparar, clave);
}


//funcion auxiliar que recibe el hijo izquierdo del nodo a eliminar y recorre el arbol lo más a la derecha posible del mismo.
nodo_abb_t* rec_buscar_nodo_reemplazo(nodo_abb_t* nodo) {
    //caso base. Si no hay nodo, retornar NULL
    if (!nodo) {
        return NULL;
    }

    //verifico si hay hijo derecho. Si hay hijo derecho, lo navego.
    if (nodo->der) {
        //busco el nodo en el hijo derecho.
        return rec_buscar_nodo_reemplazo(nodo->der);
    } else {
        //si el nodo no tiene hijo derecho, es el maximo nodo menor al nodo a borrar, asi que lo retorno.
        return nodo;
    }
}

//funcion auxiliar que destruye un nodo.
void*  abb_nodo_destruir(nodo_abb_t* nodo, bool liberar_nodo) {
    //me guardo un puntero al dato.
    void* dato = nodo->dato;
    //libero la clave.
    free(nodo->clave);
    //por ultimo, libero el nodo si corresponde.
    if (liberar_nodo) {
        free(nodo);
    }
    //retorno el dato contenido en el nodo
    return dato;
}
//funcion auxiliar que recorre nodo a nodo en post order. al llegar al ultimo nodo, libera el nodo, junto con su dato. y la clave.
void rec_abb_destruir(nodo_abb_t* nodo, abb_destruir_dato_t funcion_destruccion) {
    //si no hay nodo, no tengo que hacer nada, asi que salgo.
    if (!nodo) {
        return;
    }

    //navego al hijo izquierdo.
    rec_abb_destruir(nodo->izq, funcion_destruccion);
    //navego al hijo derecho.
    rec_abb_destruir(nodo->der, funcion_destruccion);

    //me creo una variable liberar_nodo por legibilidad.
    bool liberar_nodo = true;
    //ahora opero sobre el nodo, destruyendo la clave y el nodo. Recupero el dato y lo elimino si hay una funcion de destruccion.
    void* dato = abb_nodo_destruir(nodo, liberar_nodo);
    //Si hay una funcion de destruccion, borro el dato.
    if (funcion_destruccion) {
        funcion_destruccion(dato);
    }
}

//funcion auxiliar que retorna cantidad de hijos de un arbol.
//PRE: Se pasa un nodo no nulo.
//Post: Retorna la cantidad de hijos del nodo.
size_t contar_hijos_nodo(nodo_abb_t* nodo) {
    size_t cant_hijos = 0;
    //por precondicion no deberia pasarse un nodo nulo, pero por las dudas retorno 0 ante la posibilidad.
    if (!nodo) {
        return cant_hijos;
    }
    //si tiene hijo izquierdo, actualizo la cantidad de hijos
    if (nodo->izq) {
        cant_hijos++;
    }
    //si tiene hijo derecho, actualizo la cantidad de hijos
    if (nodo->der) {
        cant_hijos++;
    }
    //retorno la cantidad de hijos.
    return cant_hijos;
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ABB
 * *****************************************************************/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    //me guardo espacio en memoria para el abb.
    abb_t* abb = malloc(sizeof(abb_t));
    //si no se creo, retorno NULL.
    if (!abb) {
        return NULL;
    }
    // inicializo raiz en NULL;
    abb->raiz = NULL;
    // Inicializo cant en 0;
    abb->cant = 0;
    // Inicializo funcion destruccion de dato destruir con funcion pasada por parametro;
    abb->destruir = destruir_dato;
    // Inicializo funcion comparacion comparar con funcion pasada por parametro;
    abb->comparar = cmp;

    //retorno puntero al abb
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    //Busco el nodo padre. Si el nodo buscado no tiene padre o bien es la raiz, o bien el abb está vacio.
    //Si encuentro al padre, tengo que ver si hay que insertar al hijo izq o al hijo der.
    nodo_abb_t* nodo_padre = rec_buscar_nodo_padre(arbol->raiz, arbol->comparar, clave);
    //Si no hay nodo padre, tengo que verificar que el arbol no esté vacio.
    if (!nodo_padre) {
        //Si el arbol está vacio, puedo insertar el dato.
        if (!arbol->raiz) {
            //creo un nodo nuevo con la clave y el valor pasados.
            nodo_abb_t* nuevo_nodo = crear_nodo_abb(clave, dato);
            //si fallo la creacion del nodo, retorno false.
            if (!nuevo_nodo) {
                return false;
            }
            //guardo el nuevo nodo en la raiz
            arbol->raiz = nuevo_nodo;
            //actualizo la cantidad de elementos.
            arbol->cant++;
            //retorno true, porque se insertó correctamente.
            return true;
        }
        //si el arbol no está vacio y aun asi retornó null, puede ser que el nodo a insertar sea la raiz.
        //Comparo la clave de la raiz con la buscada
        if(arbol->comparar(arbol->raiz->clave, clave) == 0) {
            //Si tengo una funcion destruir, destruyo el dato anterior.
            if(arbol->destruir) {
                arbol->destruir(arbol->raiz->dato);
            }
            //ahora si, actualizo el dato.
            arbol->raiz->dato = dato;
            //retorno true
            return true;
        } else {
            //si no hay nodo padre y hay un dato en la raiz pero no es el buscado, retorno false, ya que algo salio mal.
            return false;
        }
    //Si encontré un nodo padre, tengo dos opciones. Si la clave del padre es mayor, inserto a la izquierda. Si es menor, inserto a la derecha.
    //Una vez que se de que lado tengo que insertar, me fijo el dato ya existe.
    } else {
        //Si la clave del padre es mayor a la buscada, me fijo si tiene hijo a la izquierda.
        //Puedo comparar por las dudas que se corresponda la clave, pero deberia ser una precondicion de la funcion auxiliar buscar padre.
        if (arbol->comparar(nodo_padre->clave, clave) > 0 ) {
            //Si ya existe el campo, actualizo el dato y elimino el anterior.
            if (nodo_padre->izq) {
                //Si tengo una funcion destruir, destruyo el dato anterior.
                if(arbol->destruir) {
                    arbol->destruir(nodo_padre->izq->dato);
                }
                //ahora si, actualizo el dato.
                nodo_padre->izq->dato = dato;
                //retorno true
                return true;
            } else {
                //creo un nodo nuevo con la clave y el valor pasados.
                nodo_abb_t* nuevo_nodo = crear_nodo_abb(clave, dato);
                //si fallo la creacion del nodo, retorno false.
                if (!nuevo_nodo) {
                    return false;
                }
                //guardo el nuevo nodo en el hijo izquierdo
                nodo_padre->izq = nuevo_nodo;
                //actualizo la cantidad de elementos.
                arbol->cant++;
                //retorno true, porque se insertó correctamente.
                return true;
            }
        //no tiene sentido comparar, ya que por precondicion, si trae un nodo, no va a ser el buscado, sino el padre.
        //Solo lo dejo por las dudas y lo saco si funciona igual.
        } else if (arbol->comparar(nodo_padre->clave, clave) < 0 ) {
            //Si ya existe el campo, actualizo el dato y elimino el anterior.
            if (nodo_padre->der) {
                //Si tengo una funcion destruir, destruyo el dato anterior.
                if(arbol->destruir) {
                    arbol->destruir(nodo_padre->der->dato);
                }
                //ahora si, actualizo el dato.
                nodo_padre->der->dato = dato;
                //retorno true
                return true;
            } else {
                //creo un nodo nuevo con la clave y el valor pasados.
                nodo_abb_t* nuevo_nodo = crear_nodo_abb(clave, dato);
                //si fallo la creacion del nodo, retorno false.
                if (!nuevo_nodo) {
                    return false;
                }
                //guardo el nuevo nodo en el hijo derecho
                nodo_padre->der = nuevo_nodo;
                //actualizo la cantidad de elementos.
                arbol->cant++;
                //retorno true, porque se insertó correctamente.
                return true;
            }
        }
    }
    //si por algun motivo no entró en ningun lado, retorno false.
    return false;
}

//seguir instrucciones de lauti en video de borrar. (08/06/2020)
//Se localizar el nodo a eliminar.
// Hay tres casos posibles:
// Si el nodo es una hoja (es decir: no tiene hijos), se elimina.
// Si el nodo tiene un solo hijo, se elimina el nodo y se reemplaza con su hijo.
// Si el nodo tiene dos hijos no se elimina el nodo, sino que se reemplaza con alguna de estas dos opciones:
    // * El siguiente inorder (es decir, con el menor de sus hijos mayores)
    // * El anterior inorder (el mayor de sus hijos menores).

// Se llama a la eliminación recursiva en el subárbol correspondiente de acuerdo a la estrategia de eliminación elegida.
// Como se eligió o bien el menor de sus hijos mayores o el mayor de sus hijos menores,
// obligatoriamente al nodo a borrar le va a faltar un hijo, haciendo que se caiga en alguno de los dos primeros casos.
void *abb_borrar(abb_t *arbol, const char *clave) {
    //Si la clave no pertenece, retorno NULL.
    if (!abb_pertenece(arbol, clave )) {
        return NULL;
    }
    //Busco el nodo padre. Si el nodo buscado no tiene padre o bien es la raiz, o bien el abb está vacio.
    //Si encuentro al padre, tengo que ver si hay que borrar al hijo izq o al hijo der.
    nodo_abb_t* nodo_padre = rec_buscar_nodo_padre(arbol->raiz, arbol->comparar, clave);

    //si no encuentro un nodo padre, tengo que ver por las dudas que el nodo a eliminar no sea la raiz.
    //Voy a crear un contador de hijos y procesar la cantidad de hijos.
    //Ademas me guardo en un puntero el nodo a borrar
    nodo_abb_t* nodo_a_borrar = NULL;
    //Me guardo el tipo de nodo que voy a borrar, para ahorrar verificaciones.
    size_t tipo_nodo = 0;
    size_t cant_hijos = 0;
    if (!nodo_padre) {
        //Si el arbol está vacio, retorno NULL.
        if (!arbol->raiz) {
            return NULL;
        }
        //Si el arbol no está vacio, pero la clave no es la de la raiz, retorno NULL.
        if (arbol->comparar(arbol->raiz->clave, clave) != 0) {
            return NULL;
        }
        //Sino, tengo que verificar que tenga un hijo, dos o ninguno.
        cant_hijos = contar_hijos_nodo(arbol->raiz);

        //Me guardo la raiz ya que es el nodo a borrar
        nodo_a_borrar = arbol->raiz;

        //el tipo de nodo será una raiz
        tipo_nodo = RAIZ;

    //si hay nodo padre, entonces tengo que ver quién es el hijo.
    } else {
        //me fijo si es el hijo izquierdo el nodo que busco.
        if (arbol->comparar(nodo_padre->izq->clave, clave) == 0) {
            //Verifico que tenga un hijo, dos o ninguno.
            cant_hijos = contar_hijos_nodo(nodo_padre->izq);

            //Me guardo al hijo izquierdo ya que es el nodo a borrar
            nodo_a_borrar = nodo_padre->izq;

            //el tipo de nodo será hijo izquierdo
            tipo_nodo = IZQ;
        //si encontró nodo_padre, es porque si o si hay un hijo a borrar.
        // Así que si no es el izquierdo, tiene que ser el derecho.
        } else {
            //Verifico que tenga un hijo, dos o ninguno.
            cant_hijos = contar_hijos_nodo(nodo_padre->der);

            //Me guardo al hijo izquierdo ya que es el nodo a borrar
            nodo_a_borrar = nodo_padre->der;

            //el tipo de nodo será hijo derecho
            tipo_nodo = DER;
        }
    }

    //Ahora ya tengo un puntero al padre, si es que existe,
    //tengo el nodo a borrar (deberia haber salido antes si no existía) y tengo ademas la cantidad de hijos.
    //me creo una variable liberar_nodo por legibilidad.
    bool liberar_nodo = true;
    //Si no tiene hijos, destruyo el nodo junto con la clave y recupero el dato.
    if (cant_hijos == 0) {
        void* dato = abb_nodo_destruir(nodo_a_borrar, liberar_nodo);
        //si el nodo tenia padre, actualizo el puntero del padre a NULL.
        if (nodo_padre) {
            if (tipo_nodo == IZQ) {
                nodo_padre->izq = NULL;
            }
            if (tipo_nodo == DER) {
                nodo_padre->der = NULL;
            }
        } else {
            //Si no tiene padre, es la raiz. Entonces me aseguro de que el espacio liberado tenga valor NULL.
            arbol->raiz = NULL;
        }
        //Ahora si, disminuyo la cantidad de elementos
        arbol->cant--;
        //retorno el dato contenido en el nodo borrado.
        return dato;
    //si el arbol tiene un solo hijo, se elimina el nodo y se reemplaza con su hijo.
    } else if (cant_hijos == 1) {
        //me guardo el puntero al hijo que corresponda, ya que despues el padre tendra que apuntar a este.
        nodo_abb_t* hijo = NULL;
        if (nodo_a_borrar->izq) {
            hijo = nodo_a_borrar->izq;
        } else {
            hijo = nodo_a_borrar->der;
        }

        //borro el nodo y me guardo el dato.
        void* dato = abb_nodo_destruir(nodo_a_borrar, liberar_nodo);
        //Si el nodo es raiz, actualizo la raiz con el hijo.
        if (tipo_nodo == RAIZ) {
            //ahora la raiz apunta al nieto.
            arbol->raiz = hijo;
        //Si el nodo era el hijo izquierdo, actualizo el hijo izquierdo con el nieto guardado.
        } else if (tipo_nodo == IZQ) {
            nodo_padre->izq = hijo;
        //Si el nodo era el hijo derecho, actualizo el hijo izquierdo con el nieto guardado.
        } else if (tipo_nodo == DER) {
            nodo_padre->der = hijo;
        }
        //disminuyo la cantidad de elementos
        arbol->cant--;
        //retorno el dato contenido en el nodo borrado.
        return dato;
    //Si el nodo tiene hijo izquierdo e hijo derecho, tengo que buscar un reemplazo.
    } else if (cant_hijos == 2) {
        //voy a buscar un reemplazo buscando el mayor de sus hijos menores.
        nodo_abb_t* nodo_reemplazo = rec_buscar_nodo_reemplazo(nodo_a_borrar->izq);
        //si falla la busqueda, retorno NULL.
        if (!nodo_reemplazo) {
            return NULL;
        }
        //Me guardo puntero a la clave del reemplazante. En realidad la duplico para reutilizar el elimnar nodo, que libera la clave.
        //Recordar que a esto se le hace un malloc internamente (aunque el borrado de clave la libera)
        char* clave_reemplazante = strdup(nodo_reemplazo->clave);
        //borro el reemplazante y me guardo su dato.
        //void* dato_reemplazante = abb_nodo_destruir(nodo_reemplazo, liberar_nodo);
        //Aca voy a probar llamando a la eliminacion de forma recursiva. Deberia tener solo un hijo o ninguno.
        void* dato_reemplazante = abb_borrar(arbol, clave_reemplazante);
        //Si falla la eliminacion, libero la clave copiada y retorno NULL
        if(!dato_reemplazante) {
            free(clave_reemplazante);
            return NULL;
        }

        //me guardo puntero a la clave del nodo a borrar
        char* clave_a_borrar = nodo_a_borrar->clave;
        //piso la clave del nodo a borrar.
        nodo_a_borrar->clave = clave_reemplazante;
        //me guardo puntero del dato del nodo a borrar.
        void* dato_a_borrar = nodo_a_borrar->dato;
        //piso el dato del nodo a borrar.
        nodo_a_borrar->dato = dato_reemplazante;
        //libero la clave del nodo a borrar, que ya no la necesito.
        free(clave_a_borrar);

        //entiendo que no deberia actualizar las referencias en el padre. Dejo comentario por las dudas por si falla algo.
        //ahora si, retorno el dato del "nodo borrado".
        return dato_a_borrar;
    }

    //me pide retornar algo por defecto. Tengo que revisar.
    return NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    //Busco si ya existe el nodo.
    nodo_abb_t* nodo = buscar_nodo(arbol, clave);
    //Si encontró el nodo, retorno el Dato, sino, retorno NULL.
    return (nodo) ? nodo->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    //Busco si ya existe el nodo.
    nodo_abb_t* nodo = buscar_nodo(arbol, clave);
    //Si encontró el nodo, retorno true, sino, retorno false.
    return (nodo) ? true : false;
}

//pimitiva que retorna la cantidad de elementos del arbol.
size_t abb_cantidad(abb_t *arbol) {
    return arbol->cant;
}

//primitiva que destruye el arbol en sentido post-order.
void abb_destruir(abb_t *arbol) {
    //libero toda la estructura de nodos contenida.
    rec_abb_destruir(arbol->raiz, arbol->destruir);
    //ahora si, libero el arbol.
    free(arbol);
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/
//funcion recursiva que se encarga de iterar en el abb a partir de los nodos.
void rec_abb_in_order(nodo_abb_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
    if (!nodo) {
        return;
    }
    //Recorrido in-order.
    //Primero visito el nodo izquierdo.
    rec_abb_in_order(nodo->izq, visitar, extra);

    //ejecuto la funcion visitar. Si retorna true, continuo visitando nodos.
    bool resultado = visitar(nodo->clave , nodo->dato, extra);
    //Si retorna false, dejo de iterar.
    if (!resultado) {
        return;
    }
    //Una vez que visito el nodo padre, ahora recorro al hijo derecho.
    rec_abb_in_order(nodo->der, visitar, extra);
}

//se navega in order por el abb
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    rec_abb_in_order(arbol->raiz, visitar, extra);
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//se hace con una pila para guardar el estado del iterador. (ver clase 2020-06-12 Clase Práctica)
//Crea el iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    //Asigno espacio en memoria para mi iterador.
    abb_iter_t* iter_abb = malloc(sizeof(abb_iter_t));
    //Si falla la asignacion, retorno NULL
    if(!iter_abb) {
        return NULL;
    }
    //Me guardo puntero al arbol
    iter_abb->abb = arbol;

    //Asigno en memoria una lista de nodos abb.
    lista_t* lista_abb = lista_crear();
    //Si falla, libero el iterador y retorno NULL.
    if (!lista_abb) {
        free(iter_abb);
        return NULL;
    }

    //Si hay un nodo raiz al momento de crear el iterador, lo guardo en el iterador.
    if(arbol->raiz) {
        //Apilo la raiz
        bool resultado = lista_insertar_primero(lista_abb, arbol->raiz);
        //Si falla el apilado, se libera la lista y el iterador y se retorna NULL.
        if(!resultado) {
            free(lista_abb);
            free(iter_abb);
            return NULL;
        }
    }

    //si se insertó todo correctamente, inicializo el campo del struct
    iter_abb->lista_abb = lista_abb;

    //caso contrario, retorno el iterador inicializado.
    return iter_abb;
}


bool abb_iter_in_avanzar(abb_iter_t *iter) {
    //Si el iterador se encuentra al final retorno false.
    if (abb_iter_in_al_final(iter)) {
        return false;
    }
    //desapilo el primer elemento de mi lista.
    nodo_abb_t* nodo = lista_borrar_primero(iter->lista_abb);
    if (!nodo) {
        //si falló el desapilado, retorno false.
        return false;
    }
    //Apilo hijo derecho e hijo izquierdo del desapilado
    bool resultado_der = lista_insertar_primero(iter->lista_abb, nodo->der);
    //Si falla el apilado, vuelvo a apilar el nodo para dejar todo en el estado anterior y retorno false
    if(!resultado_der) {
        lista_insertar_primero(iter->lista_abb, nodo);
        return false;
    }

    bool resultado_izq = lista_insertar_primero(iter->lista_abb, nodo->izq);
    //Si falla el apilado, deberia desapilar el hijo derecho y apilar el nodo original para dejar en el estado anterior y retornar false.
    if(!resultado_izq) {
        lista_borrar_primero(iter->lista_abb);
        lista_insertar_primero(iter->lista_abb, nodo);
        return false;
    }
    //si pude apilar los hijos, retorno true.
    return true;
}

//veo el tope de la pila
const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return NULL;
    }
    nodo_abb_t* nodo = lista_ver_primero(iter->lista_abb);
    if (!nodo) {
        return NULL;
    }
    return nodo->clave;;
}

//verifico si la pila está vacia.
bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return lista_esta_vacia(iter->lista_abb);
}

//destruyo la pila.
void abb_iter_in_destruir(abb_iter_t* iter){
    //libero la lista del abb.
    lista_destruir(iter->lista_abb, NULL);
    //libero el iterador del abb.
    free(iter);
}