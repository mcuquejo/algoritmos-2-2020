#include "cola.h"
#include "testing.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#define CANT_ELEMENTOS 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
void pruebas_cola_null() {
    cola_t* ejemplo = NULL;
    print_test("Puntero inicializado a NULL", ejemplo == NULL);
}

    // Se pueda crear y destruir correctamente la estructura.
void pruebas_crear_destruir_cola() {
    cola_t* cola = cola_crear();    
    print_test("Se crea cola", cola != NULL);

    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);
}
    // Se puedan encolar elementos, que al desencolarlos se mantenga el invariante de cola.
void pruebas_encolar_desencolar() {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);

    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";    

    cola_encolar(cola, &elem1);    
    print_test("Se encolo correctamente elem1 con valor 99", *(int*)cola_ver_primero(cola) == elem1);
    print_test("permite desencolar con un solo elemento", *(int*)cola_desencolar(cola) == elem1);
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);

    cola_encolar(cola, &elem2);
    print_test("Se encolo correctamente elem2 con valor \'a\'", *(char*)cola_ver_primero(cola) == elem2);

    cola_encolar(cola, &elem3);
    print_test("Se encolo correctamente elem3 con valor \'palabra\'. El primer elemento debe seguir siendo \'a\'", *(char*)cola_ver_primero(cola) == elem2);
    
    print_test("permite desencolar con dos elementos", *(char*)cola_desencolar(cola) == elem2);
    print_test("El primer elemento ahora es elem3 con valor \'palabra\'", *(char**)cola_ver_primero(cola) == elem3);

    print_test("permite desencolar con un solo elemento", *(char**)cola_desencolar(cola) == elem3);

    print_test("el primer elemento es null cuando la cola está vacia", cola_ver_primero(cola) == NULL);

    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);
    
}
    // Prueba de volumen: Se pueden encolar muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la cola hasta un valor
    //  sabido mucho mayor que el tamaño inicial, y desencolar elementos hasta que esté vacía, comprobando que siempre cumpla el invariante. 
    //  Recordar no encolar siempre el mismo puntero, 
    //  validar que se cumpla siempre que el primer elemento de la cola sea el correcto paso a paso, 
    //  y que el nuevo primer elemento después de cada desencolar también sea el correcto.
void pruebas_encolar_muchos_elementos(size_t cant_elem) {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);
    
    int elem1 = 77;
    char elem2 = 'a';

    for (size_t i = 0; i < cant_elem; i++) {
        if (i % 2 == 0) {
            cola_encolar(cola, &elem1);
        } else {
            cola_encolar(cola, &elem2);
        }                 
    }
    
    print_test("Los elementos se encolaron correctamente", *(int*)cola_ver_primero(cola) == elem1);    
    
    cola_desencolar(cola);
    print_test("Se desencolo un elemento correctamente", *(char*)cola_ver_primero(cola) == elem2);    

    for (size_t i = 0; i < cant_elem - 1; i++) {
        cola_desencolar(cola);
    }
    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);
    
    /* Pruebo que encole correctamente cada elemento */
    size_t i;
    bool ok = true;
    for (i = 0; i < cant_elem; i++) {
        // Si algun elemento no se pudo encolar correctamente, ok sera false
        ok &= cola_encolar(cola, &i);
    }
    print_test("se pudieron encolar todos los elementos", ok);

    /* Pruebo que lo guardado sea correcto */
    ok = true;
    int valor;
    for (i = 0; i < cant_elem; i++) {
        valor = *(int*)cola_desencolar(cola);
        ok &= (valor == i);
    }
    print_test("se pudieron obtener todos los elementos", ok);
    
    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);
}

    // El apilamiento del elemento NULL es válido.
void pruebas_encolar_null() {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);
    cola_encolar(cola, NULL);
    print_test("Permitio encolar un elemento de valor NULL", cola_ver_primero(cola) == NULL);
    print_test("Permitio desencolar un elemento de valor NULL", cola_desencolar(cola) == NULL);
    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);
}
    // Condición de borde: comprobar que al desencolar hasta que está vacía hace que la cola se comporte como recién creada.
    // Condición de borde: las acciones de desencolar y ver_primero en una cola a la que se le encolo y desencolo hasta estar vacía son inválidas.
void pruebas_desencolar_hasta_vaciar() {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);

    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);
    print_test("El primer elemento es NULL por estar vacia", cola_ver_primero(cola) == NULL);        

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";

    cola_encolar(cola, &elem1);
    cola_encolar(cola, &elem2);
    cola_encolar(cola, &elem3);

    print_test("La cola NO esta vacia", !cola_esta_vacia(cola));
    print_test("Permite desencolar una cola que no esta vacia", cola_desencolar(cola) != NULL);
    print_test("El primer elemento es \'a\' por haber desencolado el elemento anterior", *(char*)cola_ver_primero(cola) == elem2);        

    cola_desencolar(cola);
    cola_desencolar(cola);
    cola_desencolar(cola);

    print_test("La cola nuevamente esta vacia", cola_esta_vacia(cola));
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);
    print_test("El primer elemento es NULL por estar vacia", cola_ver_primero(cola) == NULL);               

    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);
}
    // Condición de borde: las acciones de desencolar y ver_primero en una cola recién creada son inválidas.
    // Condición de borde: la acción de esta_vacía en una cola recién creada es verdadero.
void pruebas_cola_vacia() {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);

    print_test("La cola esta vacia", cola_esta_vacia(cola));
    print_test("no permite desencolar una cola vacia", cola_desencolar(cola) == NULL);
    print_test("El primer elemento es NULL por estar vacia", cola_ver_primero(cola) == NULL);

    cola_destruir(cola, NULL);
    print_test("Se destruye cola", true);   
}

void inner_cola_destruir(void* cola) {
    cola_destruir(cola, NULL);
}


void pruebas_cola_dato_dinamica() {
    cola_t* cola = cola_crear();
    print_test("Se crea cola", cola != NULL);

    char* elem1 = malloc(sizeof(char)  * 10);
    elem1[0] = 'p';
    elem1[1] = 'a';
    elem1[2] = 'l';
    elem1[3] = 'a';
    elem1[4] = 'b';
    elem1[5] = 'r';
    elem1[6] = 'a';
    elem1[7] = '\0';
    
    
    print_test("Permitio encolar elemento en memoria dinamica de valor \'palabra\' correctamente", cola_encolar(cola, elem1));
    print_test("El primer elemento es \'palabra\'.", cola_ver_primero(cola) == elem1);

    char** elem2 = malloc(sizeof(char)  * 10);
    *elem2 = "palabra2";        
    
    print_test("Permitio encolar elemento en memoria dinamica de valor \'palabra2\' correctamente.", cola_encolar(cola, elem2));

    print_test("El primer elemento sigue siendo \'palabra\'.", cola_ver_primero(cola) == elem1);

    print_test("Permitio desencolar elemento en memoria dinamica de valor \'palabra\' correctamente", cola_desencolar(cola) == elem1);
    
    print_test("El primer elemento ahora es \'palabra2\'.", cola_ver_primero(cola) == elem2);
    

    cola_destruir(cola, free);
    print_test("permitio destruir la cola con un elemento dinamico encolado de valor \'palabra2\'.", true);
    free(elem1);
    print_test("Se libera el elemnto restante que fue desencolado previamente de valor \'palabra\'", true);

    cola_t* cola2 = cola_crear();
    print_test("Se crea cola 2", cola2 != NULL);
    cola_t* inner_cola = cola_crear();
    print_test("Se crea inner cola, que se encolara en cola 2", inner_cola != NULL);

    int valor1 = 2;

    print_test("Se encolan elementos en la inner cola", cola_encolar(inner_cola, &valor1));
    print_test("Se encolan elementos en la inner cola", cola_encolar(inner_cola, &valor1));
    print_test("Se encolan elementos en la inner cola", cola_encolar(inner_cola, &valor1));

    print_test("Se encolan inner cola en la cola 2", cola_encolar(cola2, inner_cola));

    cola_destruir(cola2, inner_cola_destruir);
    print_test("permitio destruir la cola 2 llamando una funcion de destrucion para inner cola que estaba en memoria dinamica.", true);

    

}


void pruebas_cola_alumno() {
    pruebas_cola_null();
    pruebas_crear_destruir_cola();
    pruebas_encolar_desencolar();    
    pruebas_encolar_muchos_elementos(CANT_ELEMENTOS);
    pruebas_encolar_null();
    pruebas_desencolar_hasta_vaciar();
    pruebas_cola_dato_dinamica();
}