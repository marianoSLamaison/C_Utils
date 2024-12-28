#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include<stdio.h>
struct NODE
{
	struct NODE * next;
	void * dato;
};
typedef struct DOUBLE_LINKED_LIST
{
	//los nodos necesarios
	struct NODE* first;
	//para simplificar agregar cosas
	struct NODE* last;
	//numero de nodos de la lista
	unsigned int num_nodos;
} t_llist;

t_dllist * dllist_create(void);
void llist_add(void* elem, t_llist* list);
void llist_add_at(void* elem, unsigned pos, t_llist* list);
void* llist_get_at(unsigned pos, t_llist* list);
struct NODO* llist_get_nodo_at(unsigned pos, t_llist* list);
void* llist_remove_at(unsigned pos, t_llist* list);
struct NODO* llist_remove_nodo_at(unsigned pos, t_llist* list);
void llist_simple_destroy(t_llist* list);
void llist_destroy(t_llist* list, void (elem_destroyer*)(void**));

#endif
