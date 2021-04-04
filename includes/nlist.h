#ifndef NLIST_H
# define NLIST_H
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include "plist.h"

typedef struct		s_node {
	struct s_node *		parent;
	struct s_node *		next;
	t_pos				pos;
	double				g, h, f;
}					t_node;

typedef struct s_nlist_element {
	t_pos					pos;
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
t_nlist_element *nlist_push(t_nlist *nlist, t_pos data);
t_pos nlist_pop(t_nlist *nlist);
t_nlist_element *nlist_unshift(t_nlist *nlist, t_pos data);
t_pos nlist_shift(t_nlist *nlist);
/* nlist_utils.c */
t_nlist_element *create_nlist_element(t_pos pos, t_nlist_element *prev,
							t_nlist_element *next);
t_nlist_element *nlist_inject(t_nlist *nlist, t_pos pos);
void nlist_destroy(t_nlist *nlist);
bool nlist_contain(t_nlist *nlist, t_pos needle);
bool nlist_is_empty(t_nlist *nlist);
/* nlist_sort.c */
void nlist_sort(t_nlist * nlist, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
				t_pos * target);
bool by_score(t_pos * a, t_pos * b, t_pos * target);


#endif