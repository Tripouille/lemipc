#include "list.h"
#include "lemipc.h"

bool
by_dist(t_pos * a, t_pos * b, t_pos * target) {
	return (dist(target, a) > dist(target, b));
}

static void
pos_swap(t_pos * a, t_pos * b) {
	t_pos tmp = *a;

	*a = *b;
	*b = tmp;
}

void
list_sort(t_list * list, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
			t_pos * target) {
	t_list_element *	element = list->head;
	t_list_element *	smallest = NULL;

	while (element != list->tail) {
		smallest = element;
		for (t_list_element	* right = element->next; right != list->tail; right = right->next)
			if (comp(&smallest->pos, &right->pos, target))
				smallest = right;
		if (comp(&smallest->pos, &list->tail->pos, target))
				smallest = list->tail;
		if (element != smallest)
			pos_swap(&element->pos, &smallest->pos);
		element = element->next;
	}
}