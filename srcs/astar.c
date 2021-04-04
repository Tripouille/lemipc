#include "astar.h"

static t_node
create_node(t_pos *pos, t_node *parent)
{
	t_node new_node;

	new_node.pos = *pos;
	new_node.parent = parent;
	new_node.f = 0;
	new_node.g = 0;
	new_node.h = 0;
	return (new_node);
}

t_node *
astar(t_pos *start, t_pos *end)
{
	t_node path_nlist;
	t_node open_nlist;
	t_node close_nlist;

	if (nlist_push(&open_nlist, create_node(start, NULL)) == NULL)
		return (NULL);
	while (!nlist_is_empty(&open_nlist))
	{
	}
}