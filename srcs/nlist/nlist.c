#include "nlist.h"

void
nlist_initialize(t_nlist *nlist)
{
	nlist->head = NULL;
	nlist->tail = NULL;
	nlist->size = 0;
}

t_nlist_element *
nlist_push(t_nlist *nlist, t_node node)
{
	t_nlist_element	*injected_element;

	injected_element = nlist_inject(nlist, node);
	if (injected_element == NULL)
		return (NULL);
	nlist->tail = injected_element;
	return (injected_element);
}

t_node
nlist_pop(t_nlist *nlist)
{
	t_nlist_element	*element_to_destroy;
	t_node			return_node;

	element_to_destroy = nlist->tail;
	if (nlist->head == nlist->tail) {
		nlist->head = NULL;
		nlist->tail = NULL;
	}
	else {
		nlist->tail->prev->next = nlist->tail->next;
		nlist->tail->next->prev = nlist->tail->prev;
		nlist->tail = nlist->tail->prev;
	}
	return_node = element_to_destroy->node;
	free(element_to_destroy);
	nlist->size -= 1;
	return (return_node);
}

t_nlist_element *
nlist_unshift(t_nlist *nlist, t_node node) {
	t_nlist_element	*injected_element;

	injected_element = nlist_inject(nlist, node);
	if (injected_element == NULL)
		return (NULL);
	nlist->head = injected_element;
	return (injected_element);
}

t_node
nlist_shift(t_nlist *nlist) {
	t_nlist_element	*element_to_destroy;
	t_node			return_node;

	element_to_destroy = nlist->head;
	if (nlist->head == nlist->tail) {
		nlist->head = NULL;
		nlist->tail = NULL;
	}
	else {
		nlist->head->next->prev = nlist->head->prev;
		nlist->head->prev->next = nlist->head->next;
		nlist->head = nlist->head->next;
	}
	return_node = element_to_destroy->node;
	free(element_to_destroy);
	nlist->size -= 1;
	return (return_node);
}
