#include "examples.h"

void hash_map_example(void)
{
	t_hmap* map = hmap_create();
	char elems[3][8] = {"primero",
			   "segundo",
			   "tercero"};
	long nums[3] = {123345456l, 987654321l, 56889421l};
	hmap_add((unsigned char*)elems[1], (size_t)sizeof(unsigned char) * strlen(elems[1]), nums + 1, map);
	hmap_add((unsigned char*)elems[0], (size_t)sizeof(unsigned char) * strlen(elems[0]), nums + 0, map);
	hmap_add((unsigned char*)elems[2], (size_t)sizeof(unsigned char) * strlen(elems[2]), nums + 2, map);

	printf("El valor al que apunta %s, es %li\n",
		       			elems[1],
					*(long*)hmap_get_value(
						(unsigned char*)elems[1], 
						(size_t)sizeof(unsigned char) * strlen(elems[1]), 
						map));
	hmap_simple_destroy(map);
	printf("El mapa a sido destruido\n");
	printf("El valor de %s, ahora apunta a NULL", elems[1]);
}
