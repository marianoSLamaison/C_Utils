#include "hash_map.h"
#include <stdlib.h>

#define DEFAULT_MAP_SIZE 0xFF
#define MAX_INDEX_NODES 0x1F
//why this way? because why not?
//the only thing bothering me was conmutatibility onestly
//
//My tree rules for a hash func
//1. No conmutatibility, it bothers me.
//2. No default values (if I had gone with the "treat them as vectors and cross produc them" I will have had
//   A lot of stuff just defaulting to 0.
//3. No calling functions. soo nothing that can't be calculated right here.
//   (macros are fine, as long as they don't call functions in secret)
#define g(n) (((n-n%0x08)<<0x0C)*3 + n*n)
#define REHASH_NEEDED 52

struct INDEX_NODE
{
	unsigned char* key;
	unsigned int key_size;
	void * val;
	struct INDEX_NODE * next;
};

struct INDEX_LIST
{
	struct INDEX_NODE* head;
	int count;//el cap es igual para todos, 
		  //a si que lo dejo como una constnte global
};

typedef struct HASH_MAP
{
	int cap;
	struct INDEX_LIST * indices;
}t_hmap;

t_hmap* hmap_create(void)
{
	t_hmap* ret = malloc(sizeof(t_hmap));
	struct INDEX_LIST* ret_indices;
	int i;
	ret->cap = DEFAULT_MAP_SIZE;
	ret_indices = malloc(sizeof(struct INDEX_LIST) * DEFAULT_MAP_SIZE);
	for(i=0; i<DEFAULT_MAP_SIZE; i++)
	{
		(ret_indices + i)->head = NULL;
		(ret_indices + i)->count = 0;
	}
	ret->indices = ret_indices;
	return ret;
}

static int get_index_from_key(unsigned char* key, unsigned int key_size, int cap)
{
	unsigned long hash = 0, n;
	int i;
	
	for(i=0; i<key_size; i++)
	{
		//creo que asi deberia quitar la conmutatibilidad
		hash += 1<<(hash & 0x07);//Fuck conmutatibility, all my homies hate 
		//conmutatibility
		n = key[i];
		hash += g(n);
	}
	return hash % cap;
}
static int index_list_push(struct INDEX_LIST* list, unsigned char* key,
			unsigned int key_size, void* val)
{
	struct INDEX_NODE* new_node = malloc(sizeof(struct INDEX_NODE));
	if (new_node == NULL) return FAILURE_CREATING;
	new_node->key      = key;
	new_node->key_size = key_size;
	new_node->val      = val;

	if (list->head == NULL)
	{
		new_node->next = NULL;
		list->head = new_node;
		goto FINISH;
	}
	new_node->next = list->head;
	list->head = new_node;
FINISH:
	list->count++;//pegamos el valor de todas formas a si no tenemos que hacer mas cosas luego
	if (list->count > MAX_INDEX_NODES) return REHASH_NEEDED; 
	return 0;
}
static int index_list_push_node(struct INDEX_LIST* list, struct INDEX_NODE* nodo)
{
	if (list->head==NULL)
	{
		list->head = nodo;
		list->count++;
		return 0;
	}
	nodo->next = list->head;
	list->head = nodo;
	list->count++;
	if (list->count > MAX_INDEX_NODES) return REHASH_NEEDED;
	return 0;
}

static struct INDEX_NODE* index_list_pop(struct INDEX_LIST* list)
{
	struct INDEX_NODE* ret = list->head;
	list->head = list->head->next;
	list->count--;
	return ret;
}

static int hmap_rehash(t_hmap* map)
{
	struct INDEX_LIST* new_ilist = malloc(sizeof(struct INDEX_LIST) * map->cap * 2);
	int i, new_cap = map->cap*2, indice;
	struct INDEX_NODE* nodo;
	if (new_ilist == NULL) return FAILURE_REHASHING;
	for(i=0; i<new_cap; i++)
	{
		(new_ilist + i)->head = NULL;
		(new_ilist + i)->count = 0;
	}
	for(i=0; i<map->cap; i++)
	{
		while((map->indices + i)->head != NULL)
		{
			nodo = index_list_pop(map->indices + i);
			indice = get_index_from_key(nodo->key, nodo->key_size, new_cap);
			index_list_push_node(new_ilist + indice, nodo);
		}
	}
	free(map->indices);
	map->indices = new_ilist;
	map->cap = new_cap;
	return 0;
}

static int cmp_keys(unsigned char* key1, unsigned int key_size1, 
		unsigned char* key2, unsigned int key_size2)
{
	int i;
	if (key1 == key2) return 0;//si apuntan a lo mismo son lo mismo en lo que a mi respecta
	if (key_size1 < key_size2) return -1;
	if (key_size1 > key_size2) return  1;

	for(i=0; i<key_size1; i++)
	{
		if (*(key1+i) < *(key2+i)) return -1;
		if (*(key1+i) > *(key2+i)) return  1;
	}
	return 0;
}

static struct INDEX_NODE* list_get_node(struct INDEX_LIST* list, 
					unsigned char* key, 
					unsigned int key_size)
{
	struct INDEX_NODE* nodo = list->head;
	if (list->count==0) return list->head;
	while(nodo != NULL)
	{
		if (cmp_keys(key, key_size, nodo->key, nodo->key_size) == 0) break;
		nodo = nodo->next;
	}
	return nodo;
}

static struct INDEX_NODE* list_remove_node(struct INDEX_LIST* list, 
					unsigned char* key, 
					unsigned int key_size)
{
	struct INDEX_NODE** passer = &list->head;
	struct INDEX_NODE*  nodo;
	while(*passer != NULL)
	{
		if ( cmp_keys(key, key_size, (*passer)->key, (*passer)->key_size) == 0 )
		{
			nodo = *passer;		
			*passer = (*passer)->next;
			break;
		}
		passer = &(*passer)->next;
	}
	return nodo;
}

t_kvpair* hmap_remove_pair(unsigned char* key, 
			unsigned int key_size, 
			t_hmap* map)
{
	int index = get_index_from_key(key, key_size, map->cap);
	t_kvpair* ret;
	struct INDEX_NODE* nodo = list_remove_node(map->indices + index, key, key_size);
	if (nodo == NULL) return NULL;
	ret = malloc(sizeof(t_kvpair));
	ret->key = nodo->key;
	ret->key_size = nodo->key_size;
	ret->val = nodo->val;
	free(nodo);
	return ret;
}

void* hmap_get_value(unsigned char* key, 
		unsigned int key_size, 
		t_hmap* map)
{
	int index = get_index_from_key(key, key_size, map->cap);
	struct INDEX_NODE* node = list_get_node(map->indices + index, key, key_size);
	return node->val;
}
//retorna el puntero a el index node o NULL si no encontro nada
static struct INDEX_NODE* index_list_find_by_key(struct INDEX_LIST* indices, 
						unsigned char* key, 
						unsigned int key_size)
{
	struct INDEX_NODE* passer = indices->head;
	while(passer != NULL)
	{
		if(cmp_keys(passer->key, passer->key_size, key, key_size)==0)
			return passer;
		passer = passer->next;
	}
	return NULL;
}

int hmap_add(unsigned char* key, 
		unsigned int key_size, 
		void* val, 
		t_hmap* map)
{
	int index = get_index_from_key(key, key_size, map->cap);
	int ret_msg = EXITO;
	//I'm not vowing to a freaking compiler either, I have been using -> all the time,
	//I'm not starting to use. now.
	if (((map->indices + index)->head != NULL) && (index_list_find_by_key(map->indices + index, key, key_size) != NULL))
		return KEY_IN_USE;
	int push_res = index_list_push(map->indices + index, key, key_size, val);
	int rehash_res;
	if (push_res & FAILURE_CREATING) ret_msg |= push_res;
	if (push_res & REHASH_NEEDED) rehash_res = hmap_rehash(map);
	if (rehash_res & FAILURE_REHASHING) ret_msg |= rehash_res;
	return ret_msg;
}

void hmap_destroy_lists_map(t_hmap* map)
{
	int i;
	struct INDEX_NODE* nodo;
	struct INDEX_LIST* list;
	for(i=0; i<map->cap; i++)
	{
		list = (map->indices + i);
		while(list->head != NULL)
		{
			nodo = index_list_pop(list);
			free(nodo);
		}
	}
	free(map->indices);
	free(map);
}

void hmap_destroy(void (*val_destroyer)(void*), void (*key_destroyer)(void*), t_hmap* map)
{
	int i;
	struct INDEX_NODE* nodo;
	struct INDEX_LIST* list;
	for(i=0; i<map->cap; i++)
	{
		list = (map->indices + i);
		while(list->head != NULL)
		{
			nodo = index_list_pop(list);
			key_destroyer(nodo->key);
			val_destroyer(nodo->val);
			free(nodo);
		}
	}
	free(map->indices);
	free(map);
}
