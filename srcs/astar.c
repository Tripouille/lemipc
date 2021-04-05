#include "astar.h"
#include "lemipc.h"

static t_node
create_node(t_pos pos, t_node const * parent) {
	t_node new_node;

	new_node.pos = pos;
	new_node.parent = parent;
	new_node.f = 0;
	new_node.g = 0;
	new_node.h = 0;
	return (new_node);
}

static t_pos
clear_astar(t_nlist * open_nlist, t_nlist * closed_nlist) {
	nlist_destroy(open_nlist);
	nlist_destroy(closed_nlist);
	return ((t_pos){-2, -2});
}

static void
update_node(t_nlist * list, t_node const * new_value) {
	for (t_nlist_element * element = list->head; element != list->tail; element = element->next) {
		if (element->node.pos.x == new_value->pos.x
		&& element->node.pos.y == new_value->pos.y) {
			if (new_value->g < element->node.g)
				element->node = *new_value;
			return ;
		}
	}
}

static t_pos
next_step_from_start(t_node const * actual_node, t_nlist * open_nlist,
					t_nlist * closed_nlist) {
	while (actual_node->parent->parent != NULL)
		actual_node = actual_node->parent;
	t_pos next_pos = actual_node->pos;
	nlist_destroy(open_nlist);
	nlist_destroy(closed_nlist);
	return (next_pos);
}

t_pos
astar(t_pos const *start, t_pos const * end) {
	t_nlist			open_nlist;
	t_nlist			closed_nlist;

	nlist_initialize(&open_nlist);
	nlist_initialize(&closed_nlist);
	if (nlist_push(&open_nlist, create_node(*start, NULL)) == NULL)
		return (clear_astar(&open_nlist, &closed_nlist));
	while (!nlist_is_empty(&open_nlist)) {
		nlist_sort(&open_nlist);
		if (nlist_push(&closed_nlist, nlist_shift(&open_nlist)) == NULL)
			return (clear_astar(&open_nlist, &closed_nlist));
		t_node const * actual_node = &closed_nlist.tail->node;
		if (actual_node->pos.x == end->x && actual_node->pos.y == end->y)
			return (next_step_from_start(actual_node, &open_nlist, &closed_nlist));
		t_node	adjacent_node[4] = {
			create_node((t_pos){actual_node->pos.x, actual_node->pos.y + 1}, actual_node),
			create_node((t_pos){actual_node->pos.x, actual_node->pos.y - 1}, actual_node),
			create_node((t_pos){actual_node->pos.x + 1, actual_node->pos.y}, actual_node),
			create_node((t_pos){actual_node->pos.x - 1, actual_node->pos.y}, actual_node)
		};
		for (int i = 0; i < 4; ++i) {
			if (pos_is_in_map(&adjacent_node[i].pos) && is_empty(&adjacent_node[i].pos)
			&& !nlist_contain(&closed_nlist, adjacent_node[i])) {
				adjacent_node[i].g = actual_node->g + 1;
				adjacent_node[i].h = abs(actual_node->pos.x - end->x) + abs(actual_node->pos.y - end->y);
				adjacent_node[i].f = adjacent_node[i].g + adjacent_node[i].h;
				if (nlist_contain(&open_nlist, adjacent_node[i]))
					update_node(&open_nlist, adjacent_node + i);
				else if (nlist_push(&open_nlist, adjacent_node[i]) == NULL)
					return (clear_astar(&open_nlist, &closed_nlist));
			}
		}
	}
	return ((t_pos){-1, -1});
}