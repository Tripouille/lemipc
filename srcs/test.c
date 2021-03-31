#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define SHMSZ		200
#define KEY			0x42
#define MAPSZ		10

int	g_shmid;

void
perror_exit(char const *msg) {
	perror(msg);
	exit(1);
}

void
map_init(char *map) {
	printf("map_init\n");
	memset(map, '.', MAPSZ * MAPSZ);
}

char *
get_shm() {
	char		*shm;
	bool		map_require_init = true;

	if ((g_shmid = shmget(KEY, SHMSZ, IPC_CREAT | IPC_EXCL | 0600)) < 0) {
		if (errno == EEXIST
		&& (g_shmid = shmget(KEY, SHMSZ, 0600)) >= 0) {
			printf("map already exist.");
			map_require_init = false;
		}
		else
			perror_exit("shmget");
	}

	if ((shm = shmat(g_shmid, NULL, 0)) == (void*)-1)
		perror_exit("shmat");
	if (map_require_init)
		map_init(shm);
	return (shm);
}

int map_display(char *map) {
	while (*map != '*') {
		for (size_t y = 0; y < MAPSZ; ++y) {
			write(1, map + (y * MAPSZ), MAPSZ);
			write(1, "\n", 1);
		}
		sleep(1);
	}
	return (0);
}

int main(int ac, char ** av) {
	if (ac != 2)
		return (1);

	char *shm = get_shm();

	if (av[1][0] == 'D')
		return (map_display(shm));

	*shm = av[1][0]; int i = 0;
	while (*shm != '*' && i++ < 10) {
		printf("av = %c / shm value = %c\n", av[1][0], *shm);
		sleep(10);
	}
	shmdt(shm);
	shmctl(g_shmid, IPC_RMID, 0);
	return (0);
}