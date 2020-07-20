#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "pila.h"
#include "cola.h"
#include "strutil.h"
#include "dc.h"
#include <string.h>
#define CANT_MIN_ARREGLO_SPLIT 2;

enum estado {SUMA = 2, RESTA = 2, MULTIPLICACION = 3, DIVISION = 3, POTENCIA = 4, RAIZ = 4, TERNARIO = 1, LOG = 1, APER_PARENTESIS = 8, CIERRE_PARENTESIS = 9};

typedef struct operacion {
    char* operacion;
    size_t precedencia;
    bool asociativo_izquierda;
} operacion_t;

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

// bool es_numero(char* cadena) {
//     return ((atoi(cadena) == 0 && strcmp(cadena,"0") == 0) || (atoi(cadena) != 0));    
// }

void liberar_pila(pila_t* pila_operaciones, cola_t* cola_salida) {
    while (!pila_esta_vacia(pila_operaciones)) {
        operacion_t* operador_salida = pila_desapilar(pila_operaciones);
        char* salida = malloc(sizeof(char) + 1);
        strcpy(salida,operador_salida->operacion);
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
        // fprintf(stdout, "operador precedencia: %zu\n", operador_en_pila->precedencia);
        if (operacion->precedencia == CIERRE_PARENTESIS) {
            
            bool error_balanceo = false;
            size_t contador = 0;
            while (!pila_esta_vacia(pila_operaciones) && !error_balanceo) { 
                // printf("loop nro: %zu\n", contador);
                // printf("entro aca %s\n", operacion->operacion);
                operacion_t* operador_salida = pila_desapilar(pila_operaciones);
                // printf("este es el operador que saco de la pila %s\n", operador_salida->operacion);
                if (operador_salida->precedencia == APER_PARENTESIS ) {
                    // printf("por algun motivo entra aca? %s\n", operador_salida->operacion);
                    free(operador_salida->operacion);
                    free(operador_salida);                    
                    return true;
                } 
                //operacion_t* operacion_prueba = pila_ver_tope(pila_operaciones);
                // printf("me fijo tope de la pila dentro del loop despues de sacar el -: %s\n", operacion_prueba->operacion);
                if (pila_esta_vacia(pila_operaciones)) {
                    // printf("la pila se vacio y no encontré el parentesis\n");
                    free(operador_salida->operacion);
                    free(operador_salida);
                    return false;    
                }
                // printf("voy encolando lo que saco de la pila %s\n", operador_salida->operacion);
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
        if ((operador_en_pila->precedencia > operacion->precedencia && strcmp(operador_en_pila->operacion, "(") != 0 && strcmp(operador_en_pila->operacion, ")") != 0) || (operador_en_pila->precedencia == operacion->precedencia && operacion->asociativo_izquierda)) {                        
            operacion_t* operador_salida = pila_desapilar(pila_operaciones);
            char* salida = malloc(sizeof(char) + 1);
            strcpy(salida,operador_salida->operacion);
            cola_encolar(cola_salida, salida);
            free(operador_salida->operacion);
            free(operador_salida);
        }
        pila_apilar(pila_operaciones, operacion);
    }
    //operacion_t* operacion_prueba = pila_ver_tope(pila_operaciones);
    // printf("tope de la pila al final del proceso: %s\n", operacion_prueba->operacion);
    return true;
}

bool es_operador(const char str) {
    // str == '\0' || str == '\n' || 
    return (str == ' ' || str == '+' || str == '-' || str == '*' || str == '/' || str == '^' || str == '?' || str == '(' || str == ')'); 
}

size_t contar_coincidencias_infix(const char* str) {
    size_t total_coincidencias = 2;
    size_t ultima_coincidencia = 0;
    for (size_t i = 0; i <= strlen(str); i++) {
        // printf("----------INICIO CICLO--------------\n");
        // printf("str[i] = \'%c\'\n", str[i]);
        if(es_operador(str[i]) || str[i] == '\0') {
            // printf("str[i] = \'%c\' cuenta como operador\n", str[i]);            
            //no voy a agregar una posicion en el array para el \0
            //UPDATE: al final tenia que agregarlo.            
            total_coincidencias++;                
            
            //esto es para verificar que se agreguen posiciones para los numeros entre los operadores.
            //No agrega nada si entre el operador actual y el anterior encontrado no habia espacios.
            if (i - ultima_coincidencia > 1) {     
                // printf("str[i] = \'%c\' se suma posicion porque habia espacio\n", str[i]);
                total_coincidencias++;
            }
            ultima_coincidencia = i;
        }
    // printf("----------FIN CICLO--------------\n\n\n");
    }    
    return total_coincidencias;
}

char **split_infix(const char *str) {
    size_t total_coincidencias = contar_coincidencias_infix(str);    //15
    // printf("total coincidencias: %zu\n", total_coincidencias);
    char** split = malloc(sizeof(str) * total_coincidencias);
    if (!split) {
        return NULL;    
    }

    size_t pos_split = 0;
    size_t pos_str = 0;
    for (size_t i = 0; i <= strlen(str); i++) {
        if(es_operador(str[i])) {    
            // printf("posicion del string donde estoy parado ahora = %zu, valor str[i] = %c, posicion del arreglo en donde va a insertar: %zu\n", i, str[i], pos_split);
            if (i == 0) {
                split[pos_split] = substr(str, 1);                
                if (pos_str < strlen(str)) {
                    pos_str++;
                }
            } else if (i < strlen(str) - 1) { 
                if (i - pos_str - 1 == 0) {
                    // printf("A entra aca con pos_string = %zu\n", pos_str);
                    split[pos_split] = substr(str + pos_str, 1);
                    // printf("A al entrar aca, guarde la posicion anterior: split[pos_split] = \'%s\'\n", split[pos_split]);
                    pos_split++;
                } else {
                    if(i != pos_str) {
                        // printf("B entra aca con pos_string = %zu\n", pos_str);
                        split[pos_split] = substr(str + pos_str, i - pos_str);                    
                        // printf("B al entrar aca, guarde la posicion anterior?: split[pos_split] = \'%s\'\n", split[pos_split]);
                        pos_split++;
                    }                    
                }                                               
                split[pos_split] = substr(str + i, 1);
                // printf("Ahora si, guarde la posicion actual: split[pos_split] = \'%s\'\n", split[pos_split]);
                
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
            // printf("entra a guardar lo que esta detras del \\0\n");
            // printf("cuanto vale pos_split??? %zu\n", pos_split);
            split[pos_split] = substr(str + pos_str, i - pos_str);
            if (pos_split < total_coincidencias - 1) {
                // printf("luego de guardar lo ultimo, sumo una posicion en el split, para que quede null\n");
                pos_split++;
            }
        }
    }
    // printf("esto es lo que está pisando el NULL en split[pos_split] = %s\n", split[pos_split]);
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
            // fprintf(stdout, "%s %zu\n", lista_operaciones[i_lista_operaciones], strlen(lista_operaciones[i_lista_operaciones]) );
            if (strlen(lista_operaciones[i_lista_operaciones]) > 0) {
                char* operacion;
                if (strlen(lista_operaciones[i_lista_operaciones]) > 1) {                    
                    operacion = formatear_operacion(lista_operaciones[i_lista_operaciones]);
                    printf("operacion formateada: \'%s\'\n", operacion);                    
                } else {
                    operacion = malloc(sizeof(strlen(lista_operaciones[i_lista_operaciones])));
                    strcpy(operacion, lista_operaciones[i_lista_operaciones]);
                    // operacion = lista_operaciones[i_lista_operaciones];
                }
                // fprintf(stdout, "%s %zu\n", operacion, strlen(operacion) );
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
                    // printf("guardo la apertura de parentesis\n");
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
                    printf("este operador no entro por ningun lado: \'%s\'\n",operacion);
                    free(operacion);
                }
                
            }            
            i_lista_operaciones++;            
        }

        liberar_pila(pila_operaciones, cola_salida);
            
        while(!cola_esta_vacia(cola_salida)) {            
            void* valor = cola_desencolar(cola_salida);
            //fprintf(stdout, "%s ", (char*)valor);            
            imprimir_resultado_formateado(resultado_ok, valor);
            free(valor);
        }
        fprintf(stdout, "\n");
        printf("destruyo lista operaciones\n");
        free_strv(lista_operaciones);                            
    }
    
    printf("destruyo pila operaciones\n");    
    pila_destruir(pila_operaciones);
    printf("destruyo cola salida\n");
    cola_destruir(cola_salida, free);
    free(linea);
    return 0;
}