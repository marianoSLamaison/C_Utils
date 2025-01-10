#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LINKED_LIST
{
	//los nodos necesarios
	struct NODO* first;
	//para simplificar agregar cosas
	struct NODO* last;
	//numero de nodos de la lista
	unsigned int num_nodos;
} t_llist;

t_llist * dllist_create(void);
void llist_add(void* elem, t_llist* list);
void llist_add_at(void* elem, unsigned pos, t_llist* list);
void* llist_get_at(unsigned pos, t_llist* list);
void* llist_remove_at(unsigned pos, t_llist* list);
void llist_simple_destroy(t_llist* list);
void llist_destroy(t_llist* list, void (*elem_destroyer)(void**));

#endif
