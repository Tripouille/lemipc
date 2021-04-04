#include "plist.h"

void
plist_initialize(t_plist *plist)
{
	plist->head = NULL;
	plist->tail = NULL;
	plist->size = 0;
}

t_plist_element *
plist_push(t_plist *plist, t_pos pos)
{
	t_plist_element	*injected_element;

	injected_element = plist_inject(plist, pos);
	if (injected_element == NULL)
		return (NULL);
	plist->tail = injected_element;
	return (injected_element);
}

t_pos
plist_pop(t_plist *plist)
{
	t_plist_element	*element_to_destroy;
	t_pos			return_pos;

	element_to_destroy = plist->tail;
	if (plist->head == plist->tail) {
		plist->head = NULL;
		plist->tail = NULL;
	}
	else {
		plist->tail->prev->next = plist->tail->next;
		plist->tail->next->prev = plist->tail->prev;
		plist->tail = plist->tail->prev;
	}
	return_pos = element_to_destroy->pos;
	free(element_to_destroy);
	plist->size -= 1;
	return (return_pos);
}

t_plist_element *
plist_unshift(t_plist *plist, t_pos pos) {
	t_plist_element	*injected_element;

	injected_element = plist_inject(plist, pos);
	if (injected_element == NULL)
		return (NULL);
	plist->head = injected_element;
	return (injected_element);
}

t_pos
plist_shift(t_plist *plist) {
	t_plist_element	*element_to_destroy;
	t_pos			return_pos;

	element_to_destroy = plist->head;
	if (plist->head == plist->tail) {
		plist->head = NULL;
		plist->tail = NULL;
	}
	else {
		plist->head->next->prev = plist->head->prev;
		plist->head->prev->next = plist->head->next;
		plist->head = plist->head->next;
	}
	return_pos = element_to_destroy->pos;
	free(element_to_destroy);
	plist->size -= 1;
	return (return_pos);
}
