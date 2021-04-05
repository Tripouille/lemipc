#include "lemipc.h"

void
sig_handler(int signal) {
	printf("Catching signal: %i\n", signal);
	if (g_player.team == DISPLAY) {
		printf("Map display abort.\n");
		if (g_player.is_my_turn)
			sem_op(MAP_SEM, 1, 0);
	}
	else {
		if (!g_player.is_my_turn)
			sem_op(MAP_SEM, -1, 0);
		printf("Team %c player at pos x %i y %i leave the map.\n",
				g_ipc.shm[pos_to_indice(&g_player.pos)], g_player.pos.x, g_player.pos.y);
		g_ipc.shm[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
		sem_op(MAP_SEM, 1, 0);
	}
	if (one_team_won()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}
	exit(0);
}