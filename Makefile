NAME	= lemipc
SRCS	= $(addprefix srcs/, main.c shm.c sem.c msg.c \
							utils.c signal.c map.c play.c \
							play_utils.c \
							$(addprefix list/, list.c list_utils.c list_sort.c))
OBJS	= $(SRCS:srcs/%.c=objs/%.o)
DEPS	= $(SRCS:srcs/%.c=deps/%.d)

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -g3 -Iincludes
DFLAGS	= -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $^ -lm -o $(NAME)

clean:
	rm -rf $(OBJS) $(DEPS) *.dSYM

fclean: clean
	rm -rf $(LINK) $(NAME)
	rm -rf *.dSYM

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)