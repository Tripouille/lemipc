#include "lemipc.h"

static void
clear_actual_pos(void) {
	g_ipc.shm[pos_to_indice(&g_player.pos)] = MAP_EMPTY;
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

	printf("Contacting ally at pos x: %i, y: %i giving order x %i y %i\n", ally->x, ally->y, dest.x, dest.y);
	t_msg	msg;
	((int*)(&msg.type))[0] = ally->x;
	((int*)(&msg.type))[1] = ally->y;
	((int*)(msg.s))[0] = dest.x;
	((int*)(msg.s))[1] = dest.y;
	msg_send(&msg);
}

static void
move(t_pos new_pos) {
	printf("I'm going x %i y %i\n", new_pos.x, new_pos.y);
	clear_actual_pos();
	g_player.pos = new_pos;
	g_ipc.shm[pos_to_indice(&g_player.pos)] = g_player.team;
}

static void
attack(t_pos * enemy) {
	printf("Enemy at pos x: %i, y: %i\n", enemy->x, enemy->y);
	t_pos		closest_ally_pos = scan(enemy, max(MAP_Y, MAP_X), is_ally);
	t_list * 	available_pos = get_available_pos_at_range(enemy);

	if (available_pos == NULL) {
		error_exit("Out of memory.\n");
	}
	if (at_range(enemy, is_ally)) {
		list_sort(available_pos, by_dist, &g_player.pos);
		printf("Yeah! I'm gonna help my ally going x %i y %i\n", available_pos->head->pos.x, available_pos->head->pos.y);
		move(list_shift(available_pos));
	}
	else if (closest_ally_pos.x != -1) {
		list_sort(available_pos, by_dist, &g_player.pos);
		move(list_shift(available_pos));
		list_sort(available_pos, by_dist, &closest_ally_pos);
		contact_closest_ally(&closest_ally_pos, list_shift(available_pos));
	}
	else
		printf("Grrr I've no ally\n");
	list_destroy(available_pos);
	free(available_pos);
}

static void
think(void) {
	t_pos		enemy_pos = scan(&g_player.pos, max(MAP_Y, MAP_X), is_valuable_enemy);

	printf("Player x %i y %i scanning for valuable enemy !\n", g_player.pos.x, g_player.pos.y);
	if (enemy_pos.x == -1)
		printf("No valuable enemy detected.\n");
	else
		attack(&enemy_pos);
}

void
play(void) {
	init_pos();
	sleep(PLAYER_WARMUP);

	while (1) {
		sem_op(MAP_SEM, -1, 0);
		//Turn start
		if (team_won()) {
			printf("My team WON!\n");
			clear_actual_pos();
			break ;
		}
		else if (im_dead()) {
			printf("Arghhh... I'm dead.\n");
			clear_actual_pos();
			break ;
		}
		else if (msg_receive()) {
			t_pos	receive_pos = {((int*)g_player.msg)[2], ((int*)g_player.msg)[3]};
			printf("Order receive: x %i y %i\n", receive_pos.x,  receive_pos.y);
			move(receive_pos);
		}
		else if (at_range(&g_player.pos, is_enemy)) {
			printf("I'm fighting!\n");
		}
		else
			think();
		//Turn end
		sem_op(MAP_SEM, 1, 0);
		sleep(PLAYER_CD);
	}
	sem_op(MAP_SEM, 1, 0);
}