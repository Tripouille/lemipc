#include "lemipc.h"

void
play(char team) {
	t_pos	pos = map_get_random_pos();

	if (pos.x == -1)
		perror_exit("Map already full.");
	
	char *		map = g_ipc.shm;
	map[pos.y * MAP_X + pos.x] = team;
	while (1) {
		printf("my team is %c\n", team);
		sleep(5);
	}
}