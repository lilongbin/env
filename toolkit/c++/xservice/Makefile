CC=g++
CFLAGS=-std=c++11 -lpthread -pthread
OBJS=service
SRC = msgqueue.cpp \
	  msgsequence.cpp \
	  msgmanager.cpp \
	  service.cpp \
	  log.cpp \
	  main.cpp

all: clean $(OBJS)

service: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o service

clean:
	-rm -rv $(OBJS)

