#include "lemipc.h"

bool
im_dead(void) {
	char *	map = g_ipc.shm;
	t_pos	posible_pos[8] = {
		{g_player.pos.x, g_player.pos.y + 1},
		{g_player.pos.x, g_player.pos.y - 1},
		{g_player.pos.x + 1, g_player.pos.y},
		{g_player.pos.x - 1, g_player.pos.y},
		{g_player.pos.x - 1, g_player.pos.y - 1},
		{g_player.pos.x + 1, g_player.pos.y - 1},
		{g_player.pos.x - 1, g_player.pos.y + 1},
		{g_player.pos.x + 1, g_player.pos.y + 1}
	};
	for (int i = 0; i < 8; ++i)
		for (int j = i + 1; j < 8; ++j)
			if (pos_is_in_map(posible_pos + i) && is_enemy(posible_pos + i)
			&& map[pos_to_indice(posible_pos + i)] == map[pos_to_indice(posible_pos + j)])
				return (true);
	return (false);
}

bool
is_empty(t_pos const * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value == MAP_EMPTY);
}

bool
is_enemy(t_pos const * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value != g_player.team && map_value != MAP_EMPTY);
}

bool
is_valuable_enemy(t_pos const * pos) {
	return (is_enemy(pos) && at_range(pos, is_ally) < 2
	&& ((at_range(pos, is_ally) && at_range(pos, is_empty))
		|| at_range(pos, is_empty) > 1));
}

bool
is_ally(t_pos const * pos) {
	char map_value = g_ipc.shm[pos_to_indice(pos)];
	return (map_value == g_player.team
	&& !(pos->x == g_player.pos.x && pos->y == g_player.pos.y));
}

size_t
pos_is_in_map(t_pos const * pos) {
	return (pos->x >= 0 && pos->x < MAP_X
	&& pos->y >= 0 && pos->y < MAP_Y);
}

size_t
pos_to_indice(t_pos const * pos) {
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
at_range(t_pos const * target, bool (*is_required_type)(t_pos const * pos)) {
	int		result = 0;
	t_pos	posible_pos[8] = {
		{target->x, target->y + 1},
		{target->x, target->y - 1},
		{target->x + 1, target->y},
		{target->x - 1, target->y},
		{target->x - 1, target->y - 1},
		{target->x + 1, target->y - 1},
		{target->x - 1, target->y + 1},
		{target->x + 1, target->y + 1}
	};

	for (int i = 0; i < 8; ++i)
		if (pos_is_in_map(posible_pos + i) && is_required_type(posible_pos + i))
			++result;
	return (result);
}

t_plist *
get_available_pos_at_range(t_pos const * target) {
	t_pos	posible_pos[8] = {
		{target->x, target->y + 1},
		{target->x, target->y - 1},
		{target->x + 1, target->y},
		{target->x - 1, target->y},
		{target->x - 1, target->y - 1},
		{target->x + 1, target->y - 1},
		{target->x - 1, target->y + 1},
		{target->x + 1, target->y + 1}
	};
	t_plist * available_pos = malloc(sizeof(t_plist));
	if (available_pos == NULL)
		return (NULL);
	plist_initialize(available_pos);

	for (int i = 0; i < 8; ++i)
		if (pos_is_in_map(posible_pos + i) && is_empty(posible_pos + i))
			if (plist_push(available_pos, posible_pos[i]) == NULL) {
				free(available_pos);
				return (NULL);
			}
	return (available_pos);
}