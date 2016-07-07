# 
# Configurable area
# 
ip_robot_address := __ip_address
execute_name := __project_name

objects := $(patsubst %.c,%.o,$(wildcard *.c))

cross_compiler_directory := /usr/arm-linux-gnueabi
ev3_install_directory := /usr/local/ev3c
build_directory := build

library_directory := $(cross_compiler_directory)/lib $(ev3c_directory)/lib
include_directory := $(cross_compiler_directory)/include $(ev3c_directory)/include

cflags := $(addprefix -I, $(include_directory)) -Wall -O3
ldflags := $(addprefix -L, $(library_directory))

CC := arm-linux-gnueabi-gcc
LD := $(CC)

execute_file := $(build_directory)/$(execute_name)

all: $(build_directory) $(execute_file) 
	
$(execute_file): $(build_directory)/$(objects) 
	$(LD) -o $@ $< $(ldflags)

$(build_directory)/%.o : %.c
	$(CC) $(cflags) -c $< -o $@

$(build_directory):
	mkdir -p $@

clean:
	rm -rf $(build_directory)

install: all
	scp $(execute_file) robot@$(ip_robot_address):~/$(execute_name)
	