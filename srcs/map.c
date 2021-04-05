#include "lemipc.h"

void
map_init(void) {
	printf("Map init\n");
	memset(g_ipc.shm, MAP_EMPTY, MAP_Y * MAP_X);
}

int
map_display(void) {
	char * map = g_ipc.shm;
	g_player.pos.x = -1;

	usleep(PLAYER_WARMUP);
	while (1) {
		sem_op(MAP_SEM, -1, 0);
		g_player.is_my_turn = true;
		if (map_is_empty()) {
			printf("Map is empty.\n");
			sem_op(MAP_SEM, 1, 0);
			break;
		}
		printf("Map display: \n");
		for (size_t y = 0; y < MAP_Y; ++y) {
			write(1, map + (y * MAP_X), MAP_X);
			write(1, "\n", 1);
		}
		sem_op(MAP_SEM, 1, 0);
		g_player.is_my_turn = false;
		usleep(DISPLAY_CD);
	}
	return (0);
}

bool
map_is_empty(void) {
	char * map = g_ipc.shm;
	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] != MAP_EMPTY)
			return (false);
	return (true);
}

t_pos
map_get_random_pos(void) {
	t_pos		available_pos[MAP_X * MAP_Y];
	size_t		i_pos = 0;

	srand(time(NULL));
	char *		map = g_ipc.shm;
	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] == MAP_EMPTY)
			available_pos[i_pos++] = (t_pos){p % MAP_X, p / MAP_X};
	if (i_pos == 0)
		return ((t_pos){-1, -1});
	return (available_pos[rand() % i_pos]);
}
