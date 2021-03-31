#include "lemipc.h"

static bool
team_won(char team) {
	char * map = g_ipc.shm;

	sem_op(MAP_SEM, -1, 0);
	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] != MAP_EMPTY && map[p] != team) {
			sem_op(MAP_SEM, 1, 0);
			return (false);
		}
	sem_op(MAP_SEM, 1, 0);
	return (true);
}

static void
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
	sleep(1);
	while (!team_won(team)) {
		printf("my team is %c\n", team);
		sleep(5);
	}
	printf("Team %c WON!\n", team);
}