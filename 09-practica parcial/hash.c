#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "hash.h"
#include <string.h>
#include <stdio.h>


#define OCUPADO 1
#define BORRADO 2
#define VACIO 0
#define TAM_INICIAL 31
#define CARGA_MAX 0.7
#define CARGA_MIN 0.3
/* ******************************************************************
 *                           STRUCTS
 * *****************************************************************/
typedef struct campo{
  char* clave;
  void* valor;
  size_t estado;
}campo_t;

struct hash{
  size_t capacidad;
  size_t cantidad;
  hash_destruir_dato_t funcion_destruccion;
  campo_t* campos;
};

struct hash_iter{
  const hash_t* hash;
  campo_t campo_act;
  size_t posicion;
};

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/
//Duplica una cadena reservando memoria dinamica con malloc()
char* strdup (const char* s) {
	char* dup = malloc(strlen (s)+1);
	if (dup == NULL)
		return NULL;
  strcpy(dup,s);
	return dup;
}
//Funcion de hash " " obtenida en:
size_t fhash(const char *s, size_t tam){
    size_t hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = (size_t)*s + 31*hashval;
    return hashval % tam;
}
//Crea un campo con una clave, un valor y un estado asociado
campo_t crear_campo(char* clave, void* dato, size_t estado){
  campo_t campo;
  campo.estado = estado;
  campo.clave = clave;
  campo.valor = dato;
  return campo;
}
//Busca una clave y un estado en el hash, de no encontrar lo pedido devuelve -1
//Si lo encuentra, devuelve la posicion.
int hash_buscar(const hash_t* hash,const char* clave, size_t estado){
  int indice = (int)fhash(clave, hash->capacidad);
  int pos_act = 0;
  for (int i=0; i<(int)hash->capacidad; i++){
    pos_act = (indice+i)%(int)hash->capacidad;
    if (estado == OCUPADO && hash->campos[pos_act].estado == VACIO) break;
    if (estado == VACIO && hash->campos[pos_act].estado == VACIO) return -1;
    if (estado == BORRADO && hash->campos[pos_act].estado != OCUPADO) return pos_act;
    if (estado == OCUPADO && hash->campos[pos_act].estado == OCUPADO){
      if (strcmp(hash->campos[pos_act].clave,clave)==0) return pos_act;
    }
  }
  return -1;
}
//Copia los los campos de una lista de campos a otra.
bool campos_copiar(hash_t*hash, char *clave, void *dato){
  size_t pos;
  campo_t campo = crear_campo(clave, dato, OCUPADO);
  pos = hash_buscar(hash, clave, BORRADO);
  hash->campos[pos] = campo;
  return true;
}
//Inicializa los campos de una lista de campos con clave: "", valor: NULL y
// estado: VACIO.
void campos_inicializar(campo_t* lista_campos, size_t tam){
  for (size_t i=0; i<tam; i++){
    lista_campos[i] = crear_campo("", NULL, VACIO);
  }
}
//Redimensiona el hash a un tamanio dado
bool hash_redimensionar(hash_t* hash, size_t tam){
  campo_t* lista_campos_nuevo = malloc(tam * sizeof(campo_t));
  if (lista_campos_nuevo == NULL) return false;
  campo_t* lista_campos_act = hash->campos;
  size_t tam_actual = hash->capacidad;
  campos_inicializar(lista_campos_nuevo, tam);
  hash->capacidad = tam;
  hash->campos = lista_campos_nuevo;
  for (size_t i=0; i<tam_actual;i++){
    if (lista_campos_act[i].estado == OCUPADO){
      campos_copiar(hash, lista_campos_act[i].clave, lista_campos_act[i].valor);
    }
  }
  free(lista_campos_act);
  return true;
}

/* ******************************************************************
 *                       PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
   hash_t* hash = malloc(sizeof(hash_t));
   hash->cantidad = 0;
   hash->capacidad = TAM_INICIAL;
   hash->funcion_destruccion = destruir_dato;
   hash->campos = malloc(TAM_INICIAL*sizeof(campo_t));
   campos_inicializar(hash->campos, TAM_INICIAL);
   return hash;
 }

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
  int pos = hash_buscar(hash, clave, OCUPADO);
  if (pos >= 0){
    void* valor_ant = hash->campos[pos].valor;
    if (hash->funcion_destruccion != NULL) hash->funcion_destruccion(valor_ant);
    hash->campos[pos].valor = dato;
    return true;
  }
  campo_t campo = crear_campo(strdup(clave), dato, OCUPADO);
  pos = hash_buscar(hash, clave, BORRADO);
  hash->campos[pos] = campo;
  hash->cantidad++;
  if (((double)hash->cantidad)/(double)hash->capacidad >= CARGA_MAX){
    hash_redimensionar(hash, hash->capacidad*2);
  }
  return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
  int pos = hash_buscar(hash, clave, OCUPADO);
  if (pos < 0) return NULL;
  void* dato = hash->campos[pos].valor;
  free(hash->campos[pos].clave);
  hash->campos[pos].estado = BORRADO;
  hash->campos[pos].valor = NULL;
  hash->campos[pos].clave ="";
  hash->cantidad--;
  //if (hash->funcion_destruccion != NULL) hash->funcion_destruccion(dato);
  if ((double)hash->cantidad/(double)hash->capacidad < CARGA_MIN && hash->capacidad/2>TAM_INICIAL){
    hash_redimensionar(hash, hash->capacidad/2);
  }
  return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
  int pertenencia = hash_buscar(hash, clave, OCUPADO);
  if (pertenencia<0) return NULL;
  return hash->campos[pertenencia].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
  if (hash->cantidad == 0) return false;
  return hash_buscar(hash, clave, OCUPADO) >= 0;
}

size_t hash_cantidad(const hash_t *hash){
  return hash->cantidad;
}

void hash_destruir(hash_t *hash){
  for (size_t i=0; i<hash->capacidad; i++){
    if (hash->campos[i].estado != OCUPADO) continue;
    if (hash->funcion_destruccion != NULL){
      hash->funcion_destruccion(hash->campos[i].valor);
    }
    free(hash->campos[i].clave);
  }
  free(hash->campos);
  free(hash);
}



/* ******************************************************************
 *                       PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
  hash_iter_t* iter = malloc(sizeof(hash_iter_t));
  if (iter == NULL) return NULL;
  iter->hash = hash;
  iter->posicion = 0;
  if (iter->hash->cantidad > 0){
    while (iter->hash->campos[iter->posicion].estado != OCUPADO){
      iter->posicion++;
    }
  }
  iter->campo_act = iter->hash->campos[iter->posicion];
  return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
  if (hash_iter_al_final(iter)) return false;
  do{
    iter->posicion++;
    if (hash_iter_al_final(iter)) return false;
  } while (iter->hash->campos[iter->posicion].estado != OCUPADO);
  iter->campo_act = iter->hash->campos[iter->posicion];
  return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
  if (hash_iter_al_final(iter)) return NULL;
  if (iter->hash->cantidad == 0) return NULL;
  return iter->campo_act.clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
  if (iter->hash->cantidad == 0) return true;
  if (iter->hash->capacidad == iter->posicion) return true;
  return false;
}

void hash_iter_destruir(hash_iter_t* iter){
  free(iter);
}