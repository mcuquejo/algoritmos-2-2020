#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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
    // Se pueda agregar elementos a la lista, y que al borrarlos se mantenga el invariante de lista. (prueba con lista_insertar_ultimo)
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

// Se pueda agregar elementos a la lista, y que al borrarlos se mantenga el invariante de lista. (prueba con lista_insertar_primero)
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

// Prueba de volumen: Se pueden agregar al principio de la lista muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la lista hasta un valor
    //  sabido mucho mayor que el tamaño inicial, y borrar el primer elemento hasta que esté vacía, comprobando que siempre cumpla el invariante. 
    //  Recordar no enlistar siempre el mismo puntero, 
    //  validar que se cumpla siempre que el primer elemento de la lista sea el correcto paso a paso, 
    //  y que el nuevo primer elemento después de cada llamada a lista_borrar_primero() también sea el correcto.
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

    // Insertar elemento NULL tanto al principio como al final de la lista es válido.
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

//pruebas sobre una lista con datos en stack, borrados con funcion inner_lista_destruir
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

lista_t* crear_lista_pruebas(){
    lista_t* lista = lista_crear();
    int* elem1 = malloc(sizeof(int));
    *elem1 = 99;
    char* elem2 = malloc(sizeof(char));
    *elem2 = 'a';
    char** elem3 = malloc(sizeof(char*) * 10);
    *elem3 = "palabra\0";
    int* elem4 = malloc(sizeof(int));
    *elem4 = 105;
    lista_insertar_primero(lista, elem1);
    print_test("el primer elemento es: 99", *(int*)lista_ver_primero(lista) == 99);    
    print_test("el ultimo elemento es: 99", *(int*)lista_ver_ultimo(lista) == 99);
    lista_insertar_ultimo(lista, elem2);
    print_test("el primer elemento es: 99", *(int*)lista_ver_primero(lista) == 99);
    print_test("el ultimo elemento es: \'a\'", *(char*)lista_ver_ultimo(lista) == 'a');
    lista_insertar_primero(lista, elem3);
    print_test("el primer elemento es: \'palabra\'", strcmp(*(char**)lista_ver_primero(lista), "palabra") == 0);
    print_test("el ultimo elemento es: \'a\'", *(char*)lista_ver_ultimo(lista) == 'a');
    lista_insertar_ultimo(lista, elem4);
    print_test("el primer elemento es: \'palabra\'", strcmp(*(char**)lista_ver_primero(lista), "palabra") == 0);
    print_test("el ultimo elemento es: 105", *(int*)lista_ver_ultimo(lista) == 105);
    print_test("Se creo una lista con la siguiente estructura: [\'palabra\', 99, \'a\', 105]", true);    
    return lista;
}

//pruebas iterador externo con una lista creada insertando valores de distintos tipos y con funciones de insertar al principio y al final
void pruebas_iterador_externo(){
    lista_t* lista = crear_lista_pruebas();
    lista_iter_t* iter_externo = lista_iter_crear(lista);
    char* elemento = *(char**)lista_ver_primero(lista);
    char* elemento_desde_iter = *(char**)lista_iter_ver_actual(iter_externo);
    print_test("Cuando se crea el iterador, el primer valor deberia ser \'palabra\' (visto con lista_ver_primero())", strcmp(elemento, "palabra") == 0);
    print_test("Cuando se crea el iterador, el primer valor deberia ser \'palabra\' (visto con lista_iter_ver_actual())", strcmp(elemento_desde_iter, "palabra") == 0);
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo", lista_iter_avanzar(iter_externo));
    print_test("Al avanzar con el iterador, el elemento debe tener valor 99 (visto con lista_iter_ver_actual())", *(int*)lista_iter_ver_actual(iter_externo) == 99);
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo", lista_iter_avanzar(iter_externo));
    print_test("Al avanzar con el iterador, el elemento debe tener valor \'a\' (visto con lista_iter_ver_actual())", *(char*)lista_iter_ver_actual(iter_externo) == 'a');
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo", lista_iter_avanzar(iter_externo));
    print_test("Al avanzar con el iterador, el elemento debe tener valor 99 (visto con lista_iter_ver_actual())", *(int*)lista_iter_ver_actual(iter_externo) == 105);
    print_test("Debe permitir avanzar con el iterador hasta el final de la lista", lista_iter_avanzar(iter_externo));
    print_test("El iterador se encuentra al final de la lista", lista_iter_al_final(iter_externo));
    print_test("No debe permitir avanzar al iterador estando al final de la lista", !lista_iter_avanzar(iter_externo));
    print_test("El elemento actual cuando la lista esta al final debe ser NULL", !lista_iter_ver_actual(iter_externo));
    int* elem5 = malloc(sizeof(int));
    *elem5 = 106;
    lista_iter_insertar(iter_externo, elem5);
    print_test("El elemento actual Ahora es 106", *(int*)lista_iter_ver_actual(iter_externo) == 106);
    print_test("Debe permitir avanzar con el iterador hasta el final de la lista", lista_iter_avanzar(iter_externo));
    print_test("El iterador se encuentra al final de la lista", lista_iter_al_final(iter_externo));
    print_test("No debe permitir avanzar al iterador estando al final de la lista", !lista_iter_avanzar(iter_externo));
    print_test("El elemento actual cuando la lista esta al final debe ser NULL", !lista_iter_ver_actual(iter_externo));
    print_test("No debe permitir avanzar al iterador estando al final de la lista", !lista_iter_avanzar(iter_externo));
    print_test("El elemento actual cuando la lista esta al final debe ser NULL", !lista_iter_ver_actual(iter_externo));
    int* elem6 = malloc(sizeof(int));
    *elem6 = 107;
    lista_iter_insertar(iter_externo, elem6);
    print_test("El iterador ya no se encuentra al final de la lista", !lista_iter_al_final(iter_externo));
    print_test("El elemento actual Ahora es 107", *(int*)lista_iter_ver_actual(iter_externo) == 107);
    print_test("Permite borrar correctamente el elemento de la lista de valor 107", *(int*)lista_iter_borrar(iter_externo) == 107);
    print_test("El iterador se encuentra nuevamente al final de la lista", lista_iter_al_final(iter_externo));
    print_test("El elemento actual Ahora es NULL", lista_iter_ver_actual(iter_externo) == NULL);
    lista_iter_destruir(iter_externo);
    lista_iter_t* iter_externo2 = lista_iter_crear(lista);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_avanzar(iter_externo2);
    lista_iter_destruir(iter_externo2);
    free(elem6);    
    lista_destruir(lista, free);
}

//pruebas utilizando dos iteradores externos sobre la misma lista.
void pruebas_dos_iteradores_externos() {
    lista_t* lista = crear_lista_pruebas();

    lista_iter_t* iter_externo3 = lista_iter_crear(lista);
    char* elemento1_desde_lista = *(char**)lista_ver_primero(lista);
    print_test("El primer valor de la lista es \'palabra\' (visto con lista_ver_primero())", strcmp(elemento1_desde_lista, "palabra") == 0);
    print_test("El ultimo valor de la lista deberia ser 105 (visto con lista_ver_ultimo())", *(int*)lista_ver_ultimo(lista) == 105);
    char* elemento1_desde_iter3 = *(char**)lista_iter_ver_actual(iter_externo3);
    print_test("Cuando se crea el iterador, el primer valor deberia ser \'palabra\' (visto con lista_ver_primero())", strcmp(elemento1_desde_iter3, "palabra") == 0);
    
    char** elem_a_liberar_1 = (char**)lista_iter_borrar(iter_externo3);
    print_test("Se elimina el elemento actual de iter 3. El valor eliminado deberia ser \'palabra\'", strcmp(*elem_a_liberar_1, "palabra") == 0);
    lista_iter_t* iter_externo4 = lista_iter_crear(lista);
    int elemento2_desde_iter3 = *(int*)lista_iter_ver_actual(iter_externo3);
    print_test("El iterador 3 deberia estar apuntando al valor 99", elemento2_desde_iter3 == 99);    
    int elemento2_desde_iter4 = *(int*)lista_iter_ver_actual(iter_externo4);
    print_test("Cuando se crea el iterador 4, el valor al que apunta, deberia ser 99", elemento2_desde_iter4 == 99);
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo 3", lista_iter_avanzar(iter_externo3));
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo 4", lista_iter_avanzar(iter_externo4));    
    int elemento3_desde_iter3 = *(char*)lista_iter_ver_actual(iter_externo3);
    print_test("El iterador 3 deberia estar apuntando al valor 99", elemento3_desde_iter3 == 'a');    
    int elemento3_desde_iter4 = *(char*)lista_iter_ver_actual(iter_externo4);
    print_test("Cuando se crea el iterador 4, el valor al que apunta, deberia ser 99", elemento3_desde_iter4 == 'a');
    int elemento3_desde_lista = *(int*)lista_ver_primero(lista);
    print_test("El primer valor de la lista es 99 (visto con lista_ver_primero())", elemento3_desde_lista == 99);
    print_test("El ultimo valor de la lista sigue siendo 105 (visto con lista_ver_ultimo())", *(int*)lista_ver_ultimo(lista) == 105);

    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo 3", lista_iter_avanzar(iter_externo3));
    int* elem_a_liberar_2 = (int*)lista_iter_borrar(iter_externo3);
    //desde iterador
    print_test("Se elimina el elemento actual de iter 3. El valor eliminado deberia ser 105", *elem_a_liberar_2 == 105);
    print_test("El iterador 3 se encuentra al final de la lista", lista_iter_al_final(iter_externo3));    
    char* elemento4_desde_iter4 = (char*)lista_iter_ver_actual(iter_externo4);    
    print_test("El iterador 4 deberia estar apuntando al valor \'a\'", *elemento4_desde_iter4 == 'a');

    //desde lista    
    print_test("El primer valor de la lista es 99 (visto con lista_ver_primero())", *(int*)lista_ver_primero(lista) == 99);
    print_test("El ultimo valor de la lista ahora es \'a\' (visto con lista_ver_ultimo())", *(char*)lista_ver_ultimo(lista) == 'a');
    
    print_test("Permitio avanzar correctamente al siguiente elemento con el iterador externo 4", lista_iter_avanzar(iter_externo4));
    print_test("El iterador 4 se encuentra al final de la lista", lista_iter_al_final(iter_externo4));    
    print_test("El primer valor de la lista es 99 (visto con lista_ver_primero())", *(int*)lista_ver_primero(lista) == 99);
    print_test("El ultimo valor de la lista ahora es \'a\' (visto con lista_ver_ultimo())", *(char*)lista_ver_ultimo(lista) == 'a');
    print_test("El iterador 3 deberia estar apuntando NULL", !lista_iter_ver_actual(iter_externo3));
    print_test("El iterador 4 deberia estar apuntando NULL", !lista_iter_ver_actual(iter_externo4));
    
    free(elem_a_liberar_1);
    free(elem_a_liberar_2);
    
    lista_iter_destruir(iter_externo3);
    lista_iter_destruir(iter_externo4);
    lista_destruir(lista, free);
}

bool imprimir_todo(void* dato, void* extra) {
    print_test("resultado exitoso al imprimir todos los elementos: ", dato);
    return true;
} 

bool imprimir_algunos_elementos(void* dato, void* extra) { 
    if (*(size_t*) extra >= 5) { 
        return false; 
    }
    size_t *contador = extra;
    (*contador)++;
    print_test("resultado exitoso al imprimir algunos elementos:", dato);
    return true;
} 

//pruebas iterador interno que recorren la lista tanto de forma completa como parcial
void pruebas_iterador_interno() {
    size_t cant_lista = 10;
    lista_t* lista = lista_crear();
    print_test("Lista: Creada correctamente", lista);    
    int vector[cant_lista];
    bool resultado_exitoso = true;    
    for (int i = 0; i < cant_lista; i++) {
        vector[i] = i;
        resultado_exitoso &= lista_insertar_primero(lista, &vector[i]);
    }    
    lista_iterar(lista, imprimir_todo, NULL);    
    size_t contador = 0;
    lista_iterar(lista, imprimir_algunos_elementos, &contador);    
    lista_destruir(lista, NULL);
}

void pruebas_lista_alumno() {
    pruebas_iterador_externo();
    pruebas_dos_iteradores_externos();
    pruebas_iterador_interno();
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