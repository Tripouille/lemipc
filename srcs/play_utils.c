#include "lemipc.h"

bool
is_empty(t_pos * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value == MAP_EMPTY);
}

bool
is_enemy(t_pos * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value != g_player.team && map_value != MAP_EMPTY);
}

bool
is_valuable_enemy(t_pos * pos) {
	return (is_enemy(pos) && at_range(pos, is_ally) < 2
	&& ((at_range(pos, is_ally) && at_range(pos, is_empty))
		|| at_range(pos, is_empty) > 1));
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
at_range(t_pos *target, bool (*is_required_type)(t_pos * pos)) {
	int		result = 0;
	t_pos	posible_pos[4] = {
		{target->x, target->y + 1},
		{target->x, target->y - 1},
		{target->x + 1, target->y},
		{target->x - 1, target->y}
	};

	for (int i = 0; i < 4; ++i)
		if (pos_is_in_map(posible_pos + i) && is_required_type(posible_pos + i))
			++result;
	return (result);
}