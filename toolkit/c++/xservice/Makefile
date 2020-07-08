CC=g++
CFLAGS=-std=c++11 -lpthread -pthread
OBJS=service
SRC = msgqueue.cpp \
	  msgsequence.cpp \
	  msgmanager.cpp \
	  service.cpp \
	  log.cpp \
	  main.cpp

service: $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o service

all: clean $(OBJS)

clean:
	-rm -rv $(OBJS)

