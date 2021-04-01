#include "lemipc.h"

void
sem_init(void) {
	if ((g_ipc.semid = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0600)) < 0) {
		if (errno == EEXIST && (g_ipc.semid = semget(KEY, 1, 0600)) >= 0)
			printf("SEM loaded.\n");
		else
			perror_exit("semget");
	}
	else
	{
		sem_op(0, 1, 0);
		printf("SEM created.\n");
	}
	printf("SEM value: %i\n", semctl(g_ipc.semid, 0, GETVAL));
}

void
sem_op(unsigned short int num, short int op, short int flags) {
	struct sembuf buf = {num, op, flags};
	semop(g_ipc.semid, &buf, 1);
}

void
sem_destroy(void) {
	semctl(g_ipc.semid, 0, IPC_RMID);
}
