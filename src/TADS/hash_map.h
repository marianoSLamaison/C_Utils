#ifndef HASH_MAP_H
#define HASH_MAP_H


//NOTA: quiero que sea de referencia cerrada me parece mas justo
//por ahora solo hay dos mensajes de error
//uno para si solamente no pudo generar
//la entrada en el diccionario
//y otro para si pudo generarla, necesito
//hacer un rehash y no pudo.
enum MENSAJES{
	FAILURE_CREATING = 0x0F,
	FAILURE_REHASHING = 0xF0,
	EXITO = 0x00
};
//la definicion esta en el punto c
//para que no anden tocando valores
//que no deberian
typedef struct HASH_MAP t_hmap;

//crea un hash_map con valores default,
//vacio
t_hmap* hmap_create(void);
/**
 * Remueve un elemento del hash_map
 * Retornra el elemento o NULL si no 
 * encontro nada
 * */
void* hmap_remove_with_key(unsigned char* key, size_t key_size, t_hmap* map);
/**
 * Retorna el valor asociado a una key,
 * sin removerlo del map en si.
 * Retorna NULL si no encontro nada
 * */
void* hmap_get_value(unsigned char* key, size_t key_size, t_hmap* map);
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
int hmanp_add(unsigned char* key, size_t key_size, void* val, t_hmap* map);
/**
 * Destruye todo el diccionario suponiendo
 * que los vals* no tiene punteros a nada
 * dentro sullo
 * */
void hmap_simple_destroy(t_hmap* map);
/**
 * Destruye todo el diccionario, pidiendo una
 * funcion especial para destruir los valores
 * en caso de que estos tengan punteros a 
 * datos
 * */
void hmap_destroy_mv(void (*val_destroyer)(void*), t_hmap* map);
#endif
