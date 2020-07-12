#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "strutil.h"
#include <string.h>
#define SIZE_OF_LONG 20

bool suma(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* b = pila_desapilar(pila_operaciones);
        long* a = pila_desapilar(pila_operaciones);        
        *resultado = *a + *b;
        pila_apilar(pila_operaciones, resultado);
        free(a);
        free(b);
    } else {
        return false;
    }
    return true;
}

bool resta(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* b = pila_desapilar(pila_operaciones);
        long* a = pila_desapilar(pila_operaciones);        
        *resultado = *a - *b;
        pila_apilar(pila_operaciones, resultado);
        free(a);
        free(b);
    } else {
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

bool potencia(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* exponente = pila_desapilar(pila_operaciones);
        long* base = pila_desapilar(pila_operaciones);        
        if (*exponente >= 0) {
            *resultado = calcular_potencia(*(long*)base, *(long*)exponente);
            pila_apilar(pila_operaciones, resultado);
            free(base);
            free(exponente);
        } else {
            free(base);
            free(exponente);
            free(resultado);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

long calcular_raiz(long radicando, long inicio, long fin) {
    if ( inicio > fin) {
        return inicio - 1;
    }
    long medio = (inicio + fin) / 2;
    if (medio * medio == radicando || (medio * medio) + 1 == radicando) {
        return medio;
    }
    if (medio * medio > radicando) {        
        return calcular_raiz(radicando, inicio, medio - 1);
    } else {
        return calcular_raiz(radicando, medio + 1, fin);
    }
}

bool raiz(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 1) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* radicando = pila_desapilar(pila_operaciones);
        if (*radicando >= 0) {
            *resultado = calcular_raiz(*radicando, 1, *radicando);
            pila_apilar(pila_operaciones, resultado);
            free(radicando);
        } else {
            free(radicando);
            free(resultado);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool division(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }        
        long* divisor = pila_desapilar(pila_operaciones);        
        long* dividendo = pila_desapilar(pila_operaciones);        
        if (*divisor != 0) {
            *resultado = *dividendo / *divisor;
            pila_apilar(pila_operaciones, resultado);
            free(dividendo);
            free(divisor);
        } else {
            free(dividendo);
            free(divisor);
            free(resultado);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool multiplicacion(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* multipicador = pila_desapilar(pila_operaciones);
        long* multiplicando = pila_desapilar(pila_operaciones);        
        *resultado = *multiplicando * *multipicador;
        pila_apilar(pila_operaciones, resultado);
        free(multiplicando);
        free(multipicador);
    } else {
        return false;
    }
    return true;
}

long calcular_logaritmo(long base, long valor) {
    if (valor == 1 || valor < base) {
        return 0;
    }
    return calcular_logaritmo(base, valor / base) + 1;
}

bool logaritmo(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 2) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* base = pila_desapilar(pila_operaciones);
        long* valor = pila_desapilar(pila_operaciones);        
        if (*base > 1) {
            *resultado = calcular_logaritmo(*base, *valor);
            pila_apilar(pila_operaciones, resultado);
            free(valor);
            free(base);
        } else {
            free(valor);
            free(base);
            free(resultado);
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool ternario(pila_t* pila_operaciones) {
    if (pila_largo(pila_operaciones) >= 3) {
        long* resultado = malloc(sizeof(long));
        if(resultado == NULL) {
            return false;
        }
        long* valor_si_falso = pila_desapilar(pila_operaciones);
        long* valor_si_verdadero = pila_desapilar(pila_operaciones);        
        long* comparacion = pila_desapilar(pila_operaciones);

        *resultado = *comparacion != 0 ? *valor_si_verdadero : *valor_si_falso;
        pila_apilar(pila_operaciones, resultado);
        free(comparacion);
        free(valor_si_verdadero);
        free(valor_si_falso);

    } else {
        return false;
    }
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
        long* valor = pila_desapilar(pila_operaciones);        
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

void imprimir_resultado(bool resultado_ok, long* valor) {
    if (resultado_ok) {            
            if(valor) {
                fprintf(stdout, "%ld\n", *(long*)valor);
            } else {
                fprintf(stdout, "\n");
            }
            
        } else {
            fprintf(stdout, "ERROR\n");
        }
}
long calculadora_polaca() {
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
                long* numero = malloc(sizeof(long));
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
            }  else if (strcmp(operacion, "?") == 0) {                      
                resultado_ok = ternario(pila_operaciones);       
            } else if (strcmp(operacion, "sqrt") == 0) {                      
                resultado_ok = raiz(pila_operaciones);       
            } else if (strcmp(operacion, "log") == 0) {                      
                resultado_ok = logaritmo(pila_operaciones);       
            }
                
            i_lista_operaciones++;                        
            free(operacion);
        }
        
        if (pila_largo(pila_operaciones) > 1 || (i_lista_operaciones == 1 && !pila_esta_vacia(pila_operaciones))) {            
            resultado_ok = false;
            free_pila(pila_operaciones);
        }        
        long* valor = pila_desapilar(pila_operaciones);        
        
        imprimir_resultado(resultado_ok, valor);
        
        free(valor);
        
        free_strv(lista_operaciones);                

    }
    pila_destruir(pila_operaciones);
    free(linea);
    return 0;
}