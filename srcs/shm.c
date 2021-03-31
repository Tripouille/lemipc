#include "lemipc.h"

void
shm_init(void) {
	bool		map_require_init = true;

	if ((g_ipc.shmid = shmget(KEY, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0600)) < 0) {
		if (errno == EEXIST && (g_ipc.shmid = shmget(KEY, SHM_SIZE, 0600)) >= 0) {
			printf("SHM loaded.\n");
			map_require_init = false;
		}
		else
			perror_exit("shmget");
	}
	else
		printf("SHM created.\n");

	if ((g_ipc.shm = shmat(g_ipc.shmid, NULL, 0)) == (void*)-1)
		perror_exit("shmat");
	else
		printf("Attach success.\n");
	if (map_require_init)
		map_init();
}

void
shm_destroy(void) {
	shmdt(g_ipc.shm);
	shmctl(g_ipc.shmid, IPC_RMID, 0);
}