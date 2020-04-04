CC=g++
CFLAGS=-std=c++11 -lpthread -pthread
OBJS=le
SRC = main.cpp \
	  everything.cpp \
	  files.cpp \
	  mstring.cpp \
	  regexcpp.cpp \
      xservice/msgqueue.cpp \
	  xservice/msgsequence.cpp \
	  xservice/msgmanager.cpp \
	  xservice/service.cpp \

le: ${SRC}
	$(CC) ${SRC} $(CFLAGS) -o le

all: clean $(OBJS) proto

clean:
	-rm -rv $(OBJS) le.dat

