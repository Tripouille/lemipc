#include "lemipc.h"

void
sig_handler(int signal) {
	printf("Catching singnal: %i\n", signal);
	if (g_player.team == DISPLAY)
		printf("Map display abort.\n");
	else {
		sem_op(MAP_SEM, -1, 0);
		printf("Team %c player at pos x %i y %i leave the map.\n",
				g_ipc.shm[pos_to_indice(&g_player.pos)], g_player.pos.x, g_player.pos.y);
		g_ipc.shm[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
		sem_op(MAP_SEM, 1, 0);
	}
	if (map_is_empty()) {
		sem_destroy();
		shm_destroy();
	}
	exit(0);
}