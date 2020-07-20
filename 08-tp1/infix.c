#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "cola.h"
#include "strutil.h"
#include <string.h>
#define CANT_MIN_ARREGLO_SPLIT 2;

enum estado {SUMA = 2, RESTA = 2, MULTIPLICACION = 3, DIVISION = 3, POTENCIA = 4, RAIZ = 4, TERNARIO = 1, LOG = 1, APER_PARENTESIS = 8, CIERRE_PARENTESIS = 9};

typedef struct operacion {
    char* operacion;
    size_t precedencia;
    bool asociativo_izquierda;
} operacion_t;

bool es_numero(char* cadena) {
    return((atol(cadena) == 0 && strcmp(cadena,"0") == 0) || (atol(cadena) != 0));    
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
    
    char* operacion_final = strdup(lista_operaciones_rec_2[0]);
     if (!operacion_final) {        
        free_strv(lista_operaciones_rec);
        free_strv(lista_operaciones_rec_2);
        return NULL;
    }
    free_strv(lista_operaciones_rec);
    free_strv(lista_operaciones_rec_2);
    return operacion_final;
}

void imprimir_resultado_formateado(bool resultado_ok, char* valor) {
    if (resultado_ok) {            
            if(valor) {
                fprintf(stdout, "%s ", valor);
            } else {
                fprintf(stdout, "\n");
            }
            
        } else {
            fprintf(stdout, "ERROR\n");
        }
}

void liberar_pila(pila_t* pila_operaciones, cola_t* cola_salida) {
    while (!pila_esta_vacia(pila_operaciones)) {
        operacion_t* operador_salida = pila_desapilar(pila_operaciones);
        char* salida = strdup(operador_salida->operacion);
        if (!salida) {
            free(operador_salida->operacion);
            free(operador_salida);
            return;
        }
        cola_encolar(cola_salida, salida);
        free(operador_salida->operacion);
        free(operador_salida);
    }
}

char* formatear_operacion_infix(char* operacion) {
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

operacion_t* crear_operador(char* operador, size_t precedencia, bool asociativo_izq) {
    operacion_t* operacion = malloc(sizeof(operacion_t));
    if (!operacion) {
        return NULL;
    }
    operacion->operacion = operador;
    operacion->precedencia = precedencia;
    operacion->asociativo_izquierda = asociativo_izq;
    return operacion;
}

bool procesar_operacion(operacion_t* operacion, pila_t* pila_operaciones, cola_t* cola_salida) {        
    if (pila_esta_vacia(pila_operaciones)) {        
        pila_apilar(pila_operaciones, operacion);
    } else {
        operacion_t* operador_en_pila = pila_ver_tope(pila_operaciones);        
        if (operacion->precedencia == CIERRE_PARENTESIS) {            
            bool error_balanceo = false;
            size_t contador = 0;
            while (!pila_esta_vacia(pila_operaciones) && !error_balanceo) { 
                operacion_t* operador_salida = pila_desapilar(pila_operaciones);
                if (operador_salida->precedencia == APER_PARENTESIS ) {
                    free(operador_salida->operacion);
                    free(operador_salida);                    
                    return true;
                } 
                if (pila_esta_vacia(pila_operaciones)) {
                    free(operador_salida->operacion);
                    free(operador_salida);
                    return false;    
                }
                char* salida = malloc(sizeof(char) + 1);
                strcpy(salida,operador_salida->operacion);
                
                cola_encolar(cola_salida, salida); 
                free(operador_salida->operacion);               
                free(operador_salida);
                free(operacion->operacion);
                free(operacion);
                contador++;
            }
        }
        bool continuar = true;
        while (continuar && !pila_esta_vacia(pila_operaciones)) {
            if ((operador_en_pila->precedencia > operacion->precedencia && strcmp(operador_en_pila->operacion, "(") != 0 && strcmp(operador_en_pila->operacion, ")") != 0) || (operador_en_pila->precedencia == operacion->precedencia && operacion->asociativo_izquierda)) {                        
                operacion_t* operador_salida = pila_desapilar(pila_operaciones);
                char* salida = strdup(operador_salida->operacion);
                if (!salida) {
                    free(operador_salida->operacion);
                    free(operador_salida);
                    return false;
                }
                cola_encolar(cola_salida, salida);
                free(operador_salida->operacion);
                free(operador_salida);
                operador_en_pila = pila_ver_tope(pila_operaciones);
            } else {
                continuar = false;
            }
        }
        
        pila_apilar(pila_operaciones, operacion);
    }
    return true;
}

bool es_operador(const char str) {
    return (str == ' ' || str == '+' || str == '-' || str == '*' || str == '/' || str == '^' || str == '?' || str == '(' || str == ')'); 
}

size_t contar_coincidencias_infix(const char* str) {
    size_t total_coincidencias = 2;
    size_t ultima_coincidencia = 0;
    for (size_t i = 0; i <= strlen(str); i++) {
        if(es_operador(str[i]) || str[i] == '\0') {
            total_coincidencias++;            
            //esto es para verificar que se agreguen posiciones para los numeros entre los operadores.
            //No agrega nada si entre el operador actual y el anterior encontrado no habia espacios.
            if (i - ultima_coincidencia > 1) {
                total_coincidencias++;
            }
            ultima_coincidencia = i;
        }
    }    
    return total_coincidencias;
}

char **split_infix(const char *str) {
    size_t total_coincidencias = contar_coincidencias_infix(str);
    char** split = malloc(sizeof(strlen(str)) * total_coincidencias + 1);
    if (!split) {
        return NULL;    
    }

    size_t pos_split = 0;
    size_t pos_str = 0;
    for (size_t i = 0; i <= strlen(str); i++) {
        if(es_operador(str[i])) {
            if (i == 0) {
                split[pos_split] = substr(str, 1);                
                if (pos_str < strlen(str)) {
                    pos_str++;
                }
            } else if (i < strlen(str) - 1) { 
                if (i - pos_str - 1 == 0) {
                    split[pos_split] = substr(str + pos_str, 1);
                    pos_split++;
                } else {
                    if(i != pos_str) {
                        split[pos_split] = substr(str + pos_str, i - pos_str);
                        pos_split++;
                    }                    
                }                                               
                split[pos_split] = substr(str + i, 1);
                
                if (pos_str < strlen(str)) {
                    pos_str = i + 1;
                }                                                
            } else if (i == strlen(str) - 1) {                  
                split[pos_split] = substr(str + i - 1, 1);                
            }                               
            if (pos_split < total_coincidencias - 1) {         
                pos_split++;
            } 
        } else if (str[i] == '\0') {
            split[pos_split] = substr(str + pos_str, i - pos_str);
            if (pos_split < total_coincidencias - 1) {
                pos_split++;
            }
        }
    }
    split[pos_split] = NULL;
    return split;
}

long infix() {
    pila_t* pila_operaciones = pila_crear();
    if (!pila_operaciones) {
        return 1;
    }

    cola_t* cola_salida = cola_crear();
    if (!cola_salida) {
        return 1;
    }    

    char* linea = NULL;
    size_t capacidad = 0;
    while (getline(&linea, &capacidad, stdin) != EOF) {
        bool resultado_ok = true;
        char** lista_operaciones = split_infix(linea);
        if (!lista_operaciones) {
            return 1;
        }
        
        size_t i_lista_operaciones = 0;
        while (lista_operaciones[i_lista_operaciones] && resultado_ok) {
            if (strlen(lista_operaciones[i_lista_operaciones]) > 0) {
                char* operacion;
                if (strlen(lista_operaciones[i_lista_operaciones]) > 1) {                    
                    operacion = formatear_operacion(lista_operaciones[i_lista_operaciones]);
                } else {                    
                    operacion = strdup(lista_operaciones[i_lista_operaciones]);
                }
                if (es_numero(operacion)) {
                    char* numero = malloc(strlen(operacion) + 1);
                    if(!numero) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        return 1;
                    }
                    strcpy(numero, operacion);
                    cola_encolar(cola_salida, numero);
                    free(operacion);
                } else if (strcmp(operacion, "+") == 0) {                
                    operacion_t* operador_suma = crear_operador(operacion, SUMA, true);
                    if (!operador_suma) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_suma, pila_operaciones, cola_salida);
                    }                
                } else if (strcmp(operacion, "-") == 0) {                
                    operacion_t* operador_resta = crear_operador(operacion, RESTA, true);
                    if (!operador_resta) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_resta, pila_operaciones, cola_salida);
                    }
                } else if (strcmp(operacion, "*") == 0) {    
                    operacion_t* operador_multiplicacion = crear_operador(operacion, MULTIPLICACION, true);
                    if (!operador_multiplicacion) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_multiplicacion, pila_operaciones, cola_salida);
                    }
                } else if (strcmp(operacion, "/") == 0) {   
                    operacion_t* operador_division = crear_operador(operacion, DIVISION, true);
                    if (!operador_division) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_division, pila_operaciones, cola_salida);
                    }
                } else if (strcmp(operacion, "^") == 0) {   
                    operacion_t* operador_potencia = crear_operador(operacion, POTENCIA, false);
                    if (!operador_potencia) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_potencia, pila_operaciones, cola_salida);
                    }
                }  else if (strcmp(operacion, "?") == 0) {         
                    operacion_t* operador_ternario = crear_operador(operacion, TERNARIO, true);
                    if (!operador_ternario) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_ternario, pila_operaciones, cola_salida);
                    }      
                } else if (strcmp(operacion, "sqrt") == 0) {   
                    operacion_t* operador_raiz = crear_operador(operacion, RAIZ, false);
                    if (!operador_raiz) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_raiz, pila_operaciones, cola_salida);
                    }      
                } else if (strcmp(operacion, "log") == 0) { 
                    operacion_t* operador_logaritmo = crear_operador(operacion, LOG, true);
                    if (!operador_logaritmo) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_logaritmo, pila_operaciones, cola_salida);
                    }
                } else if (strcmp(operacion, "(") == 0) {
                    operacion_t* operador_apertura = crear_operador(operacion, APER_PARENTESIS, true);
                    if (!operador_apertura) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_apertura, pila_operaciones, cola_salida);
                    }
                } else if (strcmp(operacion, ")") == 0) {                                                         
                    operacion_t* operador_cierre = crear_operador(operacion, CIERRE_PARENTESIS, true);
                    if (!operador_cierre) {
                        free(operacion);
                        free_strv(lista_operaciones);
                        resultado_ok = false;
                    } else {
                        resultado_ok = procesar_operacion(operador_cierre, pila_operaciones, cola_salida);
                    }
                } else {
                    free(operacion);
                }
                
            }            
            i_lista_operaciones++;            
        }

        liberar_pila(pila_operaciones, cola_salida);
            
        while(!cola_esta_vacia(cola_salida)) {            
            void* valor = cola_desencolar(cola_salida);        
            imprimir_resultado_formateado(resultado_ok, valor);
            free(valor);
        }
        fprintf(stdout, "\n");
        free_strv(lista_operaciones);                            
    }
    pila_destruir(pila_operaciones);
    cola_destruir(cola_salida, free);
    free(linea);
    return 0;
}

int main(void) {    
    infix();
    return 0;
}