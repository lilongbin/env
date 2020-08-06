#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2020-08-05 17:10:53
####################################################

CC = gcc
XX = g++
CFLAGS_STATIC_LIB = 

target := libhsm.a
user_dir = ./src
includes =  -I ./include
lib = -lpthread
lib_path =  -L ./

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))

all: $(target)
#$(target): $(c_objects)
#	$(CC)  $^ -o $@  $(lib_path) $(lib)
$(target): $(c_objects)
	ar -r $(target) $(c_objects)

#%.o : %.cpp
#	$(XX) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.c
	$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	-rm -rfv $(target)
	-rm -rfv $(c_objects)

