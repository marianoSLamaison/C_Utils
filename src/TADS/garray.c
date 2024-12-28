#include "array.h"

t_garray* garray_create(unsigned elem_size)
{
	t_array* ret = malloc(sizeof(t_array));
	ret->capacity  = 0x10;
	ret->elems_num = 0x00;
	ret->elem_size = elem_size;
	ret->elems     = malloc(elem_size * ret->capacity);
	memset(ret->elems, 0x00, elem_size * ret->capacity);
	return ret;
}

void garray_add(t_array* array, void* elem)
{
	//para poder realizar aritmetica, si no simplemente no puedo
	//confio en que char siempre es el valor mas pequeño imaginable
	if ( array->capacity == array->elems_num ) array_expand(&array->elems, (unsigned)(array->capacity * 1.25f));
	unsigned char* elems = (unsigned char*)array->elems;
	static unsigned i;
	for (i=0; i<array->elem_size; i++)
		*(elems+array->elems_num) = *((unsigned char*) elem + i);
}
