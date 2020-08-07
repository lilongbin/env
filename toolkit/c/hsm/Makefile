#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2020-08-05 17:10:53
####################################################

CC = gcc
XX = g++
CFLAGS_STATIC_LIB = 
CFLAGS = -lpthread -pthread
CFLAGS += -Wall -Wextra -Werror -Wunused -Wunreachable-code -Wformat-security

target := myhsm
user_dir = .
includes = -I ./libhsm/include
lib = -lhsm
lib_path = -L ./libhsm/

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))
cpp_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.cpp))
cpp_objects = $(patsubst %.cpp, %.o, $(cpp_src_files))

objects = $(c_objects)

all: $(target)
$(target): $(objects)
	make -C libhsm/ M=$(PWD)
	$(CC)  $^ -o $@  $(lib_path) $(lib)
	#$(XX)  $^ -o $@  $(lib_path) $(lib)

%.o : %.c
	$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.cpp
	$(XX) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	make -C libhsm/ clean M=$(PWD)
	-rm -rfv $(target)
	-rm -rfv $(objects)

