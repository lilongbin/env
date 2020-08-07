
CC = gcc
XX = g++
CFLAGS_STATIC_LIB = 
CFLAGS = -std=c++11 -lpthread -pthread
CFLAGS += -Wall -Wextra -Werror -Wunused -Wunreachable-code -Wformat-security

target := service
user_dir = .
includes =  -I ./
lib = -lpthread
lib_path =  -L ./

c_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.c))
c_objects = $(patsubst %.c, %.o, $(c_src_files))
cpp_src_files = $(foreach d, $(user_dir), $(wildcard $(d)/*.cpp))
cpp_objects = $(patsubst %.cpp, %.o, $(cpp_src_files))

objects = $(cpp_objects)

all: $(target)
$(target): $(objects)
	#make -C libhsm/ M=$(PWD)
	#$(CC)  $^ -o $@  $(lib_path) $(lib)
	$(XX)  $^ -o $@  $(lib_path) $(lib)

%.o : %.c
	$(CC) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux
%.o : %.cpp
	$(XX) $(CFLAGS) $(includes) -c $^ -g -o $@ -DLinux

.phony: clean
clean:
	#make -C libhsm/ clean M=$(PWD)
	-rm -rfv $(target)
	-rm -rfv $(objects)

