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
contact_closest_ally(t_pos * ally, t_pos dest) {
	printf("Contacting ally at pos x: %i, y: %i\n", ally->x, ally->y);
	t_msg	msg;
	((int*)(&msg.type))[0] = ally->x;
	((int*)(&msg.type))[1] = ally->y;
	((int*)(msg.s))[0] = dest.x;
	((int*)(msg.s))[1] = dest.y;
	msg_send(&msg);
}

static void
attack(t_pos * enemy) {
	printf("Enemy at pos x: %i, y: %i\n", enemy->x, enemy->y);
	if (at_range(enemy, is_ally)) {
		printf("Yeah! gonna help my ally\n");
		return ;
	}
	t_pos		closest_ally_pos = scan(enemy, max(MAP_Y, MAP_X), is_ally);
	if (closest_ally_pos.x == -1) {
		printf("Grrr I've no ally\n");
		return ;
	}
	else
		contact_closest_ally(&closest_ally_pos, (t_pos){-1, -1});
}

static void
move(void) {
	t_pos		enemy_pos = scan(&g_player.pos, max(MAP_Y, MAP_X), is_valuable_enemy);

	printf("Player x %i y %i scanning for enemy !\n", g_player.pos.x, g_player.pos.y);
	if (enemy_pos.x == -1)
		printf("No valuable enemy detected.\n");
	else
		attack(&enemy_pos);
}

void
play(void) {
	char *		map = g_ipc.shm;
	
	init_pos(); sleep(PLAYER_WARMUP);
	while (1) {
		sem_op(MAP_SEM, -1, 0);
		//Turn start
		if (team_won()) {
			map[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
			sem_op(MAP_SEM, 1, 0);
			break;
		}
		if (msg_receive()) {
			printf("Message receive: x %i y %i\n",((int*)g_player.msg)[2],  ((int*)g_player.msg)[3]);
		}
		move();
		//Turn end
		sem_op(MAP_SEM, 1, 0);
		sleep(PLAYER_CD);
	}
	printf("Team %c WON!\n", g_player.team);
}