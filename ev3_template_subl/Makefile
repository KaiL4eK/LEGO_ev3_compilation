# 
# Configurable area
# 
ip_robot_address := __ip_address
execute_name := __project_name

execute_file := $(build_directory)/$(execute_name)

objects := $(patsubst %.c,%.o,$(wildcard *.c))

ev3_install_directory := /usr/local/ev3_arm
build_directory := build
libs_directory := $(ev3_install_directory)/lib
include_directory := $(ev3_install_directory)/include

cflags := $(addprefix -I, $(include_directory)) -Wall -O3
ldflags := $(addprefix -L, $(libs_directory))

CC := arm-linux-gnueabi-gcc
LD := $(CC)

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
	