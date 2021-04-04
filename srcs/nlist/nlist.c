#include "nlist.h"

void
nlist_initialize(t_nlist *nlist)
{
	nlist->head = NULL;
	nlist->tail = NULL;
	nlist->size = 0;
}

t_nlist_element *
nlist_push(t_nlist *nlist, t_pos pos)
{
	t_nlist_element	*injected_element;

	injected_element = nlist_inject(nlist, pos);
	if (injected_element == NULL)
		return (NULL);
	nlist->tail = injected_element;
	return (injected_element);
}

t_pos
nlist_pop(t_nlist *nlist)
{
	t_nlist_element	*element_to_destroy;
	t_pos			return_pos;

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
	return_pos = element_to_destroy->pos;
	free(element_to_destroy);
	nlist->size -= 1;
	return (return_pos);
}

t_nlist_element *
nlist_unshift(t_nlist *nlist, t_pos pos) {
	t_nlist_element	*injected_element;

	injected_element = nlist_inject(nlist, pos);
	if (injected_element == NULL)
		return (NULL);
	nlist->head = injected_element;
	return (injected_element);
}

t_pos
nlist_shift(t_nlist *nlist) {
	t_nlist_element	*element_to_destroy;
	t_pos			return_pos;

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
	return_pos = element_to_destroy->pos;
	free(element_to_destroy);
	nlist->size -= 1;
	return (return_pos);
}
