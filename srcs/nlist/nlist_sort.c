#include "nlist.h"
#include "lemipc.h"

bool
by_score(t_node * a, t_node * b, t_node * target) {
	return (dist(&target->pos, &a->pos) > dist(&target->pos, &b->pos));
}

static void
node_swap(t_node * a, t_node * b) {
	t_node tmp = *a;

	*a = *b;
	*b = tmp;
}

void
nlist_sort(t_nlist * nlist, bool (*comp)(t_node * a, t_node * b, t_node * target),
			t_node * target) {
	t_nlist_element *	element = nlist->head;
	t_nlist_element *	smallest = NULL;

	while (element != nlist->tail) {
		smallest = element;
		for (t_nlist_element	* right = element->next; right != nlist->tail; right = right->next)
			if (comp(&smallest->node, &right->node, target))
				smallest = right;
		if (comp(&smallest->node, &nlist->tail->node, target))
				smallest = nlist->tail;
		if (element != smallest)
			node_swap(&element->node, &smallest->node);
		element = element->next;
	}
}