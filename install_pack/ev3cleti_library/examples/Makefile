# 
# Configurable area
# 
ip_robot_address = 0.0.0.0

sources = $(wildcard *.c)
executes = $(basename $(sources))

cross_compiler_directory = /usr/arm-linux-gnueabi
ev3c_directory = /usr/local/ev3c
ev3cleti_lib_directory = /usr/local/ev3cleti
build_directory = build

library_directory = $(cross_compiler_directory)/lib $(ev3c_directory)/lib $(ev3cleti_lib_directory)/lib
include_directory = $(cross_compiler_directory)/include $(ev3c_directory)/include $(ev3cleti_lib_directory)/include

libraries = ev3c ev3cleti stdc++ 

cflags = $(addprefix -I, $(include_directory)) -Wall -O3
ldflags = $(addprefix -L, $(library_directory)) $(addprefix -l, $(libraries))

CC = arm-linux-gnueabi-gcc

execute_files = $(addprefix $(build_directory)/, $(executes))

all: $(build_directory) $(execute_files)
	
$(execute_files): $(sources)
	$(CC) -o $@ $(notdir $@.c) $(ldflags) $(cflags)

$(build_directory):
	mkdir -p $@

clean:
	rm -rf $(build_directory)

install: all
	$(foreach executable, $(execute_files), scp $(executable) robot@$(ip_robot_address):~/$(notdir $(executable));)
	