#include "nlist.h"
#include "lemipc.h"

bool
by_score(t_pos * a, t_pos * b, t_pos * target) {
	return (dist(target, a) > dist(target, b));
}

static void
pos_swap(t_pos * a, t_pos * b) {
	t_pos tmp = *a;

	*a = *b;
	*b = tmp;
}

void
nlist_sort(t_nlist * nlist, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
			t_pos * target) {
	t_nlist_element *	element = nlist->head;
	t_nlist_element *	smallest = NULL;

	while (element != nlist->tail) {
		smallest = element;
		for (t_nlist_element	* right = element->next; right != nlist->tail; right = right->next)
			if (comp(&smallest->pos, &right->pos, target))
				smallest = right;
		if (comp(&smallest->pos, &nlist->tail->pos, target))
				smallest = nlist->tail;
		if (element != smallest)
			pos_swap(&element->pos, &smallest->pos);
		element = element->next;
	}
}