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

class MsgQ {
	private:
		int projid;
		int msqid;
		int init();
		int destroy();
	public:
		MsgQ(int projid);
		~MsgQ();
		int MQ_Snd(Msg &msgs);
		int MQ_Rcv(Msg &msgs, long msgtype=0);
};

MsgQ::MsgQ(int projid):projid(MSGKEY)
{
	projid = projid;	
	msqid = init();
	if (0 > msqid)
	{
		return;
	}
}

MsgQ::~MsgQ()
{
	destroy();
}

int MsgQ::init()
{
	key_t key = ftok(".", projid);
	msqid = msgget(key, IPC_EXCL);  /*检查消息队列是否存在*/
	if (msqid < 0) {
		msqid = msgget(key, IPC_CREAT | 0666); /*创建消息队列*/
		if (msqid < 0) {
			perror("msgget");
		}
	}
	printf("Create msg queue OK: %d\n", msqid);
	return msqid;
}

int MsgQ::MQ_Snd(Msg &msgs)
{
	int ret = -1;
	ret = msgsnd(msqid, &msgs, sizeof(struct Msg)-sizeof(long), IPC_NOWAIT);
	if (ret < 0) {
		perror("msgsnd");
	}
	return ret;
}

int MsgQ::MQ_Rcv(Msg &msgs, long msgtype)
{
	int ret = -1;
	ret = msgrcv(msqid, &msgs, sizeof(struct Msg)-sizeof(long), msgtype, 0);
	if (ret < 0) {
		perror("msgrcv");
	}
	return ret;
}

int MsgQ::destroy()
{
	key_t key = ftok(".", projid);
	int msqid = msgget(key, IPC_EXCL);  /*检查消息队列是否存在*/
	struct msqid_ds mds;
	if (msqid < 0) {
		return true;
	}
	if (!msgctl(msqid, IPC_STAT, &mds)) {
		if (mds.msg_qnum <= 0) {
			// 消息数为0,可以清空
			msgctl(msqid, IPC_RMID, 0);
			printf("msgqueue %d has cleared\n", msqid);
		} else {
			printf("msgqueue %d already has %d msgs\n", msqid, (int)mds.msg_qnum);
			return false;
		}
	}
	return true;
}

void sig_close(int signo)
{
	printf("\ntrap signal: %d\n", signo);
	printf("Service is going down...\n");
	exit(0);
}

int main()
{
	signal(SIGINT, sig_close);	//定义信号处理函数
	struct Msg msgs;
	int projid = MSGKEY;

	MsgQ msgq(projid);
	while (1) {
		msgq.MQ_Rcv(msgs, 0); // 读任意类型消息防止有些消息没有被读取导致队列满 
		usleep(100);
	}
}

