#include "lemipc.h"

static size_t
is_enemmy(char slot) {
	return (slot != g_player.team && slot != MAP_EMPTY);
}

size_t
pos_is_in_map(t_pos * pos) {
	return (pos->x >= 0 && pos->x < MAP_X
	&& pos->y >= 0 && pos->y < MAP_Y);
}

size_t
pos_to_indice(t_pos * pos) {
	return (pos->y * MAP_X + pos->x);
}

static bool
team_won(void) {
	char * map = g_ipc.shm;

	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] != MAP_EMPTY && map[p] != g_player.team)
			return (false);
	return (true);
}

static void
init_pos(void) {
	char *		map = g_ipc.shm;

	sem_op(MAP_SEM, -1, 0);
	if ((g_player.pos = map_get_random_pos()).x == -1)
		error_exit("Map already full.\n");
	map[pos_to_indice(&g_player.pos)] = g_player.team;
	sem_op(MAP_SEM, 1, 0);
}

static t_pos
scan(int range) {
	char *	map = g_ipc.shm;
	t_pos	start = {g_player.pos.x - range, g_player.pos.y - range};

	for (; start.x < g_player.pos.x + range; ++start.x)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)]))
			return (start);
	for (; start.y < g_player.pos.y + range; ++start.y)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)]))
			return (start);
	for (; start.x > g_player.pos.x - range; --start.x)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)]))
			return (start);
	for (; start.y > g_player.pos.y - range; --start.y)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)]))
			return (start);
	return ((t_pos){-1, -1});
}

void
move(void) {
	int			range = 1;
	int			max_range = max(MAP_Y, MAP_X);
	t_pos		enemy_pos;


	while (range <= max_range
	&& (enemy_pos = scan(range)).x == -1)
		++range;
	if (enemy_pos.x == -1)
		printf("team: %c no enemy detected.\n", g_player.team);
	else
		printf("team: %c enemy detected at pos x: %i, y: %i\n", g_player.team, enemy_pos.x, enemy_pos.y);
}

void
play(void) {
	char *		map = g_ipc.shm;
	
	init_pos(); sleep(3);
	while (1) {
		sem_op(MAP_SEM, -1, 0);
		move();
		if (team_won()) {
			map[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
			sem_op(MAP_SEM, 1, 0);
			break;
		}
		sem_op(MAP_SEM, 1, 0);
		sleep(5);
	}
	printf("Team %c WON!\n", g_player.team);
}