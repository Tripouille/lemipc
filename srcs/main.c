#include "lemipc.h"

t_ipc		g_ipc;
t_player	g_player;

int main(int ac, char ** av) { (void)ac, (void)av;
	if (ac > 2 || (ac == 2 && strlen(av[1]) > 1))
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
	else
		play();
	if (one_team_won()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}
	return (0);
}