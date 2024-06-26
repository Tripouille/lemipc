NAME	= lemipc
SRCS	= $(addprefix srcs/, main.c shm.c sem.c msg.c \
							utils.c signal.c map.c play.c \
							play_utils.c astar.c \
							$(addprefix plist/, plist.c plist_utils.c plist_sort.c) \
							$(addprefix nlist/, nlist.c nlist_utils.c nlist_sort.c))
OBJS	= $(SRCS:srcs/%.c=objs/%.o)
DEPS	= $(SRCS:srcs/%.c=deps/%.d)

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -g3 -Iincludes
DFLAGS	= -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $^ -o $(NAME)

clean:
	rm -rf $(OBJS) $(DEPS) *.dSYM

fclean: clean
	rm -rf $(NAME)
	rm -rf *.dSYM

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)