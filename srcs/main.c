#include "lemipc.h"

t_ipc		g_ipc;
t_player	g_player;

int main(int ac, char ** av) { (void)ac, (void)av;
	shm_init();
	t_pos start = {0, 0};
	t_pos end = {4, 4};
	t_pos next_step = astar(&start, &end);
	printf("Next Step x %i y %i\n", next_step.x, next_step.y);
	/*
	if (ac == 2 && strlen(av[1]) > 1)
		error_exit("Invalid arguments");
	signal(SIGINT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGTERM, sig_handler);
	g_player.team = ac == 1 ? DISPLAY : av[1][0];
	shm_init();
	sem_init();
	msg_init();
	if (ac == 1)
		map_display();
	else if (ac == 2 && strlen(av[1]) == 1)
		play();
	if (map_is_empty()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}*/
	return (0);
}