/*send.c*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/signal.h>
#include <unistd.h>
#include <errno.h>

#define MSGKEY 1024

typedef struct Msg
{
	long msgtype;
	int msglen;
	char msgbody[2048];
} Msg;

class MsgQUser {
	private:
		int projid;
		int msqid;
		int require();
	public:
		MsgQUser(int projid);
		~MsgQUser();
		int MQ_Snd(Msg &msgs);
		int MQ_Rcv(Msg &msgs, long msgtype=0);
};

MsgQUser::MsgQUser(int projid):projid(MSGKEY)
{
	projid = projid;	
	msqid = require();
}

MsgQUser::~MsgQUser()
{
	return;
}

int MsgQUser::require()
{
	key_t key = ftok(".", projid);
	msqid = msgget(key, IPC_EXCL);  /*检查消息队列是否存在*/
	if (msqid < 0) {
		perror("msgget");
	}
	return msqid;
}

int MsgQUser::MQ_Snd(Msg &msgs)
{
	int ret = -1;
	ret = msgsnd(msqid, &msgs, sizeof(struct Msg)-sizeof(long), IPC_NOWAIT);
	if (ret < 0) {
		perror("msgsnd");
	}
	return ret;
}

int MsgQUser::MQ_Rcv(Msg &msgs, long msgtype)
{
	int ret = -1;
	ret = msgrcv(msqid, &msgs, sizeof(struct Msg)-sizeof(long), msgtype, 0);
	if (ret < 0) {
		perror("msgrcv");
	}
	return ret;
}

int fillmsg(Msg &msg, long msgtype, int msglen, char *msgbody)
{
	msg.msgtype = msgtype;
	msg.msglen = msglen;
	strncpy(msg.msgbody, msgbody, msglen);
	return true;
}

int parse(int mid, Msg &msg)
{
	if (mid != msg.msgtype)
	{
		return true;
	}
	long msgtype = 0;
	int msglen = 0;
	char msgbody[2048];
	msgtype = msg.msgtype;
	msglen = msg.msglen;
	strncpy(msgbody, msg.msgbody, msglen);
	msgbody[msglen+1] = '\0';
	printf("parse: [%ld] [%s]\n", msgtype, (char *)msgbody);
	return true;
}

int main()
{
	struct Msg msgs;
	int projid = MSGKEY;
	long msgtype;
	int msglen;
	char msgbody[32] = "1234567890123456789012345678901";
	int ret;

	MsgQUser msguser(projid);
	long i = 0;
	for (i = 0; i <= 100; i++)
	{
		msgtype = (i % 3) + 1;
		// sprintf(msgbody, "hello world x%ld", i);
		msglen = strlen(msgbody);
		fillmsg(msgs, msgtype, msglen, msgbody);
		/* 发送消息队列 */
		ret = msguser.MQ_Snd(msgs);
		if (ret < 0)
		{
			break;
		}
		usleep(1000);
	}
}

