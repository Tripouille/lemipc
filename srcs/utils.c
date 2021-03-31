#include "lemipc.h"

void
perror_exit(char const *msg) {
	perror(msg);
	shm_destroy();
	sem_destroy();
	exit(1);
}