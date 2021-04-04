#ifndef ASTAR_H
# define ASTAR_H
# include "plist.h"

typedef struct		s_node {
	struct s_node *		parent;
	struct s_node *		next;
	t_pos				pos;
	double				g, h, f;
}					t_node;

t_node * astar(t_pos * start, t_pos * end);

#endif