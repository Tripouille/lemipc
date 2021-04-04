#include "lemipc.h"

t_ipc		g_ipc;
t_player	g_player;

int main(int ac, char ** av) { (void)ac; (void)av;
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
	else
		dprintf(2, "Invalid arguments");
	if (map_is_empty()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}
	return (0);
}