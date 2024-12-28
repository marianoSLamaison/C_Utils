#ifndef HASH_MAP_H
#define HASH_MAP_H
typedef struct H_MAP_SUB_INDEX
{
	unsigned char* key;
	void* val;
}t_sindex;

typedef struct H_MAP_INDEX
{
	t_sindex* indices;
	int elem_c, cap;
}t_index;
//NOTA: quiero que sea de referencia cerrada me parece mas justo
typedef struct HASH_MAP
{
	int cap, bussy_indexes;
	t_index * indices;
}t_hmap;
#endif
