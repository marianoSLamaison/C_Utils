#ifndef HASH_MAP_H
#define HASH_MAP_H
//Nota: 
//	Las llamadas a librerias extra como stdlib, se hacen en el .c para
//	evitar inclusiones no deseadas

//NOTA: quiero que sea de referencia cerrada me parece mas justo
//por ahora solo hay dos mensajes de error
//uno para si solamente no pudo generar
//la entrada en el diccionario
//y otro para si pudo generarla, necesito
//hacer un rehash y no pudo.
enum MENSAJES{
	FAILURE_CREATING = 0x01,
	FAILURE_REHASHING = 0x02,
	KEY_IN_USE = 0x04,//si confirmamos que la key ya estaba en uso
	EXITO = 0x00
};
//la definicion esta en el punto c
//para que no anden tocando valores
//que no deberian
typedef struct HASH_MAP t_hmap;

typedef struct KEY_VAL_PAIR
{
	void* key;
	//Yes I will rather just use unsigned int before bowing to stddef.h
	unsigned int key_size;
	void* val;
} t_kvpair;

//crea un hash_map con valores default,
//vacio
t_hmap* hmap_create(void);
/**
 * Remueve un elemento del hash_map
 * Retornra el par de key y dato
 * que a sido retirado o NULL si no 
 * encontro nada.
 * */
t_kvpair* hmap_remove_pair(unsigned char* key, unsigned int key_size, t_hmap* map);
/**
 * Retorna el valor asociado a una key,
 * sin removerlo del map en si.
 * Retorna NULL si no encontro nada
 * */
void* hmap_get_value(unsigned char* key, unsigned int key_size, t_hmap* map);
/**
 * Añade un elemento al diccionario,
 * Retorna 0 si la operacion fue exitosa
 * Retorna un mensaje de error si no
 * y no añade el elemento.
 * Puede retornar los dos mensajes a la vez,
 * para confirma use los mensajes a modo
 * de mascara y compare
 * EJ: ret & FAILURE_CREATING == FAILURE_CREATING
 * */
int hmap_add(unsigned char* key, unsigned int key_size, void* val, t_hmap* map);
/**
 * Destruye todo el diccionario suponiendo
 * que lo creaste solamente de valores que todavia existen en arrays
 * generados automaticamente
 * */
void hmap_destroy_lists_map(t_hmap* map);
/**
 * Destruye todo el diccionario, pidiendo funciones
 * especiales para liberar las variables. 
 * (Si bien se usa unsigned char* para la key, puedes usar cualquier cosa en realidad
 * no se toma en cuenta los datos a los que apunten los punteros por ahora.)
 * */
void hmap_destroy(void (*val_destroyer)(void*), void (*key_destroyer)(void*), t_hmap* map);

/**
MatroFunction solo llama a hmap_destroy con free como su argumento para key_destroyer
*/
#define hmap_destroy_v(VDESTROYER, MAP) (hmap_destroy((VDESTROYER), free, (MAP)))
/**
MatroFunction solo llama a hmap_destroy con free como su argumento para val_destroyer
*/
#define hmap_destroy_k(KDESTROYER, MAP) hmap_destroy(free, (KDESTROYER), (MAP)) 

#endif
