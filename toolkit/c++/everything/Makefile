CC=g++
# CFLAGS=-std=c++11 -L/usr/local/lib -lprotobuf -lpthread
# CFLAGS=-std=c++11 -L./lib -lprotobuf -lpthread
CFLAGS=-std=c++11 -lpthread
OBJS=le
SRC = everything.cpp \
	  files.cpp \
	  mstring.cpp \
	  regexcpp.cpp

le: ${SRC}
	$(CC) ${SRC} $(CFLAGS) -o le

all: clean $(OBJS) proto

proto:
	protoc -I=. --cpp_out=. protobuf.proto

clean:
	# -rm -rv $(OBJS) le.dat protobuf.pb.*
	-rm -rv $(OBJS) le.dat

