#target: prerequisites 
#	command
####################################################
# Author      : longbin
# Created date: 2020-08-05 17:10:53
####################################################

CC = gcc
XX = g++
AR = ar
CFLAGS_STATIC_LIB = 
CFLAGS = -lpthread -pthread
CFLAGS += -Wall -Wextra -Werror -Wunused -Wunreachable-code -Wformat-security

target := libhsm.a
user_dir = ./src
includes = -I ./include
lib = -lpthread
lib_path = -L ./

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))
cpp_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.cpp))
cpp_objects = $(patsubst %.cpp, %.o, $(cpp_src_files))

objects = $(c_objects)

all: $(target)
$(target): $(objects)
	$(AR) -r $(target) $(c_objects)
	#$(XX)  $^ -o $@  $(lib_path) $(lib)

%.o : %.c
	$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.cpp
	$(XX) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	-rm -rfv $(target)
	-rm -rfv $(objects)

