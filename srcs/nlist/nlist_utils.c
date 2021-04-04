#include "nlist.h"

t_nlist_element *
create_nlist_element(t_pos pos, t_nlist_element *prev,
									t_nlist_element *next) {
	t_nlist_element	*new_nlist_element;

	new_nlist_element = malloc(sizeof(*new_nlist_element));
	if (new_nlist_element == NULL)
		return (NULL);
	new_nlist_element->pos = pos;
	new_nlist_element->prev = prev;
	new_nlist_element->next = next;
	return (new_nlist_element);
}

t_nlist_element *
nlist_inject(t_nlist *nlist, t_pos pos) {
	t_nlist_element	*new_nlist_element;

	new_nlist_element = create_nlist_element(pos, nlist->tail, nlist->head);
	if (new_nlist_element == NULL) {
		nlist_destroy(nlist);
		return (NULL);
	}
	if (nlist->head == NULL) {
		nlist->head = new_nlist_element;
		nlist->tail = new_nlist_element;
	}
	nlist->tail->next = new_nlist_element;
	nlist->head->prev = new_nlist_element;
	nlist->size += 1;
	return (new_nlist_element);
}

void
nlist_destroy(t_nlist *nlist) {
	t_nlist_element	*element;
	t_nlist_element	*element_next;

	element = nlist->head;
	while (element != nlist->tail) {
		element_next = element->next;
		free(element);
		element = element_next;
	}
	free(nlist->tail);
	nlist_initialize(nlist);
}

bool
nlist_contain(t_nlist *nlist, t_pos needle) {
	t_nlist_element	*element;

	if (nlist->head == NULL)
		return (false);
	element = nlist->head;
	while (element != nlist->tail) {
		if (element->pos.x == needle.x && element->pos.y == needle.y)
			return (true);
		element = element->next;
	}
	return (element->pos.x == needle.x && element->pos.y == needle.y);
}

bool
nlist_is_empty(t_nlist *nlist) {
	return (nlist->head == NULL);
}
