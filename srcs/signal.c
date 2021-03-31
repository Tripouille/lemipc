#include "lemipc.h"

void
sigint_handler(int signal) {
	printf("Catching singnal: %i\n", signal);
	shm_destroy();
	sem_destroy();
	exit(0);
}