#ifndef LEMIPC_H
# define LEMIPC_H
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <time.h>

# define KEY			0x42
# define SHM_SIZE		200
# define MAP_X			10
# define MAP_Y			10
# define MAP_EMPTY		'.'

# define MAP_SEM		0

typedef struct	s_ipc
{
	int		shmid;
	char	*shm;

	int 	semid;

}				t_ipc;

typedef struct	s_pos
{
	int		x;
	int		y;
}				t_pos;

extern t_ipc	g_ipc;

/* shm.c */
void shm_init(void);
void shm_destroy(void);
/* sem.c */
void sem_init(void);
void sem_op(unsigned short num, short op, short flags);
void sem_destroy(void);
/* map.c */
void map_init(void);
int map_display(void);
t_pos map_get_random_pos(void);
/* utils.c */
void perror_exit(char const *msg);
/* signal.c */
void sig_handler(int signal);
/* play.c */
void play(char team);
#endif