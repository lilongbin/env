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
#ifndef MID
#define MID 3
#endif

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
	printf("parse: mid=%d, msgtype=%ld\n", mid, msg.msgtype);
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
	int ret;
	int mid = MID;

	MsgQUser msguser(projid);
	while (1) {
		/*读消息队列 */
		// 只接收指定类型的消息
		ret = msguser.MQ_Rcv(msgs, mid);
		if (ret < 0)
		{
			break;
		}
		parse(mid, msgs);
	}
}

