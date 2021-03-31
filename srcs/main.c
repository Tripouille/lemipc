#include "lemipc.h"

t_ipc	g_ipc;

int main(int ac, char ** av) {
	signal(SIGINT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGKILL, sig_handler);
	signal(SIGTERM, sig_handler);
	shm_init();
	sem_init();
	if (ac == 1)
		map_display();
	else if (ac == 2 && strlen(av[1]) == 1)
		play(av[1][0]);
	else
		dprintf(2, "Invalid arguments");
	sem_destroy();
	shm_destroy();
	return (0);
}