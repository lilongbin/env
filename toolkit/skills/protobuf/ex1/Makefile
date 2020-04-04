#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2019-07-23 10:58:18
####################################################

# g++ helloproto.cpp test.pb.cpp -L /usr/local/lib/ -lprotobuf -pthread -o helloproto

CC        :=  g++
SRC       :=  helloproto.cpp
SRC       +=  test.pb.cpp
protofile :=  test.proto
CFLAGS    :=  -L /usr/local/lib/ -lprotobuf -lpthread
OBJ       :=  helloproto

all: OBJ
proto:
	protoc -I. --cpp_out=. test.proto
	mv test.pb.cc test.pb.cpp

OBJ: proto
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)

.PHONY: clean
clean:
	-rm -v $(OBJ)

