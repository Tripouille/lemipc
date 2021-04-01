#ifndef LEMIPC_H
# define LEMIPC_H
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/msg.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <signal.h>
# include <time.h>

# define BUFFER_SIZE	(sizeof(long) * 2)
# define KEY			0x42
# define SHM_SIZE		200
# define MAP_X			5
# define MAP_Y			5
# define MAP_EMPTY		'.'

# define MAP_SEM		0

# define DISPLAY		-1

typedef struct	s_ipc
{
	int		shmid;
	char	*shm;

	int 	semid;
	int		msgid;
}				t_ipc;

typedef struct	s_pos
{
	int		x;
	int		y;
}				t_pos;

typedef struct	s_player
{
	char		team;
	t_pos		pos;
	char		msg[BUFFER_SIZE];
}				t_player;

typedef struct	s_msg
{
	long	type;
	char	s[sizeof(long)];
}				t_msg;

extern		t_ipc		g_ipc;
extern		t_player	g_player;

/* shm.c */
void shm_init(void);
void shm_destroy(void);
/* sem.c */
void sem_init(void);
void sem_op(unsigned short num, short op, short flags);
void sem_destroy(void);
/* msg.c */
void msg_init(void);
void msg_send(t_msg * msg);
bool msg_receive(void);
void msg_destroy(void);
/* map.c */
void map_init(void);
int map_display(void);
bool map_is_empty(void);
t_pos map_get_random_pos(void);
/* utils.c */
ssize_t min(ssize_t value1, ssize_t value2);
ssize_t max(ssize_t value1, ssize_t value2);
void error_exit(char const *msg);
void perror_exit(char const *msg);
/* signal.c */
void sig_handler(int signal);
/* play.c */
void play(void);
size_t pos_is_in_map(t_pos * pos);
size_t pos_to_indice(t_pos * pos);

#endif