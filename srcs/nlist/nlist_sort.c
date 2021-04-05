#include "nlist.h"
#include "lemipc.h"

static bool
comp(t_node * a, t_node * b) {
	return (a->f > b->f);
}

static void
node_swap(t_node * a, t_node * b) {
	t_node tmp = *a;

	*a = *b;
	*b = tmp;
}

void
nlist_sort(t_nlist * nlist) {
	t_nlist_element *	element = nlist->head;
	t_nlist_element *	smallest = NULL;

	while (element != nlist->tail) {
		smallest = element;
		for (t_nlist_element	* right = element->next; right != nlist->tail; right = right->next)
			if (comp(&smallest->node, &right->node))
				smallest = right;
		if (comp(&smallest->node, &nlist->tail->node))
				smallest = nlist->tail;
		if (element != smallest)
			node_swap(&element->node, &smallest->node);
		element = element->next;
	}
}