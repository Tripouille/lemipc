#include "list.h"

t_list_element *
create_list_element(t_pos pos, t_list_element *prev,
									t_list_element *next) {
	t_list_element	*new_list_element;

	new_list_element = malloc(sizeof(*new_list_element));
	if (new_list_element == NULL)
		return (NULL);
	new_list_element->pos = pos;
	new_list_element->prev = prev;
	new_list_element->next = next;
	return (new_list_element);
}

t_list_element *
list_inject(t_list *list, t_pos pos) {
	t_list_element	*new_list_element;

	new_list_element = create_list_element(pos, list->tail, list->head);
	if (new_list_element == NULL) {
		list_destroy(list);
		return (NULL);
	}
	if (list->head == NULL) {
		list->head = new_list_element;
		list->tail = new_list_element;
	}
	list->tail->next = new_list_element;
	list->head->prev = new_list_element;
	list->size += 1;
	return (new_list_element);
}

void
list_destroy(t_list *list) {
	t_list_element	*element;
	t_list_element	*element_next;

	element = list->head;
	while (element != list->tail) {
		element_next = element->next;
		free(element);
		element = element_next;
	}
	free(list->tail);
	list_initialize(list);
}

bool
list_contain(t_list *list, t_pos needle) {
	t_list_element	*element;

	if (list->head == NULL)
		return (false);
	element = list->head;
	while (element != list->tail) {
		if (element->pos.x == needle.x && element->pos.y == needle.y)
			return (true);
		element = element->next;
	}
	return (element->pos.x == needle.x && element->pos.y == needle.y);
}

bool
list_is_empty(t_list *list) {
	return (list->head == NULL);
}
