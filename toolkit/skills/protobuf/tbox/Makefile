#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2019-07-23 10:58:18
####################################################

# g++ tboxservice.cpp itbox.pb.cpp -L /usr/local/lib/ -lprotobuf -pthread -o tboxservice

CC        :=  g++
SRC       :=  tboxservice.cpp
SRC       +=  sequenceId.cpp
SRC       +=  itbox.pb.cpp
protofile :=  itbox.proto
CFLAGS    :=  -L /usr/local/lib/ -lprotobuf -lpthread
OBJ       :=  tboxservice

all: OBJ
proto:
	protoc -I. --cpp_out=. itbox.proto
	mv itbox.pb.cc itbox.pb.cpp

OBJ: proto
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)

.PHONY: clean
clean:
	-rm -v $(OBJ)

