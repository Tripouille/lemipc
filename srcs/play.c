#include "lemipc.h"

static bool
is_enemmy(t_pos * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value!= g_player.team && map_value != MAP_EMPTY);
}

static bool
is_ally(t_pos * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value == g_player.team
	&& !(pos->x == g_player.pos.x && pos->y == g_player.pos.y));
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
scan(t_pos * pos, int max_range, bool (*is_target)(t_pos *)) {
	for (int range = 1; range <= max_range; ++range) {
		t_pos	start = {pos->x - range, pos->y - range};
		for (; start.x < pos->x + range; ++start.x)
			if (pos_is_in_map(&start) && is_target(&start))
				return (start);
		for (; start.y < pos->y + range; ++start.y)
			if (pos_is_in_map(&start) && is_target(&start))
				return (start);
		for (; start.x > pos->x - range; --start.x)
			if (pos_is_in_map(&start) && is_target(&start))
				return (start);
		for (; start.y > pos->y - range; --start.y)
			if (pos_is_in_map(&start) && is_target(&start))
				return (start);
	}
	return ((t_pos){-1, -1});
}

void
move(void) {
	t_pos		enemy_pos = scan(&g_player.pos, max(MAP_Y, MAP_X), is_enemmy);

	printf("Player x %i y %i scanning for enemy !\n", g_player.pos.x, g_player.pos.y);
	if (enemy_pos.x == -1)
		printf("Scan detect no enemy.\n");
	else {
		printf("Scan detect enemy at pos x: %i, y: %i\n", enemy_pos.x, enemy_pos.y);
		printf("Scanning for ally !\n");
		t_pos		ally_pos = scan(&enemy_pos, max(MAP_Y, MAP_X), is_ally);
		if (ally_pos.x == -1)
			printf("no ally detected.\n");
		else {
			printf("Scan detect ally at pos x: %i, y: %i\n", ally_pos.x, ally_pos.y);
			t_msg	msg;
			((int*)(&msg.type))[0] = ally_pos.x;
			((int*)(&msg.type))[1] = ally_pos.y;
			((int*)(msg.s))[0] = g_player.pos.x;
			((int*)(msg.s))[1] = g_player.pos.y;
			msg_send(&msg);
		}
	}
}

void
play(void) {
	char *		map = g_ipc.shm;
	
	init_pos(); sleep(3);
	while (1) {
		sem_op(MAP_SEM, -1, 0);
		move();
		if (msg_receive()) {
			printf("Message receive: x %i y %i\n",((int*)g_player.msg)[2],  ((int*)g_player.msg)[3]);
		}
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