#include "lemipc.h"

bool
is_enemmy(t_pos * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value!= g_player.team && map_value != MAP_EMPTY);
}

bool
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

bool
team_won(void) {
	char * map = g_ipc.shm;

	for (size_t p = 0; p < MAP_X * MAP_Y; ++p)
		if (map[p] != MAP_EMPTY && map[p] != g_player.team)
			return (false);
	return (true);
}

int
available_pos_around_target(t_pos * target) {
	t_pos	posible_pos[4];
	posible_pos[0] = (t_pos){target->x, target->y - 1}; //top
	posible_pos[1] = (t_pos){target->x + 1, target->y}; //left
	posible_pos[2] = (t_pos){target->x, target->y + 1}; //bottom
	posible_pos[3] = (t_pos){target->x - 1, target->y}; //right
	int		result = 0;

	for (int i = 0; i < 4; ++i)
		if (pos_is_in_map(posible_pos + i))
			++result;
	return (result);
}

t_pos
closest_available_pos(t_pos * start, t_pos * target) {
	double	closest_dist = 1E10;
	t_pos	closest_pos = {-1, -1};
	t_pos	posible_pos[4];
	posible_pos[0] = (t_pos){target->x, target->y - 1}; //top
	posible_pos[1] = (t_pos){target->x + 1, target->y}; //left
	posible_pos[2] = (t_pos){target->x, target->y + 1}; //bottom
	posible_pos[3] = (t_pos){target->x - 1, target->y}; //right

	for (int i = 0; i < 4; ++i) {
		double actual_dist = dist(start, posible_pos + i);
		if (pos_is_in_map(posible_pos + i)
		&& actual_dist < closest_dist) {
			closest_dist = actual_dist;
			closest_pos = posible_pos[i];
		}
	}
	return (closest_pos);
}
