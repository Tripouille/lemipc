#ifndef NLIST_H
# define NLIST_H
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include "plist.h"

typedef struct		s_node {
	t_pos				pos;
	struct s_node *		parent;
	double				f, g, h;
}					t_node;

typedef struct s_nlist_element {
	t_node					node;
	struct s_nlist_element	*prev;
	struct s_nlist_element	*next;
}				t_nlist_element;

typedef struct s_nlist {
	t_nlist_element		*head;
	t_nlist_element		*tail;
	size_t				size;
}				t_nlist;

/* nlist.c */
void nlist_initialize(t_nlist *nlist);
t_nlist_element *nlist_push(t_nlist *nlist, t_node data);
t_node nlist_pop(t_nlist *nlist);
t_nlist_element *nlist_unshift(t_nlist *nlist, t_node data);
t_node nlist_shift(t_nlist *nlist);
/* nlist_utils.c */
t_nlist_element *create_nlist_element(t_node node, t_nlist_element *prev,
							t_nlist_element *next);
t_nlist_element *nlist_inject(t_nlist *nlist, t_node node);
void nlist_destroy(t_nlist *nlist);
bool nlist_contain(t_nlist *nlist, t_node needle);
bool nlist_is_empty(t_nlist *nlist);
/* nlist_sort.c */
void nlist_sort(t_nlist * nlist, bool (*comp)(t_node * a, t_node * b, t_node * target),
				t_node * target);
bool by_score(t_node * a, t_node * b, t_node * target);


#endif