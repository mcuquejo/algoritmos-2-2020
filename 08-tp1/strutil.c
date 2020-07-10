#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#define CANT_MIN_ARREGLO_SPLIT 2;


char *substr(const char *str, size_t n) {
    char* substring = malloc(sizeof(str) * n + 1);    
    if (!substring) {
        return NULL;
    }
    for (size_t i = 0; i < n + 1; i++) {
        substring[i] = '\0';
    }
    strncpy(substring, str, n);
    return substring;
}

size_t contar_coincidencias(const char* str, char sep) {
    size_t total_coincidencias = CANT_MIN_ARREGLO_SPLIT;
    for (size_t i = 0; i <= strlen(str); i++) {
        if(str[i] == sep) {                        
            total_coincidencias++;
        }
    }
    return total_coincidencias;
}

char **split(const char *str, char sep) {
    size_t total_coincidencias = contar_coincidencias(str, sep);

    for (size_t i = 0; i <= strlen(str); i++) {
        if(str[i] == sep) {                        
            total_coincidencias++;
        }
    }
    char** split = malloc(sizeof(str) * total_coincidencias);
    if (!split) {
        return NULL;    
    }

    size_t pos_i_split = 0;
    size_t pos_i_str = 0;
    for (size_t i = 0; i <= strlen(str); i++) {
        if(str[i] == sep || str[i] == '\0') {            
            split[pos_i_split] = substr(str + pos_i_str, i - pos_i_str);            
            if (pos_i_split < total_coincidencias - 1) {
                pos_i_split++;
            }
            if (pos_i_str < strlen(str)) {
                pos_i_str = i + 1;
            }            
        }
    } 
    split[pos_i_split] = NULL;
    return split;
}

void inicializar_datos_strv(char** strv, size_t* cant_separadores, size_t* largo_arreglo) {
    size_t v_cant_separadores = 0;
    size_t v_largo_arreglo = 0;
    if (!strv) {
        return;
    }
    while (strv[v_cant_separadores]) {
        v_largo_arreglo += strlen(strv[v_cant_separadores]) + 1;
        v_cant_separadores++;
        
    }
    *cant_separadores = v_cant_separadores - 1;
    *largo_arreglo = v_largo_arreglo + 1;    
}

char *join(char **strv, char sep) {
    if (!strv) {
        return NULL;
    }
    size_t cant_separadores = 0;
    size_t largo_arreglo = 0;
    inicializar_datos_strv(strv, &cant_separadores, &largo_arreglo);    
    char* joined_strv = malloc(sizeof(char) * largo_arreglo);
    if (!joined_strv) {
        return NULL;
    }

    for (size_t i = 0; i < largo_arreglo; i++) {
        joined_strv[i] = '\0';
    }

    size_t pos_arreglo = 0;
    size_t pos_strv = 0;
    while (strv[pos_arreglo]) {
        strncpy(joined_strv + pos_strv, strv[pos_arreglo], strlen(strv[pos_arreglo]));

        pos_strv += strlen(strv[pos_arreglo]);
        if (cant_separadores > 0 && sep != '\0') {
            strncpy(joined_strv + pos_strv, &sep, 1);
            cant_separadores--;
            pos_strv++;
        }
        pos_arreglo++;
    }

    return joined_strv;
}

void free_strv(char *strv[]){
    size_t i = 0;
    while (strv[i]) {
        free(strv[i]);
        i++;
    }
    free(strv);
}