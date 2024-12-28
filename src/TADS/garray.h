#ifndef GARRAY_H
#define GARRAT_H
typedef struct GENERIC_ARRAY
{
	unsigned capacity, elems_num, elem_size;
	void* elems;
}t_garray;
#endif
