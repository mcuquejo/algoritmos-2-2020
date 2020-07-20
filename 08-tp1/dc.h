#include <stdbool.h>
#include "pila.h"

bool es_numero(char* cadena);
void free_pila(pila_t* pila_operaciones);
char* formatear_operacion(char* operacion);
int calculadora_polaca();