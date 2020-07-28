#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"
#include "hash.h"
#define TAM_INICIAL 31

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
    printf("Inicialicé %zu posiciones con listas", count);
}

bool necesita_redimension(hash_t* hash) {
    //funcion que verifica si se necesita redimensionar.
    //Si la cantidad de elementos es mayor a tres veces el tamaño del hash, hay que redimensionar
    if (hash->cant > hash->tam * 3) {
        return true;
    }
    //Si la cantidad de elementos es menor a un tercio de la capacidad total del hash
    //y redimensionar no hará que el tamaño nuevo sea menor al tamaño inicial, hay que redimensionar
    if (hash->cant < hash->tam / 3 && hash->tam / 2 >= TAM_INICIAL) {
        return true;
    }
    //esto luego se puede convertir en esta linea: Ahora nomás para ver por separado.
    //return (((hash->cant > hash->tam * 3)) || (hash->cant < hash->tam / 3 && hash->tam / 2 >= TAM_INICIAL)) ? true : false;
    //en cualquier otro caso, retornar false
    return false;
}

campo_hash_t* hash_buscar(hash_t* hash, size_t posicion, const char* clave, bool borrar) {
    //si la lista de la posicion donde debia caer el campo está vacia, retorno NULL
    if (lista_esta_vacia(hash->tabla[posicion])) {
        return NULL;
    }
    //si no está vacia, recorro la lista con un iterador de lista.
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    while (!lista_iter_al_final(iter_lista_hash)) {
        campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
        if (strcmp(clave, campo_actual->clave) == 0) {
            //si encuentro el campo, y es para borrar, lo borro.
            if (borrar) {
                //me guardo el puntero al resultado (que deberia ser el mismo que el del campo_actual), ya que no deberia ser NULL.
                void* resultado = lista_iter_borrar(iter_lista_hash);
                //Si es NULL, es porque falló al borrar. Retorno NULL.
                if (!resultado) {
                    return NULL;
                }
            }
            // Destruyo el iterador y retorno un puntero al campo (aca retorno campo_actual, que deberia coincidir con resultado).
            lista_iter_destruir(iter_lista_hash);
            return campo_actual;
        }
        //avanzo al siguiente elemento de la lista.
        lista_iter_avanzar(iter_lista_hash);
    }
    //Si termina de recorrer el iterador y no encontró la lista, destruyo el iterador y retorno NULL.
    lista_iter_destruir(iter_lista_hash);
    return NULL;
}

//Ajusto la funcion hash_eliminar_campo. Ahora recibe la posicion e itera hasta encontrar el campo, lo elimina del iterador y lo devuelve.
//PRE: El campo existe en la lista pasada por parametro
//POST: Itero la lista hasta encontrar la posicion del campo. Elimino el elemento y lo retorno.
campo_hash_t* hash_eliminar_campo(const hash_t* hash, size_t posicion, const char* clave) {
    //Creo un iterador de lista para recorrerla.
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);

    //Me guardo el campo al que apunta el iterador de lista.
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);

    //Si el campo no era el buscado, va a recorrer el iterador hasta encontrarlo, o hasta que se acabe la lista.
    //Por la precondicion, deberia encontrarlo siempre.
    while (!lista_iter_al_final(iter_lista_hash) && strcmp(clave, campo_actual->clave) != 0) {
        //avanzo al siguiente elemento de la lista.
        lista_iter_avanzar(iter_lista_hash);
    }
    //me guardo el puntero al resultado (que deberia ser el mismo que el del campo_actual), ya que no deberia ser NULL.
    void* resultado = lista_iter_borrar(iter_lista_hash);
    //Si es NULL, es porque falló al borrar. Retorno NULL.
    if (!resultado) {
        return NULL;
    }
    //Destruyo el iterador.
    lista_iter_destruir(iter_lista_hash);

    //retorno un puntero al campo.
    return campo_actual;
}

//Ajusto la funcion hash_obtener_campo. Ahora recibe la posicion e itera hasta encontrar el campo y lo devuelve.
//PRE: El campo existe en la lista pasada por parametro
//POST: Itero la lista hasta encontrar la posicion del campo. Retorno un puntero al campo.
campo_hash_t* hash_obtener_campo(const hash_t* hash, size_t posicion, const char* clave) {
    //Creo un iterador de lista para recorrerla.
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);

    //Me guardo el campo al que apunta el iterador de lista.
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);

    //Si el campo no era el buscado, va a recorrer el iterador hasta encontrarlo, o hasta que se acabe la lista.
    //Por la precondicion, deberia encontrarlo siempre.
    while (!lista_iter_al_final(iter_lista_hash) && strcmp(clave, campo_actual->clave) != 0) {
        //avanzo al siguiente elemento de la lista.
        lista_iter_avanzar(iter_lista_hash);
    }
    //Al salir del while, destruyo el iterador.
    lista_iter_destruir(iter_lista_hash);
    //retorno un puntero al campo.
    return campo_actual;
}

int hash_obtener_posicion(const hash_t* hash, const char* clave) {
    //busco en qué posicion deberia estar el valor.
    size_t posicion = fnv_hash(clave, hash->tam);

    //si la lista de la posicion donde debia caer el campo está vacia, retorno -1
    if (lista_esta_vacia(hash->tabla[posicion])) {
        return -1;
    }
    int pos = (int)posicion;
    //si no está vacia, recorro la lista con un iterador de lista.
    lista_iter_t* iter_lista_hash = lista_iter_crear(hash->tabla[posicion]);
    while (!lista_iter_al_final(iter_lista_hash)) {
        campo_hash_t* campo_actual = lista_iter_ver_actual(iter_lista_hash);
        if (strcmp(clave, campo_actual->clave) == 0) {
            // Destruyo el iterador y retorno la posicion del hash donde está el campo
            lista_iter_destruir(iter_lista_hash);
            return pos;
        }
        //avanzo al siguiente elemento de la lista.
        lista_iter_avanzar(iter_lista_hash);
    }
    //Si termina de recorrer el iterador y no encontró la lista, destruyo el iterador y retorno NULL.
    lista_iter_destruir(iter_lista_hash);
    return -1;
}

void destruir_campo(campo_hash_t* campo, hash_destruir_dato_t destruir_dato) {
    //si el dato debe ser liberado, se libera.
    if (destruir_dato) {
        destruir_dato(campo->valor);
    }
    //se libera la clave del campo.
    free(campo->clave);
    //Por ultimo, se libera el campo. Esto se podria hacer en hash destruir, liberando con free en lista_destruir(lista, free)
    free(campo);
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
        return NULL;
    }
    hash->cant = 0;
    hash->tam = TAM_INICIAL;
    hash->destruir_dato = destruir_dato;
    hash->tabla = tabla;
    inicializar_hash_tabla(hash->tabla, hash->tam);
    return hash;
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    //busco en qué posicion deberia guardar el valor.
    size_t posicion = fnv_hash(clave, hash->tam);
    //con la funcion auxiliar, me fijo si existe ya ese campo en el hash. Si existe, me guardo el puntero, sino, voy a recibir NULL.
    campo_hash_t* campo_a_guardar = hash_buscar(hash, posicion, clave, false);
    //Si el campo no existia en la lista correspondiente a la posicion donde deberia ir en el hash, inicializo el campo y lo inserto primero en la lista.
    if (!campo_a_guardar) {
        //Creo un puntero de tipo campo, para guardar la información del campo nuevo.
        campo_hash_t* campo_nuevo = malloc(sizeof(campo_hash_t));
        //si falla retorno false. Hasta acá no creé asigné memoria dinamica a ninguna estructura. No deberia ser necesario liberar nada.
        if (!campo_nuevo) {
            return false;
        }
        //al asignar la clave al struct campo_hash_t, está haciendo un malloc. Recordar que al eliminarlo, debo liberar este espacio en memoria.
        campo_nuevo->clave = strdup(clave);
        //Verifico que se haya duplicado correctamente la clave. Sino, libero el struct campo que cree y retorno False.
        if (!campo_nuevo->clave) {
            free(campo_nuevo);
            return false;
        }
        //El valor que guardo es un puntero al dato. De esto se va a encargar de liberar el usuario con la funcion destruir_dato que haya pasado al hash.
        campo_nuevo->valor = dato;
        //Inserto el campo en la primera posicion de la lista. Esto es indistinto. Podría haber quedado en el ultimo lugar.
        bool resultado = lista_insertar_primero(hash->tabla[posicion], campo_nuevo);
        //me fijo que el resultado de la insercion haya sido exitoso. Caso contrario, retorno false despues de liberar campo_nuevo.
        if (!resultado) {
            free(campo_nuevo);
            return false;
        }
        //Luego de insertar, actualizo la cantidad de elementos del hash.
        hash->cant++;
        //verifico que luego de aumentar la cantidad, no sea necesario redimensionar.
        if (necesita_redimension(hash)) {
            //redimensiono. la funcion se encarga de aumentar o disminuir la capacidad.
            //Recordar sacar stdio.h al eliminar este comentario.
            printf("se necesita redimensionar para arriba. Comentario provisional\n");
        }

    //Si el campo existia, voy a actualizar el dato, sin tocar la clave.
    } else {
        //Si barbara me paso una funcion de destruccion, tengo que destruir el dato anterior con dicha funcion.
        if (hash->destruir_dato) {
            campo_hash_t* campo_anterior = campo_a_guardar;
            hash->destruir_dato(campo_anterior->valor);
        }
        //ahora si, reemplazo el dato anterior, con el que le pase por parametro.
        campo_a_guardar->valor = dato;
    }
    //retorno true, ya que si hubiera fallado, salia antes..
    return true;
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *hash_borrar(hash_t *hash, const char *clave) {
    //busco en qué posicion deberia borrar el valor.
    size_t posicion = fnv_hash(clave, hash->tam);
    //con la funcion auxiliar, me fijo si existe ya ese campo en el hash. Si existe, me guardo el puntero, sino, voy a recibir NULL.
    //Le paso como parámetro borrar = true. Si lo encuentra, borra la posicion de la lista y me devuelve el campo_hash.
    campo_hash_t* campo_a_borrar = hash_buscar(hash, posicion, clave, true);

    //Si encuentro el campo, deberia borrarlo, pero acá tengo un problema, ya que el iterador me devuelve el campo a borrar,
    // pero no puedo eliminar el elemento de la lista desde acá. A lo sumo puedo liberar el campo.
    //Deberia iterar y si encuentro el campo, borrarlo.
    //Para ello voy a modificar la funcion de busqueda de hash, pasando por parametro una funcion que si no es nula, borre el campo.
    if (campo_a_borrar) {
        //me guardo el dato contenido en el campo.
        void* dato_campo_borrado = campo_a_borrar->valor;

        //tengo que liberar la clave, porque ya no la voy a necesitar.
        free(campo_a_borrar->clave);
        //Ahora puedo liberar el campo_hash. El dato no, porque se lo voy a devolver al usuario.
        free(campo_a_borrar);
        //Luego de borrar, actualizo la cantidad de elementos del hash.
        hash->cant--;
        //verifico que luego de disminuir la cantidad, no sea necesario redimensionar.
        if (necesita_redimension(hash)) {
            //redimensiono. la funcion se encarga de aumentar o disminuir la capacidad.
            //Recordar sacar stdio.h al eliminar este comentario.
            printf("se necesita redimensionar para abajo. Comentario provisional\n");
        }
        //retorno el dato del campo borrado.
        return dato_campo_borrado;
    //Si el campo no existia en la lista correspondiente a la posicion donde deberia ir en el hash, retorno NULL, ya que no tengo que borrar
    }
    //Si llega hasta aca, es porque no habia dato para borrar. Entonces retorno NULL.
    return NULL;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada
 */
void *hash_obtener(const hash_t *hash, const char *clave) {
    //busco en qué posicion deberia estar el valor.
    int posicion = hash_obtener_posicion(hash, clave);
    if (posicion == -1) {
        return NULL;
    }
    //con la funcion auxiliar, me fijo si existe ya ese campo en el hash. Si existe, me guardo el puntero, sino, voy a recibir NULL.
    //Le paso como parámetro borrar = false.
    campo_hash_t* campo_buscado = hash_obtener_campo(hash, (size_t)posicion, clave);

    //Si encontró el campo buscado, guardo el dato que contiene y lo retorno.
    if (campo_buscado) {
        //me guardo el dato contenido en el campo.
        void* dato_campo_buscado = campo_buscado->valor;
        return dato_campo_buscado;
    }

    //Si no encontró el campo buscado, retorno NULL.
    return NULL;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada
 */
bool hash_pertenece(const hash_t *hash, const char *clave) {
    //Si se encuentra el campo buscado, retorna true, sino, false. Aca tengo que tener en cuenta que no me sirve recibir un null.
    //podria probar devolviendo la posicion a donde deberia estar, total el iterador de listas no deberia ser mayor a 3 o 4
    //elementos. Luego me crearia un iterador para navegar al dato y hacer lo necesario.
    return (hash_obtener_posicion(hash, clave) != -1) ? true : false;
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
    //el hash está compuesto por tres partes. hash_t -> lista_t -> campo_hash_t.
    //Tengo que liberar primero para cada lista_t, los campos. Una vez que libere todos los elementos de las listas
    //Tengo que liberar la lista. Una vez que libere la lista, tengo que liberar el hash.
    //si recorro todos los elementos de hash->tabla, y llamo a lista_destruir, pasando como parametro
    for (size_t i = 0; i < hash->tam; i++) {
        while (!lista_esta_vacia(hash->tabla[i])) {
            //voy borrando los nodos de la lista y me guardo el campo_hash_t que contiene
            campo_hash_t* campo_a_borrar = lista_borrar_primero(hash->tabla[i]);
            //si hay un campo_hash_t, destruyo la clave, el dato si corresponde y por ultimo el campo.
            if (campo_a_borrar) {
                destruir_campo(campo_a_borrar, hash->destruir_dato);
            }
        }
        //una vez que está vacia la lista, la destruyo. No necesito liberar nada, porque el campo ya fue destruido previamente.
        lista_destruir(hash->tabla[i], NULL);
    }
    //una vez que vacié todas las listas y las destruí, puedo liberar la estructura que las contenia.
    free(hash->tabla);
    //Ahora sí, puedo elminar el hash.
    free(hash);
}

/* ******************************************************************
 *                       PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

// Crea iterador
hash_iter_t *hash_iter_crear(const hash_t *hash) {
    //creo una estructura de tipo hash_iter
    hash_iter_t* iter_hash = malloc(sizeof(hash_iter_t));
    //Si no se creo correctamente, retorno NULL
    if(!iter_hash) {
        return NULL;
    }
    //me guardo un puntero al hash.
    iter_hash->hash = hash;
    //al crear el iterador, debo apuntar a la primera posicion no vacia de la tabla de hash
    size_t pos = 0;
    while(lista_esta_vacia(iter_hash->hash->tabla[pos]) && pos < iter_hash->hash->tam) {
        //voy actualizando la posicion hasta obtener una lista que nos se encuentre vacia;
        //se detiene si encuentra una lista que no esté vacia o llegó a la ultima posicion del hash.
        pos++;
    }

    //me guardo la posicion en donde me quedé. Con esta posicion voy a saber qué lista tengo que recorrer para seguir avanzando.
    iter_hash->posicion = pos;
    printf("Busque la posicion %zu del hash\n", iter_hash->posicion);

    //para la posicion en donde me encuentre, voy a crear un iterador de lista. Se va a posicionar en la primera posicion de la lista.
    //Si está vacia, va a estar al final.
    lista_iter_t* iter_lista = lista_iter_crear(iter_hash->hash->tabla[pos]);
    //si falló la creacion del iterador de lista, libero el iterador de hash y retorno NULL
    if (!iter_lista) {
        free(iter_hash);
        return NULL;
    }

    //retorno el iterador de hash.
    return iter_hash;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t *iter) {
    //Si el iterador de hash está al final, retorno false.
    if (hash_iter_al_final(iter)) {
        return false;
    }

    //intento avanzar. Si el iterador de hash no está al final, nunca deberia estar parado sobre el final de una lista.
    bool avanzar = lista_iter_avanzar(iter->iter_lista);
    //Si no pudo avanzar, retorno false.
    if (!avanzar) {
        return false;
    }

    //si luego de avanzar, la lista esta al final, tengo que crear un nuevo iterador.
    if (lista_iter_al_final(iter->iter_lista)) {
        //primero destruyo el anterior iterador de lista.
        lista_iter_destruir(iter->iter_lista);
        //si todavia puedo avanzar por el hash, avanzo a una nueva posicion.
        if (iter->posicion < iter->hash->tam - 1) {
            iter->posicion++;
        //si ya no se puede avanzar, retorno false. Aca estaria al final del hash.
        //Se cumple que lista_iter_al final y iter->posicion = hash->tam - 1
        } else {
            return false;
        }

        //Si pude avanzar a la siguiente posicion del hash, tengo que recorrer el hash hasta que encuentre una lista no vacia
        // o hasta que llegue a la ultima posicion.
        while (lista_esta_vacia(iter->hash->tabla[iter->posicion]) && iter->posicion < iter->hash->tam - 1) {
            iter->posicion++;
        }

        //para la posicion en donde me encuentre, voy a crear un iterador de lista. Se va a posicionar en la primera posicion de la lista.
        //Si está vacia, va a estar al final.
        lista_iter_t* iter_lista = lista_iter_crear(iter->hash->tabla[iter->posicion]);
        //si falló la creacion del iterador de lista, retorno false
        if (!iter_lista) {
            return false;
        }
    }
    //despues de haber avanzado, debo retornar true. Nunca deberia quedarme al final de una lista, siempre y cuando no esté al final del hash.
    return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    //Si estoy al final del iterador, retorno NULL.
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    //Sino, me creo un campo con la posicion actual del iterador de lista.
    campo_hash_t* campo_actual = lista_iter_ver_actual(iter->iter_lista);

    //Si el campo no se comlpetó por algun motivo, retorno NULL.
    if (!campo_actual) {
        return NULL;
    }
    //retorno la clave del campo que consegui.
    return campo_actual->clave;
}

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t *iter) {
    //estoy al final si se cumple que el iterador de lista está al final y la posicion del iterador coincide con la ultima posicion del hash.
    return (lista_iter_al_final(iter->iter_lista) && iter->posicion == iter->hash->tam - 1);
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter) {
    //libero el iterador de lista.
    lista_iter_destruir(iter->iter_lista);
    //libero el iterador de hash
    free(iter);
}