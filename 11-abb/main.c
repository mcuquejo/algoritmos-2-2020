#include "testing.h"
#include <stdio.h>

void pruebas_abb_alumno(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

#ifndef CORRECTOR
int main(int argc, char *argv[])
{
    printf("~~~ PRUEBAS ALUMNO ~~~\n");
    pruebas_abb_alumno();

    return failure_count() > 0;
}
#endif
