#ifndef plist_H
# define plist_H
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct	s_pos {
	int		x;
	int		y;
}				t_pos;

typedef struct s_plist_element {
	t_pos					pos;
	struct s_plist_element	*prev;
	struct s_plist_element	*next;
}				t_plist_element;

typedef struct s_plist {
	t_plist_element		*head;
	t_plist_element		*tail;
	size_t				size;
}				t_plist;

/* plist.c */
void plist_initialize(t_plist *plist);
t_plist_element *plist_push(t_plist *plist, t_pos data);
t_pos plist_pop(t_plist *plist);
t_plist_element *plist_unshift(t_plist *plist, t_pos data);
t_pos plist_shift(t_plist *plist);
/* plist_utils.c */
t_plist_element *create_plist_element(t_pos pos, t_plist_element *prev,
							t_plist_element *next);
t_plist_element *plist_inject(t_plist *plist, t_pos pos);
void plist_destroy(t_plist *plist);
bool plist_contain(t_plist *plist, t_pos needle);
bool plist_is_empty(t_plist *plist);
/* plist_sort.c */
void plist_sort(t_plist * plist, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
				t_pos * target);
bool by_dist(t_pos * a, t_pos * b, t_pos * target);


#endif