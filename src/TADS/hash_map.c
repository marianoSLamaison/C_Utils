#include "hash_map.h"

t_hmap* hmap_create(void)
{
	t_hmap* ret = malloc(sizeof(t_hmap));
	//128 elementos de base deberia ser suficiente, calculo.
	ret->cap = 0xEF;//todos los hmaps tienen minimo 16 espaios libres
	ret->elem_c = 0;
	ret->values = malloc(sizeof(t_hmapi) * ret->cap);
	int i,j;
	t_index* indice;
	for(i=0; i<ret->cap; i++)
	{
		indice = &ret->indices[i];
		indice->cap = 0x10;
		indice->bussy_indexes = 0;
		ret->indices = malloc(sizeof(t_sindex)*0x10);
		for ( j=0; j<0x10; j++) 
		{
			ret->indices[i].key=NULL;
			ret->indices[i].val == NULL;
		}
	}
	return ret;
}
//why this way? because why not?
//the only thing bothering me was conmutatibility onestly
#define g(n) (((n-n%0x08)<<0x0C)*3 + n*n)

static unsigned long hash_func(void* _key, int k_length)
{
	unsigned char* key = (signed char*)_key;
	unsigned char n;
	unsigned long hash = 0;
	int i;
	
	for(i=0; i<k_length; i++)
	{
		hash += 1<<(n & 0x07);//Fuck conmutatibility, all my homies hate 
		//conmutatibility
		n = key[i];
		hash += g(n);
	}
	return hash;
}

static int get_index_from_key(signed char* key, t_hmap* map)
{
	long ret;
	int num_cs = strlen(key) - 1;
	unsigned long hash = hash_func(key,num_cs);
	return hash % map->elem_c;
}

void hmap_add(signed char* key, void* val, t_hmap* map)
{
	//no voy a ponerme a rehashear eternamente por un "tal vez" de mejor eficiencia
	//si hay mas de un item, toca chequear linialmente para eso confiamos en que la funcion
	//tenga una chance baja de dar repetidos
	t_index* indice = map->indices[get_index_from_key(key, map)];
	if ( index->elem_c == 0 )
	{
		index->indices[0].key = key;
		index->indices[0].val = val;
		index->elem_c=1;
		//incrementamos el numeor de indices ocupados
		//si llega a cierta marca
		map->bussy_indexes++;
		return;
	}
	if ( index->elem_c < 
	
	
}
