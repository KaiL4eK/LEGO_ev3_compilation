
build_directory := build
project_name := test
execute_file := $(build_directory)/$(project_name)

objects := $(patsubst %.c,%.o,$(wildcard *.c))

libs_directory := /usr/local/ev3_arm/lib
include_directory := /usr/local/ev3_arm/include

cflags := $(addprefix -I, $(include_directory)) -Wall 
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
	scp $(execute_file) robot@$(ip_robot_address):~/$(project_name)