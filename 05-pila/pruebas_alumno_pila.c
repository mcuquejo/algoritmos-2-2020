#include "pila.h"
#include "testing.h"
#include <stddef.h>
#define CANT_ELEMENTOS 10000

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
void pruebas_pila_null() {
    pila_t* ejemplo = NULL;
    print_test("Puntero inicializado a NULL", ejemplo == NULL);
}

    // Se pueda crear y destruir correctamente la estructura.
void pruebas_crear_destruir_pila() {
    pila_t* pila = pila_crear();    
    print_test("Se crea pila", pila != NULL);

    pila_destruir(pila);
    print_test("Se destruye pila", true);
}
    // Se puedan apilar elementos, que al desapilarlos se mantenga el invariante de pila.
void pruebas_apilar_desapilar() {
    pila_t* pila = pila_crear();
    print_test("Se crea pila", pila != NULL);

    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";    

    pila_apilar(pila, &elem1);    
    print_test("Se apilo correctamente elem1 con valor 99", *(int*)pila_ver_tope(pila) == elem1);
    print_test("permite desapilar con un solo elemento", *(int*)pila_desapilar(pila) == elem1);
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);

    pila_apilar(pila, &elem2);
    print_test("Se apilo correctamente elem2 con valor \'a\'", *(char*)pila_ver_tope(pila) == elem2);

    pila_apilar(pila, &elem3);
    print_test("Se apilo correctamente elem3 con valor \'palabra\'", *(char**)pila_ver_tope(pila) == elem3);
    
    print_test("permite desapilar con dos elementos", *(char**)pila_desapilar(pila) == elem3);
    print_test("El tope vuelve a ser el elem2 con valor \'a\'", *(char*)pila_ver_tope(pila) == elem2);

    print_test("permite desapilar con un solo elemento", *(char*)pila_desapilar(pila) == elem2);

    print_test("el tope es null cuando la pila está vacia", pila_ver_tope(pila) == NULL);

    print_test("La pila esta vacia", pila_esta_vacia(pila));
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);

    pila_destruir(pila);
    print_test("Se destruye pila", true);
    
}
    // Prueba de volumen: Se pueden apilar muchos elementos (1000, 10000 elementos, o el volumen que corresponda): hacer crecer la pila hasta un valor
    //  sabido mucho mayor que el tamaño inicial, y desapilar elementos hasta que esté vacía, comprobando que siempre cumpla el invariante. 
    //  Recordar no apilar siempre el mismo puntero, 
    //  validar que se cumpla siempre que el tope de la pila sea el correcto paso a paso, 
    //  y que el nuevo tope después de cada desapilar también sea el correcto.
void pruebas_apilar_muchos_elementos(size_t cant_elem) {
    pila_t* pila = pila_crear();
    print_test("Se crea pila", pila != NULL);
    
    int elem1 = 77;
    char elem2 = 'a';

    for (size_t i = 0; i < cant_elem; i++) {
        if (i % 2 == 0) {
            pila_apilar(pila, &elem1);
        } else {
            pila_apilar(pila, &elem2);
        }                 
    }
    
    if (cant_elem % 2 == 0) {
        print_test("Los elementos se apilaron correctamente", *(char*)pila_ver_tope(pila) == elem2);    
    } else {
        print_test("Se desapilo un elemento correctamente", *(int*)pila_ver_tope(pila) == elem1);
    }
    
    
    pila_desapilar(pila);
    if (cant_elem % 2 == 0) {
        print_test("Los elementos se apilaron correctamente", *(char*)pila_ver_tope(pila) == elem1);    
    } else {
        print_test("Se desapilo un elemento correctamente", *(int*)pila_ver_tope(pila) == elem2);
    }

    for (size_t i = 0; i < cant_elem - 1; i++) {
        pila_desapilar(pila);
    }
    print_test("La pila esta vacia", pila_esta_vacia(pila));
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);
    
    /* Pruebo guardar en todas las posiciones */
    size_t i;
    bool ok = true;
    for (i = 0; i < cant_elem; i++) {
        // Si algun elemento no se pudo guardar correctamente, ok sera false
        ok &= pila_apilar(pila, &i);
    }
    print_test("se pudieron guardar todos los elementos", ok);

    /* Pruebo que lo guardado sea correcto */
    ok = true;
    int valor;
    for (i = 0; i < cant_elem; i++) {
        valor = *(int*)pila_desapilar(pila);
        ok &= (valor == i);
    }
    print_test("se pudieron obtener todos los elementos", ok);
    
    pila_destruir(pila);
    print_test("Se destruye pila", true);
}

    // El apilamiento del elemento NULL es válido.
void pruebas_apilar_null() {
    pila_t* pila = pila_crear();
    print_test("Se crea pila", pila != NULL);
    pila_apilar(pila, NULL);
    print_test("Permitio apilar un elemento de valor NULL", pila_ver_tope(pila) == NULL);
    print_test("Permitio desapilar un elemento de valor NULL", pila_desapilar(pila) == NULL);
    pila_destruir(pila);
    print_test("Se destruye pila", true);
}
    // Condición de borde: comprobar que al desapilar hasta que está vacía hace que la pila se comporte como recién creada.
    // Condición de borde: las acciones de desapilar y ver_tope en una pila a la que se le apiló y desapiló hasta estar vacía son inválidas.
void pruebas_desapilar_hasta_vaciar() {
    pila_t* pila = pila_crear();
    print_test("Se crea pila", pila != NULL);

    print_test("La pila esta vacia", pila_esta_vacia(pila));
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);
    print_test("El tope es NULL por estar vacia", pila_ver_tope(pila) == NULL);        

    int elem1 = 99;
    char elem2 = 'a';
    char* elem3 = "palabra";

    pila_apilar(pila, &elem1);
    pila_apilar(pila, &elem2);
    pila_apilar(pila, &elem3);

    print_test("La pila NO esta vacia", !pila_esta_vacia(pila));
    print_test("Permite desapilar una pila que no esta vacia", pila_desapilar(pila) != NULL);
    print_test("El tope es \'a\' por haber eliminado el tope anterior", *(char*)pila_ver_tope(pila) == elem2);        

    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);

    print_test("La pila esta nuevamente vacia", pila_esta_vacia(pila));
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);
    print_test("El tope es NULL por estar vacia", pila_ver_tope(pila) == NULL);        

    pila_destruir(pila);
    print_test("Se destruye pila", true);
}
    // Condición de borde: las acciones de desapilar y ver_tope en una pila recién creada son inválidas.
    // Condición de borde: la acción de esta_vacía en una pila recién creada es verdadero.
void pruebas_pila_vacia() {
    pila_t* pila = pila_crear();
    print_test("Se crea pila", pila != NULL);

    print_test("La pila esta vacia", pila_esta_vacia(pila));
    print_test("no permite desapilar una pila vacia", pila_desapilar(pila) == NULL);
    print_test("El tope es NULL por estar vacia", pila_ver_tope(pila) == NULL);

    pila_destruir(pila);
    print_test("Se destruye pila", true);   
}


void pruebas_pila_alumno() {
    pruebas_pila_null();
    pruebas_crear_destruir_pila();
    pruebas_apilar_desapilar();    
    pruebas_apilar_muchos_elementos(CANT_ELEMENTOS);
    pruebas_apilar_null();
    pruebas_desapilar_hasta_vaciar();
}