CC=g++
CFLAGS=-std=c++11
OBJS=server client

all: clean $(OBJS)
server:
	$(CC) server.cpp socketserver.cpp base64.cpp sha1.cpp websocket_handler.cpp websocket_proc.cpp $(CFLAGS) -o server
client:
	$(CC) client.cpp websocket_client.cpp socketclient.cpp $(CFLAGS) -lpthread -o client

clean:
	-rm -rv $(OBJS) 

