#include "lemipc.h"

void
map_init(void) {
	printf("Map init\n");
	memset(g_ipc.shm, MAP_EMPTY, MAP_Y * MAP_X);
}

int map_display(void) {
	char * map = g_ipc.shm;

	while (1) {
		printf("Map display: \n");
		sem_op(MAP_SEM, -1, 0);
		for (size_t y = 0; y < MAP_Y; ++y) {
			write(1, map + (y * MAP_X), MAP_X);
			write(1, "\n", 1);
		}
		sem_op(MAP_SEM, 1, 0);
		sleep(1);
	}
	return (0);
}

t_pos
map_get_random_pos(void) {
	t_pos		available_pos[MAP_X * MAP_Y];
	size_t		i_pos = 0;

	srand(time(NULL));
	sem_op(MAP_SEM, -1, 0);
	char *		map = g_ipc.shm;
	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
			if (map[p] == MAP_EMPTY)
				available_pos[i_pos++] = (t_pos){p % MAP_X, p / MAP_X};
	sem_op(MAP_SEM, 1, 0);
	if (i_pos == 0)
		return ((t_pos){-1, -1});
	return (available_pos[rand() % i_pos]);
}
