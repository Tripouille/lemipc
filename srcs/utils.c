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
error_exit(char const *msg) {
	write(2, msg, strlen(msg));
	shm_destroy();
	sem_destroy();
	exit(1);
}

void
perror_exit(char const *msg) {
	perror(msg);
	shm_destroy();
	sem_destroy();
	exit(1);
}