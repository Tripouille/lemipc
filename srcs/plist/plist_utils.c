#include "plist.h"

t_plist_element *
create_plist_element(t_pos pos, t_plist_element *prev,
									t_plist_element *next) {
	t_plist_element	*new_plist_element;

	new_plist_element = malloc(sizeof(*new_plist_element));
	if (new_plist_element == NULL)
		return (NULL);
	new_plist_element->pos = pos;
	new_plist_element->prev = prev;
	new_plist_element->next = next;
	return (new_plist_element);
}

t_plist_element *
plist_inject(t_plist *plist, t_pos pos) {
	t_plist_element	*new_plist_element;

	new_plist_element = create_plist_element(pos, plist->tail, plist->head);
	if (new_plist_element == NULL) {
		plist_destroy(plist);
		return (NULL);
	}
	if (plist->head == NULL) {
		plist->head = new_plist_element;
		plist->tail = new_plist_element;
	}
	plist->tail->next = new_plist_element;
	plist->head->prev = new_plist_element;
	plist->size += 1;
	return (new_plist_element);
}

void
plist_destroy(t_plist *plist) {
	t_plist_element	*element;
	t_plist_element	*element_next;

	element = plist->head;
	while (element != plist->tail) {
		element_next = element->next;
		free(element);
		element = element_next;
	}
	free(plist->tail);
	plist_initialize(plist);
}

bool
plist_contain(t_plist *plist, t_pos needle) {
	t_plist_element	*element;

	if (plist->head == NULL)
		return (false);
	element = plist->head;
	while (element != plist->tail) {
		if (element->pos.x == needle.x && element->pos.y == needle.y)
			return (true);
		element = element->next;
	}
	return (element->pos.x == needle.x && element->pos.y == needle.y);
}

bool
plist_is_empty(t_plist *plist) {
	return (plist->head == NULL);
}
