#include "lemipc.h"

ssize_t
min(ssize_t value1, ssize_t value2) {
	return (value1 < value2 ? value1 : value2);
}

ssize_t
max(ssize_t value1, ssize_t value2) {
	return (value1 > value2 ? value1 : value2);
}

void
error_exit(char const * msg) {
	write(2, msg, strlen(msg));
	if (one_team_won()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}
	exit(1);
}

void
perror_exit(char const * msg) {
	perror(msg);
	if (one_team_won()) {
		sem_destroy();
		shm_destroy();
		msg_destroy();
	}
	exit(1);
}

double
dist(t_pos const * start, t_pos const * end) {
	return (sqrt(pow(start->x - end->x, 2) + pow(start->y - end->y, 2)));
}
