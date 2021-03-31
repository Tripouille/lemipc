#include "lemipc.h"

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