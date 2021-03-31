#include "lemipc.h"

void
init_pos(t_pos * pos, char team) {
	char *		map = g_ipc.shm;

	sem_op(MAP_SEM, -1, 0);
	if ((*pos = map_get_random_pos()).x == -1)
		error_exit("Map already full.\n");
	map[pos->y * MAP_X + pos->x] = team;
	sem_op(MAP_SEM, 1, 0);
}

void
play(char team) {
	t_pos	pos;
	
	init_pos(&pos, team);
	while (1) {
		printf("my team is %c\n", team);
		sleep(5);
	}
}