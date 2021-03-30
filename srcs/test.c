#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMSZ     27

int main(int ac, char ** av) {
	(void)ac; (void)av;


	int shmid;
	key_t key = 5678;
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	char *shm;
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
	*shm = av[1][0];
	while (*shm != '*') {
		sleep(10);
		printf("av = %c / shm value = %c\n", av[1][0], *shm);
	}
	return (0);
}