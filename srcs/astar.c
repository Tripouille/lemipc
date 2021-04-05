#include "astar.h"
#include "lemipc.h"

static t_node
create_node(t_pos pos, t_node *parent) {
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
		t_node actual_node = nlist_shift(&open_nlist);
		if (nlist_push(&closed_nlist, actual_node) == NULL)
			return (clear_astar(&open_nlist, &closed_nlist));

		if (actual_node.pos.x == end->x && actual_node.pos.y == end->y) {
			while (actual_node.parent != NULL)
				actual_node = *actual_node.parent;
			nlist_destroy(&open_nlist);
			nlist_destroy(&closed_nlist);
			return (actual_node.pos);
		}
		t_node	adjacent_node[8] = {
			create_node((t_pos){actual_node.pos.x, actual_node.pos.y + 1}, NULL),
			create_node((t_pos){actual_node.pos.x, actual_node.pos.y - 1}, NULL),
			create_node((t_pos){actual_node.pos.x + 1, actual_node.pos.y}, NULL),
			create_node((t_pos){actual_node.pos.x - 1, actual_node.pos.y}, NULL),
			create_node((t_pos){actual_node.pos.x - 1, actual_node.pos.y - 1}, NULL),
			create_node((t_pos){actual_node.pos.x + 1, actual_node.pos.y - 1}, NULL),
			create_node((t_pos){actual_node.pos.x - 1, actual_node.pos.y + 1}, NULL),
			create_node((t_pos){actual_node.pos.x + 1, actual_node.pos.y + 1}, NULL)
		};
		for (int i = 0; i < 8; ++i) {
			if (pos_is_in_map(&adjacent_node[i].pos) && is_empty(&adjacent_node[i].pos)
			&& !nlist_contain(&closed_nlist, adjacent_node[i])) {
				
			}
	}
	return ((t_pos){-1, -1});
}