#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#define CANT_ELEMENTOS 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
void pruebas_lista_null() {
    lista_t* ejemplo = NULL;
    print_test("Puntero inicializado a NULL", ejemplo == NULL);
}

    // Se pueda crear y destruir correctamente la estructura.
void pruebas_crear_destruir_lista() {
    lista_t* lista = lista_crear();    
    print_test("Se crea lista", lista != NULL);

    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
}
    // Se pueda agregar elementos a la lista, y que al borrarlos se mantenga el invariante de lista.
void pruebas_enlistar_desenlistar_ultimo() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);

    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";    

    lista_insertar_ultimo(lista, &elem1);    
    print_test("Se agrego correctamente elem1 con valor 99 al final de la lista", *(int*)lista_ver_primero(lista) == elem1);
    print_test("permite borrar primer elemento con un solo elemento", *(int*)lista_borrar_primero(lista) == elem1);
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    lista_insertar_ultimo(lista, &elem2);
    print_test("Se agrego correctamente elem2 con valor \'a\' al final de la lista", *(char*)lista_ver_primero(lista) == elem2);

    lista_insertar_ultimo(lista, &elem3);
    print_test("Se agrego correctamente elem3 con valor \'palabra\' al final de la lista. El primer elemento debe seguir siendo \'a\'", *(char*)lista_ver_primero(lista) == elem2);
    
    print_test("permite borrar primer elemento con dos elementos", *(char*)lista_borrar_primero(lista) == elem2);
    print_test("El primer elemento ahora es elem3 con valor \'palabra\'", *(char**)lista_ver_primero(lista) == elem3);

    print_test("permite borrar primer elemento con un solo elemento", *(char**)lista_borrar_primero(lista) == elem3);

    print_test("el primer elemento es NULL cuando la lista está vacia", lista_ver_primero(lista) == NULL);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
    
}

void pruebas_enlistar_desenlistar_primero() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);

    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";    

    lista_insertar_primero(lista, &elem1);    
    print_test("Se agrego correctamente elem1 con valor 99 al principio de la lista", *(int*)lista_ver_primero(lista) == elem1);
    print_test("permite borrar primer elemento con un solo elemento", *(int*)lista_borrar_primero(lista) == elem1);
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    lista_insertar_primero(lista, &elem2);
    print_test("Se agrego correctamente elem2 con valor \'a\' al principio de la lista.", *(char*)lista_ver_primero(lista) == elem2);

    lista_insertar_primero(lista, &elem3);
    print_test("Se agrego correctamente elem3 con valor \'palabra\' al principio de la lista. El primer elemento ahora es \'palabra\'", *(char**)lista_ver_primero(lista) == elem3);
    
    print_test("permite borrar primer elemento con dos elementos", *(char**)lista_borrar_primero(lista) == elem3);
    print_test("El primer elemento ahora es elem3 con valor \'a\'", *(char*)lista_ver_primero(lista) == elem2);

    print_test("permite borrar primer elemento con un solo elemento", *(char*)lista_borrar_primero(lista) == elem2);

    print_test("el primer elemento es NULL cuando la lista está vacia", lista_ver_primero(lista) == NULL);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
    
}
    // Prueba de volumen: Se pueden agregar al final de la lista muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la lista hasta un valor
    //  sabido mucho mayor que el tamaño inicial, y borrar el primer elemento hasta que esté vacía, comprobando que siempre cumpla el invariante. 
    //  Recordar no enlistar siempre el mismo puntero, 
    //  validar que se cumpla siempre que el primer elemento de la lista sea el correcto paso a paso, 
    //  y que el nuevo primer elemento después de cada llamada a lista_borrar_primero() también sea el correcto.
void pruebas_enlistar_ultimo_muchos_elementos(size_t cant_elem) {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);
    
    int elem1 = 77;
    char elem2 = 'a';

    for (size_t i = 0; i < cant_elem; i++) {
        if (i % 2 == 0) {
            lista_insertar_ultimo(lista, &elem1);
        } else {
            lista_insertar_ultimo(lista, &elem2);
        }                 
    }
    
    print_test("Los elementos se agregaron correctamente al final de la lista", *(int*)lista_ver_primero(lista) == elem1);    
    
    lista_borrar_primero(lista);
    print_test("Se elimino primer elemento de la lista correctamente", *(char*)lista_ver_primero(lista) == elem2);    

    for (size_t i = 0; i < cant_elem - 1; i++) {
        lista_borrar_primero(lista);
    }
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);
    
    // Pruebo que inserte correctamente cada elemento al final de la lista
    size_t i;
    bool ok = true;
    for (i = 0; i < cant_elem; i++) {
        // Si algun elemento no se pudo agregar correctamente al final de la lista, ok sera false
        ok &= lista_insertar_ultimo(lista, &i);
    }
    print_test("se pudieron insertar todos los elementos al final de la lista", ok);

    /* Pruebo que lo guardado sea correcto */
    ok = true;
    int valor;
    for (i = 0; i < cant_elem; i++) {
        valor = *(int*)lista_borrar_primero(lista);
        ok &= (valor == i);
    }
    print_test("se pudieron obtener todos los elementos de la lista", ok);
    
    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
}

void pruebas_enlistar_primero_muchos_elementos(size_t cant_elem) {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);
    
    int elem1 = 77;
    char elem2 = 'a';

    for (size_t i = 0; i < cant_elem; i++) {
        if (i % 2 == 0) {
            lista_insertar_primero(lista, &elem1);
        } else {
            lista_insertar_primero(lista, &elem2);
        }                 
    }
    
    print_test("Los elementos se agregaron correctamente al inicio de la lista", *(char*)lista_ver_primero(lista) == elem2);    
    
    lista_borrar_primero(lista);
    print_test("Se elimino primer elemento de la lista correctamente", *(int*)lista_ver_primero(lista) == elem1);    

    for (size_t i = 0; i < cant_elem - 1; i++) {
        lista_borrar_primero(lista);
    }
    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);
    
    // Pruebo que inserte correctamente cada elemento al inicio de la lista
    size_t i;
    bool ok = true;
    for (i = 0; i < cant_elem; i++) {
        // Si algun elemento no se pudo agregar correctamente al inicio de la lista, ok sera false
        ok &= lista_insertar_primero(lista, &i);
    }
    print_test("se pudieron insertar todos los elementos al inicio de la lista", ok);

    /* Pruebo que lo guardado sea correcto */
    ok = true;
    int valor;
    for (i = 0; i < cant_elem; i++) {
        valor = *(int*)lista_borrar_primero(lista);
        ok &= (valor == i);
    }
    print_test("se pudieron obtener todos los elementos de la lista", ok);
    
    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
}

    // Insertar elemento NULL al final de la lista es válido.
void pruebas_enlistar_null() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);
    lista_insertar_ultimo(lista, NULL);
    print_test("Permitio agregar un elemento de valor NULL al final de la lista", lista_ver_primero(lista) == NULL);
    print_test("Permitio borrar primer elemento de valor NULL de la lista", lista_borrar_primero(lista) == NULL);
    lista_insertar_primero(lista, NULL);
    print_test("Permitio agregar un elemento de valor NULL al inicio de la lista", lista_ver_primero(lista) == NULL);
    print_test("Permitio borrar primer elemento de valor NULL de la lista", lista_borrar_primero(lista) == NULL);
    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
}
    // Condición de borde: comprobar que al borrar primer elemento hasta que está vacía hace que la lista se comporte como recién creada.
    // Condición de borde: las acciones de borrar borrar_primero y ver_primero en una lista a la que se le agrego y borro elementos hasta estar vacía son inválidas.
void pruebas_desenlistar_hasta_vaciar() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);
    print_test("El primer elemento es NULL por estar vacia", lista_ver_primero(lista) == NULL);        

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";

    lista_insertar_ultimo(lista, &elem1);
    lista_insertar_ultimo(lista, &elem2);
    lista_insertar_ultimo(lista, &elem3);

    print_test("La lista NO esta vacia", !lista_esta_vacia(lista));
    print_test("Permite borrar primer elemento una lista que no esta vacia", lista_borrar_primero(lista) != NULL);
    print_test("El primer elemento es \'a\' por haber borrado el elemento anterior", *(char*)lista_ver_primero(lista) == elem2);        

    lista_borrar_primero(lista);
    lista_borrar_primero(lista);
    lista_borrar_primero(lista);

    print_test("La lista nuevamente esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);
    print_test("El primer elemento es NULL por estar vacia", lista_ver_primero(lista) == NULL);               

    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);
}
    // Condición de borde: las acciones de desenlistar y ver_primero en una lista recién creada son inválidas.
    // Condición de borde: la acción de esta_vacía en una lista recién creada es verdadero.
void pruebas_lista_vacia() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);

    print_test("La lista esta vacia", lista_esta_vacia(lista));
    print_test("no permite borrar primer elemento en  una lista vacia", lista_borrar_primero(lista) == NULL);
    print_test("El primer elemento es NULL por estar vacia", lista_ver_primero(lista) == NULL);

    lista_destruir(lista, NULL);
    print_test("Se destruye lista", true);   
}

void inner_lista_destruir(void* lista) {
    lista_destruir(lista, NULL);
}


void pruebas_lista_dato_dinamica() {
    lista_t* lista = lista_crear();
    print_test("Se crea lista", lista != NULL);

    char* elem1 = malloc(sizeof(char)  * 10);
    elem1[0] = 'p';
    elem1[1] = 'a';
    elem1[2] = 'l';
    elem1[3] = 'a';
    elem1[4] = 'b';
    elem1[5] = 'r';
    elem1[6] = 'a';
    elem1[7] = '\0';
    
    
    print_test("Permitio agregar elemento en memoria dinamica de valor \'palabra\' al final de la lista correctamente", lista_insertar_ultimo(lista, elem1));
    print_test("El primer elemento es \'palabra\'.", lista_ver_primero(lista) == elem1);

    char** elem2 = malloc(sizeof(char)  * 10);
    *elem2 = "palabra2";        
    
    print_test("Permitio agregar elemento en memoria dinamica de valor \'palabra2\' al final de la lista correctamente.", lista_insertar_ultimo(lista, elem2));

    print_test("El primer elemento sigue siendo \'palabra\'.", lista_ver_primero(lista) == elem1);

    print_test("Permitio borrar primer elemento en memoria dinamica de valor \'palabra\' de la lista correctamente", lista_borrar_primero(lista) == elem1);
    
    print_test("El primer elemento ahora es \'palabra2\'.", lista_ver_primero(lista) == elem2);
    

    lista_destruir(lista, free);
    print_test("permitio destruir la lista con un elemento dinamico de valor \'palabra2\'.", true);
    free(elem1);
    print_test("Se libera el elemento restante que fue borrado previamente de valor \'palabra\'", true);

    lista_t* lista2 = lista_crear();
    print_test("Se crea lista 2", lista2 != NULL);
    lista_t* inner_lista = lista_crear();
    print_test("Se crea inner lista, que se agregara al final en lista 2", inner_lista != NULL);

    int valor1 = 2;

    print_test("Se agregan elementos al final en la inner lista", lista_insertar_ultimo(inner_lista, &valor1));
    print_test("Se agregan elementos al final en la inner lista", lista_insertar_ultimo(inner_lista, &valor1));
    print_test("Se agregan elementos al final en la inner lista", lista_insertar_ultimo(inner_lista, &valor1));

    print_test("Se agrega inner lista al final en la lista 2", lista_insertar_ultimo(lista2, inner_lista));

    lista_destruir(lista2, inner_lista_destruir);
    print_test("permitio destruir la lista 2 llamando una funcion de destrucion para inner lista que estaba en memoria dinamica.", true);
}


void pruebas_lista_alumno() {
    pruebas_lista_null();
    pruebas_crear_destruir_lista();
    pruebas_enlistar_desenlistar_primero();
    pruebas_enlistar_desenlistar_ultimo();
    pruebas_enlistar_primero_muchos_elementos(CANT_ELEMENTOS);
    pruebas_enlistar_ultimo_muchos_elementos(CANT_ELEMENTOS);
    pruebas_enlistar_null();
    pruebas_desenlistar_hasta_vaciar();
    pruebas_lista_dato_dinamica();
}