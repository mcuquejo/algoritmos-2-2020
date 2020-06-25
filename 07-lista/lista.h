#include <stdbool.h>
#include <stdlib.h>

/* La lista está planteada como una lista de punteros genéricos. */
typedef struct lista lista_t;

/* El iterador está planteado como una estructura que contiene punteros a la lista, al elemento actual y al anterior de la lista. */
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento a la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);


// Devuelve la cantidad de elementos contenidos en la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR EXTERNO
 * *****************************************************************/
// Crea una iterador. Si falla la creacion debe devolver NULL. Caso contrario, 
// devuelve un nuevo iterador apuntando al primer elemento de la lista pasada como parametro.
// Pre: la lista fue creada.
// Post: devuelve un nuevo iterador apuntando al primer elemento de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// El iterador avanza al siguiente elemento de la lista. Si la lista esta 
// vacia o el iterador se encuentra al final de la lista, devuelve falso.
// Pre: la lista fue creada. El iterador fue creado.
// Post: Se actualizo la posicion del iterador al siguiente elemento de la lista, 
// o se devolvio false en caso de error.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve un puntero apuntando al dato contenido en la posicion actual del iterador.
// Si la lista esta vacia, o si el iterador se encuentra al final de la lista, devuelve NULL.
// Pre: la lista fue creada. El iterador fue creado.
// Post: Se devolvio el puntero al dato de la posicion actual del iterador o NULL si
//la lista esta vacia o el iterador esta al final de la lista.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero si el iterador se encuentra fuera de la lista. Esto ocurre cuando
// El elemento anterior al que apunta el iterador es el ultimo elemento de la lista y el
// elemento al que apunta el iterador es NULL. Devuelve false en caso contrario.
// Pre: la lista fue creada. El iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: la lista fue creada. El iterador fue creado.
// Post: se elimino el iterador y se libero la memoria correspondiente.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta un elemento entre la posicion anterior a la que apunta el iterador y la posicion
// actual. Debe actualizar las referencias de primer elemento de la lista Si el iterador se 
// encuentra al principio de la lista, y/o del ultimo elemento, si el iterador se encuentra al
// final de la lista.Caso contrario, devuelve falso.
// Pre: la lista fue creada. El iterador fue creado.
// Post: se inserto un elemento en la posicion actual en la que se encuentra el iterador. Caso contrario,
// devuelve falso.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina un elemento entre la posicion anterior a la que apunta el iterador y la posicion
// actual. Debe actualizar las referencias de primer elemento de la lista Si el iterador se 
// encuentra al principio de la lista, o si al eliminar el elemento, quedase un unico elemento en la lista
// y/o debe actualizar las referencias al ultimo elemento, si el iterador se encuentra al
// final de la lista, o si al eliminar el elemento, quedase un unico elemento en la lista.
// Debe retornar el dato contenido en la posicion eliminada, o NULL si fallo la eliminacion
// Pre: la lista fue creada. El iterador fue creado.
// Post: Se devolvio el dato contenido en la posicion a eliminar y se borro el nodo de la lista,liberando
// la memoria correspondiente. Caso contrario, devolvio NULL.
void *lista_iter_borrar(lista_iter_t *iter);

/* ******************************************************************
 *                    PRIMITIVAS DE ITERADOR INTERNO
 * *****************************************************************/
//Recibe un puntero a funcion, el cual define que instrucciones se ejecutaran para cada dato.
//Si se pasa NULL en el parametro extra, se recorrera la lista hasta el final. Si se pasa
//un puntero en el parametro extra, se ejecutara la funcion visitar en cada dato, hasta que
//se acabe la lista, o no se cumpla la condicion especificada utilizando el valor extra como
// corte.
// Pre: la lista fue creada. El iterador fue creado.
// Post: Se recorrio la lista hasta la posicion indicada a partir de la funcion visitar y el parametro
// extra y se ejecutaron las instrucciones correspondientes sobre cada dato.
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);