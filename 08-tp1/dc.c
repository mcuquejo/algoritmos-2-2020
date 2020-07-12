#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "strutil.h"
#include <string.h>
#define SIZE_OF_LONG 20

bool suma(pila_t* pila_operaciones){    
    char* b = pila_desapilar(pila_operaciones);    
    char* a = pila_desapilar(pila_operaciones);    
    char* b_copia = calloc(SIZE_OF_LONG, sizeof(char));   
    printf("longitud de b_copia = %zu\n", strlen(b_copia));
    char* a_copia = calloc(SIZE_OF_LONG, sizeof(char));
    printf("longitud de a_copia = %zu\n", strlen(a_copia));
    int* suma = malloc(sizeof(int));
    if (!a || !b || !a_copia || !b_copia || !suma) {
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(suma);
        return false;
    }
    // strcpy(b_copia, b);
    sprintf(b_copia, "%d", *b);
    printf("longitud de b_copia = %zu\n", strlen(b_copia));
    free(b);        
    // strcpy(a_copia, a);
    sprintf(a_copia, "%d", *a);
    printf("longitud de a_copia = %zu\n", strlen(a_copia));
    printf("longitud de a = %zu\n", strlen(a));
    printf("a = %s\n", a);
    printf("a = %i\n", *a);
    free(a);    
    *suma = (int)(*(long*)a_copia + *(long*)b_copia);
    printf("resultado suma = %i\n", *suma);
    pila_apilar(pila_operaciones, suma);
    free(a_copia);
    free(b_copia);
    return true;
}

bool resta(pila_t* pila_operaciones){    
    char* b = pila_desapilar(pila_operaciones);    
    char* a = pila_desapilar(pila_operaciones);    
    char* b_copia = calloc(SIZE_OF_LONG, sizeof(char));
    char* a_copia = calloc(SIZE_OF_LONG, sizeof(char));
    int* resta = malloc(sizeof(int));
    if (!a || !b || !a_copia || !b_copia || !resta) {
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(resta);
        return false;
    }
    strcpy(b_copia, b);
    free(b);    
    strcpy(a_copia, a);
    free(a);    
    *resta = (int)(*(long*)a_copia - *(long*)b_copia);
    pila_apilar(pila_operaciones, resta);
    free(a_copia);
    free(b_copia);
    return true;
}

bool multiplicacion(pila_t* pila_operaciones){    
    char* b = pila_desapilar(pila_operaciones);    
    char* a = pila_desapilar(pila_operaciones);    
    char* b_copia = calloc(SIZE_OF_LONG, sizeof(char));
    char* a_copia = calloc(SIZE_OF_LONG, sizeof(char));
    int* multiplicacion = malloc(sizeof(int));
    if (!a || !b || !a_copia || !b_copia || !multiplicacion) {
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(multiplicacion);
        return false;
    }
    strcpy(b_copia, b);
    free(b);    
    strcpy(a_copia, a);
    free(a);    
    *multiplicacion = (int)(*(long*)a_copia * *(long*)b_copia);
    pila_apilar(pila_operaciones, multiplicacion);
    free(a_copia);
    free(b_copia);
    return true;
}

bool division(pila_t* pila_operaciones){    
    char* b = pila_desapilar(pila_operaciones);    
    char* a = pila_desapilar(pila_operaciones);    
    char* b_copia = calloc(SIZE_OF_LONG, sizeof(char));
    char* a_copia = calloc(SIZE_OF_LONG, sizeof(char));
    int* division = malloc(sizeof(int));
    if (!a || !b || !a_copia || !b_copia || !division) {
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(division);
        return false;
    }
    strcpy(b_copia, b);
    free(b);    
    strcpy(a_copia, a);
    free(a);
    if (*b_copia != 0) {    
        *division = (int)(*(long*)a_copia / *(long*)b_copia);
        pila_apilar(pila_operaciones, division);
        free(a_copia);
        free(b_copia);
    } else {
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(division);
        return false;
    }
    return true;
}

long calcular_potencia(long base, long exponente) {    
    if (exponente == 0) {
        return 1;
    } else {
        long resultado = calcular_potencia(base * base, exponente / 2);
        if (exponente % 2 == 1) {            
            resultado = base * resultado;
        }
        return resultado;
    }
}

bool potencia(pila_t* pila_operaciones){    
    char* b = pila_desapilar(pila_operaciones);    
    char* a = pila_desapilar(pila_operaciones);    
    char* b_copia = calloc(SIZE_OF_LONG, sizeof(char));
    char* a_copia = calloc(SIZE_OF_LONG, sizeof(char));
    int* potencia = malloc(sizeof(int));
    if (!a || !b || !a_copia || !b_copia || !potencia) {
        printf("entro aca\n");
        free(a);
        free(b);
        free(a_copia);
        free(b_copia);
        free(potencia);
        return false;
    }
    strcpy(b_copia, b);
    free(b);    
    strcpy(a_copia, a);
    free(a);
    printf("%s\n", b_copia);
    printf("%f\n", atof(a_copia));
    *potencia = (int)calcular_potencia(*(long*)(a_copia), *(long*)(b_copia));
    pila_apilar(pila_operaciones, potencia);
    free(a_copia);
    free(b_copia);
    return true;
}

bool es_numero(char* cadena) {
    if((atoi(cadena) == 0 && strcmp(cadena,"0") == 0) || (atoi(cadena) != 0)) {
        return true;
    }
    return false;    
}

void free_pila(pila_t* pila_operaciones) {
    while (pila_largo(pila_operaciones) > 1) {
        int* valor = pila_desapilar(pila_operaciones);        
        free(valor);
    }
}

char* formatear_operacion(char* operacion) {
    char** lista_operaciones_rec = split(operacion, '\0');
    if (!lista_operaciones_rec) {
        return NULL;
    }
    char** lista_operaciones_rec_2 = split(lista_operaciones_rec[0], '\n');
    if (!lista_operaciones_rec_2) {
        free_strv(lista_operaciones_rec);
        return NULL;
    }
    char* operacion_final = malloc(sizeof(strlen(lista_operaciones_rec_2[0])));
    if (!operacion_final) {
        free_strv(lista_operaciones_rec);
        free_strv(lista_operaciones_rec_2);
        return NULL;
    }

    strcpy(operacion_final, lista_operaciones_rec_2[0]);    
    free_strv(lista_operaciones_rec);
    free_strv(lista_operaciones_rec_2);    
    return operacion_final;
}

void imprimir_resultado(bool resultado_ok, int valor) {
    if (resultado_ok) {            
            if(valor) {
                fprintf(stdout, "%d\n", valor);
            } else {
                fprintf(stdout, "\n");
            }
            
        } else {
            fprintf(stdout, "ERROR\n");
        }
}
int calculadora_polaca() {
    pila_t* pila_operaciones = pila_crear();
    if (!pila_operaciones) {
        return 1;
    }
    char* linea = NULL;
    size_t capacidad = 0;
    while (getline(&linea, &capacidad, stdin) != EOF) {
        bool resultado_ok = true;        
        char** lista_operaciones = split(linea, ' ');
        if (!lista_operaciones) {
            return 1;
        }
        size_t i_lista_operaciones = 0;
        while(lista_operaciones[i_lista_operaciones] && resultado_ok) {            
            char* operacion = formatear_operacion(lista_operaciones[i_lista_operaciones]);
            if (es_numero(operacion)) {
                int* numero = malloc(sizeof(int));
                if(numero == NULL) {
                    fprintf(stdout, "ERROR\n");
                    return 1;
                }
                *numero = atoi(lista_operaciones[i_lista_operaciones]);                
                pila_apilar(pila_operaciones, numero);
            } else if (strcmp(operacion, "+") == 0) {                
                resultado_ok = suma(pila_operaciones);
            } else if (strcmp(operacion, "-") == 0) {                
                resultado_ok = resta(pila_operaciones);
            } else if (strcmp(operacion, "*") == 0) {                
                resultado_ok = multiplicacion(pila_operaciones);
            } else if (strcmp(operacion, "/") == 0) {                
                resultado_ok = division(pila_operaciones);
            } else if (strcmp(operacion, "^") == 0) {                      
                resultado_ok = potencia(pila_operaciones);       
            }     
            i_lista_operaciones++;                        
            free(operacion);
        }
        
        if (pila_largo(pila_operaciones) > 1 || (i_lista_operaciones == 1 && !pila_esta_vacia(pila_operaciones))) {            
            resultado_ok = false;
            free_pila(pila_operaciones);
        }        
        int* valor = pila_desapilar(pila_operaciones);        
        
        imprimir_resultado(resultado_ok, *valor);
        
        free(valor);
        
        free_strv(lista_operaciones);                

    }
    pila_destruir(pila_operaciones);
    free(linea);
    return 0;
}