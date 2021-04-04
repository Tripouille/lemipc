#include "plist.h"
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
plist_sort(t_plist * plist, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
			t_pos * target) {
	t_plist_element *	element = plist->head;
	t_plist_element *	smallest = NULL;

	while (element != plist->tail) {
		smallest = element;
		for (t_plist_element	* right = element->next; right != plist->tail; right = right->next)
			if (comp(&smallest->pos, &right->pos, target))
				smallest = right;
		if (comp(&smallest->pos, &plist->tail->pos, target))
				smallest = plist->tail;
		if (element != smallest)
			pos_swap(&element->pos, &smallest->pos);
		element = element->next;
	}
}