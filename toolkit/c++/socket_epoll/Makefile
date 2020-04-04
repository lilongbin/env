CC=g++
CFLAGS=-std=c++11
OBJS=server client

all: clean $(OBJS)
server:
	$(CC) socketserver.cpp $(CFLAGS) -o server
client:
	$(CC) socketclient.cpp $(CFLAGS) -o client

clean:
	-rm -rv $(OBJS) 

