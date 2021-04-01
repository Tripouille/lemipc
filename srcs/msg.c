#include <lemipc.h>

void
msg_init(void) {
	if ((g_ipc.msgid = msgget(KEY, IPC_CREAT | IPC_EXCL | 0600)) < 0) {
		if ((g_ipc.msgid = msgget(KEY, 0600)) >= 0) {
			printf("MessageQ loaded.\n");
		}
		else
			perror_exit("msgget\n");
	}
	else
		printf("MessageQ created.\n");
}

void
msg_send(t_msg * msg) {
	if (msgsnd(g_ipc.msgid, msg, BUFFER_SIZE, 0) < 0)
		printf("Message send failure.\n");
	else
		printf("Message send success.\n");
}

bool
msg_receive(void) {
	long		type;

	((int*)(&type))[0] = g_player.pos.x;
	((int*)(&type))[1] = g_player.pos.y;
	if (msgrcv(g_ipc.msgid, g_player.msg, BUFFER_SIZE, type, IPC_NOWAIT) < 0) { //while
		if (errno == ENOMSG)
			printf("No new message.\n");
		else
			printf("Message receive failure.\n");
		return (false);
	}
	else
		printf("Message receive success.\n");
	return (true);
}

void
msg_destroy(void) {
	msgctl(g_ipc.msgid, IPC_RMID, 0);
}