#include "lemipc.h"

void
sig_handler(int signal) {
	printf("Catching singnal: %i\n", signal);
	sem_destroy();
	shm_destroy();
	exit(0);
}