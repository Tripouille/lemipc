#include "lemipc.h"

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
scan(t_pos * pos, int max_range, bool (*is_valid_target)(t_pos *)) {
	for (int range = 1; range <= max_range; ++range) {
		t_pos	start = {pos->x - range, pos->y - range};
		for (; start.x < pos->x + range; ++start.x)
			if (pos_is_in_map(&start) && is_valid_target(&start))
				return (start);
		for (; start.y < pos->y + range; ++start.y)
			if (pos_is_in_map(&start) && is_valid_target(&start))
				return (start);
		for (; start.x > pos->x - range; --start.x)
			if (pos_is_in_map(&start) && is_valid_target(&start))
				return (start);
		for (; start.y > pos->y - range; --start.y)
			if (pos_is_in_map(&start) && is_valid_target(&start))
				return (start);
	}
	return ((t_pos){-1, -1});
}

static void
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
			t_pos closest_pos = closest_available_pos(&ally_pos, &enemy_pos);
			printf("closest_pos for ally x: %i, y: %i\n", closest_pos.x, closest_pos.y);
			t_msg	msg;
			((int*)(&msg.type))[0] = ally_pos.x;
			((int*)(&msg.type))[1] = ally_pos.y;
			((int*)(msg.s))[0] = closest_pos.x;
			((int*)(msg.s))[1] = closest_pos.y;
			msg_send(&msg);
		}
	}
}

void
play(void) {
	char *		map = g_ipc.shm;
	
	init_pos(); sleep(PLAYER_WARMUP);
	while (1) {
		sem_op(MAP_SEM, -1, 0);
		if (msg_receive()) {
			printf("Message receive: x %i y %i\n",((int*)g_player.msg)[2],  ((int*)g_player.msg)[3]);
		}
		move();
		if (team_won()) {
			map[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
			sem_op(MAP_SEM, 1, 0);
			break;
		}
		sem_op(MAP_SEM, 1, 0);
		sleep(PLAYER_CD);
	}
	printf("Team %c WON!\n", g_player.team);
}