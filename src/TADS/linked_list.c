#include "linked_list.h"

struct NODO
{
	struct NODO * next;
	void * elem;
};

//por lo general el usuario va a querer tener una lista que puede
//pasar de lugar a lugar, o va a tener que modificar la lista
//como su valor, es mas comodo tenerla siempre como un puntero
//que forsarme a pasarla siempre como una referencia
t_llist * llist_create(void)
{
	t_llist * ret   = (t_llist*) malloc(sizeof(t_llist));
	ret->num_nodos = 0;
	ret->first     = NULL;
	ret->last      = NULL;
	return ret;
}
//carga un elemento al final de la lista
void llist_add(void* elem, t_llist* list)
{
	struct NODO * new_nod = malloc(sizeof(struct NODO));
	new_nod->elem = elem;
	new_nod->next = NULL;
	
	if ( list->first == NULL )
	{
		list->first = new_nod;
		list->last = list->first;
		list->num_nodos++;
		return;
	}
	list->last->next = new_nod;
	list->last = new_nod;
	list->num_nodos++;
}

void llist_add_at(void* elem, unsigned pos, t_llist* list)
{
	struct NODO** passer = &list->first;
	struct NODO* new_nod;
	unsigned i=0;
	while(*passer!=NULL)
	{
		if(i==pos)
		{
			new_nod = malloc(sizeof(struct NODO));
			new_nod->next = *passer;
			*passer = new_nod;
			new_nod->elem = elem;
			list->num_nodos++;
			break;
		}
		passer=&(*passer)->next;
		i++;
	}
}

void* llist_get_at(unsigned pos, t_llist* list)
{
	struct NODO * passer = list->first;
	unsigned i = 0;
	for(; i<pos && passer!=NULL; i++)
	{
		if(i==pos) return passer->elem;
		passer = passer->next;
	}
	return passer;//si llegamos aqui sin nada, es que passer es igual a NULL
}

static struct NODO* llist_get_nodo_at(unsigned pos, t_llist* list)
{
	struct NODO * passer = list->first;
	unsigned i = 0;
	for(; i<pos && passer!=NULL; i++)
	{
		if (i == pos) return passer;
		passer = passer->next;
	}
	return passer;
}

void* llist_remove_at(unsigned pos, t_llist* list)
{
	struct NODO** passer = &list->first;
	struct NODO* handle = NULL;
	void* ret = NULL;
	unsigned i=0;
	for(; i<pos && (*passer)!=NULL; i++)
	{
		if (i==pos)
		{//ahora mismo estoy apuntando, al "Next" del puntero anterior
		//solo tengo que 
		//domino bien punteros simples, pero los dobles todavia me confunden
			handle = *passer;
			*passer = handle->next;//actualizamos el puntero al siguiente para no romper la cadena
			ret = handle->elem;
			free(handle);
			return ret;
		}
		passer = &(*passer)->next;
		i++;
	}
	return NULL;
}
struct NODO* llist_remove_nodo_at(unsigned pos, t_llist* list)
{
	struct NODO** passer = &list->first;
	struct NODO* handle = NULL;
	unsigned i=0;
	for(; i<pos && (*passer)!=NULL; i++)
	{
		passer = &(*passer)->next;
		if (i==pos)
		{
			handle = *passer;
			*passer = handle->next;
			return handle;
		}
	}
	return NULL;
}
void llist_simple_destroy(t_llist* list)
{
	struct NODO* passer = list->first;
	struct NODO* older;
	while ( passer != NULL)
	{
		free(passer->elem);
		older = passer;
		passer = passer->next;
		free(older);
	}
}
void llist_destroy(t_llist* list, void (*elem_destroyer)(void**))
{
	struct NODO* passer = list->first;
	struct NODO* older;
	while ( passer != NULL)
	{
		elem_destroyer(&passer->elem);
		older = passer;
		passer = passer->next;
		free(older);
	}
}

t_llist* llist_map(t_llist* list, void* (*convert)(void*))
{
	t_llist* ret = llist_create();
	struct NODO* passer = list->first;
	void* new_elem;
	while( passer != NULL )
	{
		new_elem = convert(passer->elem);
		llist_add(new_elem, ret);
		
		passer = passer->next;
	}
	return ret;
}

void* llist_find_get(t_llist* list, void* elem, int (*cmp_func)(void*, void*))
{
	struct NODO* passer = list->first;
	while(passer!=NULL)
	{
		if ( cmp_func(elem, passer) == 0) goto RESULT;
		passer = passer->next;
	}
	return NULL;
	RESULT:
	return passer->elem;
}

long llist_find(t_llist* list, void* elem, int(*cmp_func)(void*, void*))
{
	struct NODO* passer = list->first;
	long ret=0;
	while(passer!=NULL)
	{
		if(cmp_func(elem, passer) == 0) goto RESULT;
		passer = passer->next;
		ret++;
	}
	return -1;
	RESULT:
	return ret;
}

//sospecho esto no sera tan veloz como con un array, por las get
//usamos shellsort por que me gusta
void llist_sort(t_llist* list, int(*cmp_func)(void*, void*))
{
	unsigned gap, i, j, size = list->num_nodos;
	void* tmp;
	struct NODO * v1;
	struct NODO * v2;
	for(gap=size/2; gap>0; gap/=2)
		for(i=gap; i<size; i++)
			for(j=i-gap; j>=0 && cmp_func((v1 = llist_get_nodo_at(j,list)), (v2 = llist_get_nodo_at(j+gap,list)))>0; j-=gap)
			{
				tmp = v1->elem;
				v1->elem = v2->elem;
				v2->elem = tmp;
			};
}
