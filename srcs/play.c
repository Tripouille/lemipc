#include "lemipc.h"

static size_t
is_enemmy(char slot, char team) {
	return (slot != team && slot != MAP_EMPTY);
}

static size_t
pos_is_in_map(t_pos * pos) {
	return (pos->x >= 0 && pos->x < MAP_X
	&& pos->y >= 0 && pos->y < MAP_Y);
}

static size_t
pos_to_indice(t_pos * pos) {
	return (pos->y * MAP_X + pos->x);
}

static bool
team_won(char team) {
	char * map = g_ipc.shm;

	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] != MAP_EMPTY && map[p] != team)
			return (false);
	return (true);
}

static void
init_pos(t_pos * pos, char team) {
	char *		map = g_ipc.shm;

	sem_op(MAP_SEM, -1, 0);
	if ((*pos = map_get_random_pos()).x == -1)
		error_exit("Map already full.\n");
	map[pos_to_indice(pos)] = team;
	sem_op(MAP_SEM, 1, 0);
}

static t_pos
scan(t_pos *pos, char team, char * map, int range) {
	t_pos	start = {pos->x - range, pos->y - range};

	for (; start.x < pos->x + range; ++start.x)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)], team))
			return (start);
	for (; start.y < pos->y + range; ++start.y)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)], team))
			return (start);
	for (; start.x > pos->x - range; --start.x)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)], team))
			return (start);
	for (; start.y > pos->y - range; --start.y)
		if (pos_is_in_map(&start) && is_enemmy(map[pos_to_indice(&start)], team))
			return (start);
	return ((t_pos){-1, -1});
}

void
move(t_pos *pos, char team) {
	char *		map = g_ipc.shm;
	int			range = 1;
	t_pos		enemy_pos;

	while (range < MAP_Y + 1
	&& (enemy_pos = scan(pos, team, map, range)).x == -1) // a revoir
		++range;
	if (enemy_pos.x == -1)
		printf("team: %c no enemy detected.\n", team);
	else
		printf("team: %c enemy detected at pos x: %i, y: %i\n", team, enemy_pos.x, enemy_pos.y);
}

void
play(char team) {
	char *		map = g_ipc.shm;
	t_pos		pos;
	
	init_pos(&pos, team); sleep(3);

	while (1) {
		sem_op(MAP_SEM, -1, 0);
		move(&pos, team);
		if (team_won(team)) {
			map[pos_to_indice(&pos)] = MAP_EMPTY;
			sem_op(MAP_SEM, 1, 0);
			break;
		}
		sem_op(MAP_SEM, 1, 0);
		sleep(5);
	}
	printf("Team %c WON!\n", team);
}