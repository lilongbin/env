#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2019-07-23 10:58:18
####################################################

# g++ helloperson.cpp person.pb.cpp -L /usr/local/lib/ -lprotobuf -pthread -o helloperson

CC        :=  g++
SRC       :=  helloperson.cpp
SRC       +=  person.pb.cpp
protofile :=  person.proto
CFLAGS    :=  -L /usr/local/lib/ -lprotobuf -lpthread
OBJ       :=  helloperson

all: OBJ
proto:
	protoc -I. --cpp_out=. person.proto
	mv person.pb.cc person.pb.cpp

OBJ: proto
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)

.PHONY: clean
clean:
	-rm -v $(OBJ)

