#include "astar.h"

static t_node *
node_create(t_pos * pos, t_node * parent) {
	t_node * new_node = malloc(sizeof(t_node));
	if (new_node == NULL)
		return (NULL);
	new_node->pos = *pos;
	new_node->parent = parent;
	new_node->next = NULL;
	new_node->f = 0;
	new_node->g = 0;
	new_node->h = 0;
	return (new_node);
}

static t_node *
node_push(t_node ** node, t_pos * pos, t_node * parent) {
	while (node != NULL)
		node = &(*node)->next;
	*node = node_create(pos, parent);
	return (*node);
}

t_node *
astar(t_pos * start, t_pos * end) {
	t_node * path = NULL;
	t_node * open_plist = NULL;
	t_node * close_plist = NULL;

	node_push(&open_plist, start, NULL);
	while (open_plist != NULL) {
		
	}
}