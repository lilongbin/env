#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2020-08-05 17:10:53
####################################################

CC = gcc
XX = g++
CFLAGS_STATIC_LIB = 

target := myhsm
user_dir = ./
includes =  -I ./libhsm/include
lib = -lhsm
lib_path =  -L ./libhsm/

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))

all: $(target)
#$(target) : $(c_objects)
#	$(CC)  $^ -o $@  $(lib_path) $(lib)
$(target): $(c_objects)
	make -C libhsm/ M=$(PWD)
	$(CC)  $^ -o $@  $(lib_path) $(lib)

#%.o : %.cpp
#	$(XX) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.c
	$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	make -C libhsm/ clean M=$(PWD)
	-rm -rfv $(target)
	-rm -rfv $(c_objects)

