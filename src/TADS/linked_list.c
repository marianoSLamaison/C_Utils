#include "linked_list.h"

//por lo general el usuario va a querer tener una lista que puede
//pasar de lugar a lugar, o va a tener que modificar la lista
//como su valor, es mas comodo tenerla siempre como un puntero
//que forsarme a pasarla siempre como una referencia
t_dllist * llist_create(void)
{
	t_llist * ret   = (t_llist) malloc(sizeof(t_llist));
	list->num_nodos = 0;
	list->first     = NULL;
	list->last      = NULL;
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
		list->num_nods++;
		return;
	}
	list->last->next = new_nod;
	list->last = new_nod;
	list->num_nods++;
}

void llist_add_at(void* elem, unsigned pos, t_llist* list)
{
	struct NODO * new_nod = malloc(sizeof(struct NODO));
	struct NODO * passer;
	struct NODO * old_passer;
	struct NODO * handler;
	new_nod->elem = elem;
	new_nod->next = NULL;
	unsigned i, num_elems;
	
	//Uninitialized list, keep walking this is an initialized list neighborhood
	if (pos >= list->num_nodos || list->first == NULL) 
	{
		dllist_add(elem, list);
		return;
	}
	
	num_elems = list->num_elems;
	passer = list->first;
	old_passer = new_nod;
	i = 0;
	do {
		if ( i==pos )
		{
			old_passer->next = new_nod;
			new_nod->next = passer;
		}
		handler = passer;
		passer = passer->next;
		old_passer = handler;
		i++;
	}while (i<pos);
	return;
}

void* llist_get_at(unsigned pos, t_llist* list)
{
	struct NODO * passer = list->first;
	unsigned i = 0;
	if ( passer == NULL || pos >= list->num_nodos ) return NULL;
	do {
		if ( i==pos )return passer->elem;	
		passer = passer->next;
		i++;
	}while(i<=pos);
}

static struct NODO* llist_get_nodo_at(unsigned pos, t_llist* list)
{
	struct NODO * passer = list->first;
	unsigned i = 0;
	if ( passer == NULL || pos >= list->num_nodos ) return NULL;
	do {
		if ( i==pos ) break;	
		passer = passer->next;
		i++;
	}while(i<=pos);
	return passer;
}

void* llist_remove_at(unsigned pos, t_llist* list)
{
	struct NODO* passer = list->first;
	struct NODO* handler;
	struct NODO* older = list->first;
	unsigned i=0;
	if ( passer == NULL || pos >= list->num_nodos ) return NULL;
	do {
		if (i==pos)
		{
			older->next = passer->next;
			break;
		}
		i++;
		handler = passer;
		passer = passer->next;
		older = handler;	
	}while(i<=pos);
	return passer->elem;
}
struct NODO* llist_remove_nodo_at(unsigned pos, t_llist* list)
{
	struct NODO* passer = list->first;
	struct NODO* handler;
	struct NODO* older = list->first;
	unsigned i=0;
	if ( passer == NULL || pos >= list->num_nodos ) return NULL;
	do {
		if (i==pos)
		{
			older->next = passer->next;
			break;
		}
		i++;
		handler = passer;
		passer = passer->next;
		older = handler;	
	}while(i<=pos);
	return passer;
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
void llist_destroy(t_llist* list, void (elem_destroyer*)(void**))
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
	void* ret;
	while(passer!=NULL)
	{
		if ( cmp_func(elem, passer) == 0) goto RESULT;
		passer = passer->next;
	}
	return NULL;
	RESULT:
	return passer->elem;
}

long llist_find(t_llist* list, void elem, int(*cmp_func)(void*, void*))
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
	struct NODO* v1;
	struct NODO* v2;
	for(gap=size/2; gap>0; gap/2)
		for(i=gap; i<size; i++)
			for(j=i-gap; j>=0 && cmp_func((v1 = llist_get_nodo_at(list,j)), (v2 = llist_get_nodo_at(list,j+gap)))>0; j-=gap)
			{
				tmp = v1->elem;
				v1->elem = v2->elem;
				v2->elem = tmp;
			};
}
