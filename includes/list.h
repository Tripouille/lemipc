#ifndef LIST_H
# define LIST_H
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct	s_pos
{
	int		x;
	int		y;
}				t_pos;

typedef struct s_list_element
{
	t_pos					pos;
	struct s_list_element	*prev;
	struct s_list_element	*next;
}				t_list_element;

typedef struct s_list
{
	t_list_element		*head;
	t_list_element		*tail;
	size_t				size;
}				t_list;

/* list.c */
void list_initialize(t_list *list);
t_list_element *list_push(t_list *list, t_pos data);
t_pos list_pop(t_list *list);
t_list_element *list_unshift(t_list *list, t_pos data);
t_pos list_shift(t_list *list);
/* list_utils.c */
t_list_element *create_list_element(t_pos pos, t_list_element *prev,
							t_list_element *next);
t_list_element *list_inject(t_list *list, t_pos pos);
void list_destroy(t_list *list);
bool list_contain(t_list *list, t_pos needle);
bool list_is_empty(t_list *list);
/* list_sort.c */
void list_sort(t_list * list, bool (*comp)(t_pos * a, t_pos * b, t_pos * target),
				t_pos * target);
bool by_dist(t_pos * a, t_pos * b, t_pos * target);


#endif