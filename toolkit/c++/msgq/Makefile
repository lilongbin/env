CC=g++
# CFLAGS=-std=c++11
OBJS=bus snd rcv1 rcv2
all: clean $(OBJS)
snd:
	$(CC) msgqsnd.cpp $(CFLAGS) -o snd
bus:
	$(CC) msgqbus.cpp $(CFLAGS) -o bus
rcv1:
	$(CC) msgqrcv.cpp -DMID=1 $(CFLAGS) -o rcv1
rcv2:
	$(CC) msgqrcv.cpp -DMID=2 $(CFLAGS) -o rcv2

clean:
	-rm -rv $(OBJS) 

