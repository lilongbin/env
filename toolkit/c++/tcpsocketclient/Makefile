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
CXXFLAGS = -std=c++11 $(CFLAGS)

target := myreceiver
user_dir = .
includes = -I ./
libs += -L ./

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))
cpp_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.cpp))
cpp_objects = $(patsubst %.cpp, %.o, $(cpp_src_files))

objects = $(cpp_objects)

all: clean $(target)
$(target): $(objects)
	@#$(CC) $^ -o $@ $(CXXFLAGS) $(libs)
	@echo $(XX) $^ -o $@ $(libs) $(includes)
	@$(XX) $^ -o $@ $(CXXFLAGS) $(libs) $(includes)
	@g++ testserver.cc -o server

%.o : %.c
	@echo $(CC) -c $^ -g -o $@ -DLinux
	@$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.cpp
	@echo $(XX) -c $^ -g -o $@ -DLinux
	@$(XX) $(CXXFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	-rm -rfv $(target) server
	-rm -rfv $(objects)

