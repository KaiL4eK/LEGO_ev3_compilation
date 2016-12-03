sources = $(ev3cleti_lib_directory)/main.c
sources += $(wildcard *.c)

objects = $(patsubst %.c,%.o,$(notdir $(sources)))

cross_compiler_directory = /usr/arm-linux-gnueabi
build_directory = build

library_directory = $(cross_compiler_directory)/lib $(ev3cleti_lib_directory)/lib
include_directory = $(cross_compiler_directory)/include $(ev3cleti_lib_directory)/include

libraries_static = ev3cleti stdc++
libraries_dynamic = pthread m

cflags = $(addprefix -I, $(include_directory)) -Wall -O3 -std=gnu99
ldflags = $(addprefix -L, $(library_directory)) -Wl,-Bstatic $(addprefix -l, $(libraries_static)) -Wl,-Bdynamic $(addprefix -l, $(libraries_dynamic))

CC = arm-linux-gnueabi-gcc
LD = $(CC)

execute_file = $(build_directory)/$(execute_name)


# Main targets --------------------------------------------

all: $(build_directory) $(execute_file) 

clean:
	rm -rf $(build_directory)

install: all
	scp $(execute_file) robot@$(ip_robot_address):~/$(execute_name)


# Service targets --------------------------------------------
	
$(execute_file): $(build_directory)/$(objects) 
	$(LD) -o $@ $< $(ldflags)

$(build_directory)/%.o : $(ev3cleti_lib_directory)/%.c
	$(CC) $(cflags) -c $< -o $@

$(build_directory)/%.o : %.c
	$(CC) $(cflags) -c $< -o $@

$(build_directory):
	mkdir -p $@