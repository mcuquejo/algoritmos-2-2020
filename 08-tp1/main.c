#include "strutil.h"
#include "dc.h"
#include "testing.h"
#include <stdio.h>
#include "infix.h"

extern void pruebas_strutil_alumno(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_cola_catedra();

int main(void) {
    /* Ejecuta todas las pruebas unitarias. */
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    //  pruebas_strutil_alumno();
    // calculadora_polaca();
    infix();

    return failure_count() > 0;
}
