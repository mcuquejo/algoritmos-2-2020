#include "strutil.h"
#include "testing.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 *                   PRUEBAS UNITARIAS ALUMNO
 * *****************************************************************/
void pruebas_substr() {
    char* str0 = substr("Hola mundo", 0);
    print_test("Substring de \'Hola mundo\' de 0 es igual a \'\'", strcmp(str0, "") == 0);

    char* str1 = substr("Hola mundo", 6);
    print_test("Substring de \'Hola mundo\' de 6 es igual a \'Hola m\'", strcmp(str1, "Hola m") == 0);
    
    char* str2 = substr("Algoritmos", 30);
    print_test("Substring de \'Algoritmos\' de 30 es igual a \'Algoritmos\'", strcmp(str2, "Algoritmos") == 0);
    
    char* str3 = substr("", 2);
    print_test("Substring de \'\' de 2 es igual a \'\'", strcmp(str3, "") == 0);

    const char* ejemplo = "Ejemplo";
    char* str4 = substr(ejemplo, 2);
    print_test("Substring de \'Ejemplo\' de 2 es igual a \'Ej\'", strcmp(str4, "Ej") == 0);
    
    char* str5 = substr(ejemplo + 4, 2);
    print_test("Substring de \'Ejemplo\' + 4 de 2 es igual a \'pl\'", strcmp(str5, "pl") == 0);

    free(str1);
    free(str2);
    free(str3);
    free(str4);
    free(str5);
}

void pruebas_split() {
    char** strv1 = split("abc,def,ghi", ',');
    print_test("split[0] de \'abc,def,ghi\' es igual a \'abc\'", strcmp(strv1[0], "abc") == 0);
    print_test("split[1] de \'abc,def,ghi\' es igual a \'def\'", strcmp(strv1[1], "def") == 0);
    print_test("split[2] de \'abc,def,ghi\' es igual a \'ghi\'", strcmp(strv1[2], "ghi") == 0);    
    
    char** strv2 = split("abc,,def,", ',');
    print_test("split[0] de \'abc,,def,\' es igual a \'abc\'", strcmp(strv2[0], "abc") == 0);
    print_test("split[1] de \'abc,,def,\' es igual a \'\'", strcmp(strv2[1], "") == 0);
    print_test("split[2] de \'abc,,def,\' es igual a \'def\'", strcmp(strv2[2], "def") == 0);
    print_test("split[2] de \'abc,,def,\' es igual a \'\'", strcmp(strv2[3], "") == 0);
    
    char** strv3 = split("abc,def,", ',');
    print_test("split[0] de \'abc,def,\' es igual a \'abc\'", strcmp(strv3[0], "abc") == 0);
    print_test("split[1] de \'abc,def,\' es igual a \'def\'", strcmp(strv3[1], "def") == 0);
    print_test("split[2] de \'abc,def,\' es igual a \'\'", strcmp(strv3[2], "") == 0);
    
    char** strv4 = split(",abc,def", ',');
    print_test("split[0] de \',abc,def\' es igual a \'\'", strcmp(strv4[0], "") == 0);
    print_test("split[1] de \',abc,def\' es igual a \'abc\'", strcmp(strv4[1], "abc") == 0);
    print_test("split[2] de \',abc,def\' es igual a \'def\'", strcmp(strv4[2], "def") == 0);
    
    char** strv5 = split("abc", '\0');
    print_test("split[0] de \'abc\' es igual a \'abc\'", strcmp(strv5[0], "abc") == 0);
    
    char** strv6 = split("", ',');
    print_test("split[0] de \'\' es igual a \'\'", strcmp(strv6[0], "") == 0);    
    
    char** strv7 = split(",", ',');
    print_test("split[0] de \',\' es igual a \'\'", strcmp(strv7[0], "") == 0);
    print_test("split[1] de \',\' es igual a \'\'", strcmp(strv7[1], "") == 0);

    free_strv(strv1);
    free_strv(strv2);
    free_strv(strv3);
    free_strv(strv4);
    free_strv(strv5);
    free_strv(strv6);
    free_strv(strv7);
}

void pruebas_join() {
    char** strv1 = split("abc,def,ghi", ',');
    char* strv1_joined = join(strv1, ';');
    print_test("join de strv [\'abc\', \'def\', \'ghi\'] es igual a \'abc;def;ghi\'", strcmp(strv1_joined, "abc;def;ghi") == 0);    

    char **strv2 = split("Hola mundo", ' ');
    char *strv2_joined = join(strv2, ',');  // "Hola,mundo"
    print_test("join de strv [\'Hola\', \'mundo\'] es igual a \'Hola,mundo\'", strcmp(strv2_joined, "Hola,mundo") == 0);    
    
    char** strv3 = split("", ',');
    char* strv3_joined = join(strv3, ',');
    print_test("join de strv [\''] es igual a \'\'", strcmp(strv3_joined, "") == 0);    

    char** strv4 = split("abc", '\0');
    char* strv4_joined = join(strv4, ',');
    print_test("join de strv [\'abc\'] es igual a \'abc\'", strcmp(strv4_joined, "abc") == 0);    
    
    char** strv5 = split(",", ',');
    char* strv5_joined = join(strv5, ',');
    print_test("join de strv [\'\', \'\'] es igual a \',\'", strcmp(strv5_joined, ",") == 0);    
    
    char** strv6 = {NULL};
    char* strv6_joined = join(strv6, ',');
    print_test("join de strv [] es igual a NULL", strv6_joined == NULL);    
    
    char** strv7 = split("abc,def", ',');
    char* strv7_joined = join(strv7, '\0');
    fprintf(stdout, "el string generado es = %s\n", strv7_joined);
    print_test("join de strv [\'abc\', \'def\'] es igual a \'abcdef\'", strcmp(strv7_joined, "abcdef") == 0);        

    free_strv(strv1);
    free_strv(strv2);
    free_strv(strv3);
    free_strv(strv4);
    free_strv(strv5);    
    free_strv(strv7);
    free(strv1_joined);
    free(strv2_joined);
    free(strv3_joined);
    free(strv4_joined);
    free(strv5_joined);
    free(strv6_joined);
    free(strv7_joined);

}


void pruebas_strutil_alumno() {
    pruebas_substr();
    pruebas_split();
    pruebas_join();
}