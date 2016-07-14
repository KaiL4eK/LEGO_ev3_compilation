# Here set ip address for examples deploy 
ip_robot_address = 0.0.0.0

CC = arm-linux-gnueabi-gcc
CXX = arm-linux-gnueabi-g++
LD = $(CC)
AR = arm-linux-gnueabi-ar

cross_compiler_directory = /usr/arm-linux-gnueabi
ev3cleti_lib_directory = $(HOME)/ev3cleti
# After changing here don`t forget to change in template

build_directory = build
public_directory = public
private_directory = private
binary_directory = binary
examples_directory = examples

library_directory = $(cross_compiler_directory)/lib $(build_directory)
include_directory = $(cross_compiler_directory)/include $(public_directory) $(private_directory)

install_directory = $(ev3cleti_lib_directory)
headers_install_directory = include
library_install_directory = lib
binary_install_directory = bin
examples_install_directory = examples

#--------------------------------------------

example_sources = $(wildcard $(examples_directory)/*.c)
executes = $(notdir $(basename $(example_sources)))
execute_files = $(addprefix $(build_directory)/, $(executes))

libraries_examples = ev3cleti stdc++
ldflags = $(addprefix -L, $(library_directory)) $(addprefix -l, $(libraries_examples)) -static

#--------------------------------------------

execute_link_file = /usr/local/bin/ev3napp

library_static_name = libev3cleti.a
library_static_file = $(build_directory)/$(library_static_name)

cflags = $(addprefix -I, $(include_directory)) -Wall -O3
cxxflags = $(cflags) -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP

objects_library  = $(addprefix $(build_directory)/,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))
objects_library += $(addprefix $(build_directory)/,$(patsubst %.c,%.o,$(wildcard *.c)))

all: $(build_directory) $(library_static_file)

$(library_static_file): $(objects_library)
	$(AR) rcs $@ $^

examples: all $(execute_files)

$(execute_files): $(example_sources)
	$(CC) -o $@ $(examples_directory)/$(notdir $@.c) $(cflags) $(ldflags)

$(build_directory)/%.o: %.c
	$(CC) -c -o $@ $< $(cflags)

$(build_directory)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(cxxflags)

install: all $(install_directory)
	cp -r $(public_directory)    $(install_directory)/$(headers_install_directory)
	cp -r $(library_static_file) $(install_directory)/$(library_install_directory)
	cp -r $(binary_directory)/*  $(install_directory)/$(binary_install_directory)
	cp -r $(examples_directory)	 $(install_directory)/$(examples_install_directory)

installapp:
	sudo ln -s $(install_directory)/$(binary_install_directory) $(execute_link_file)

installex: examples
	$(foreach executable, $(execute_files), scp $(executable) robot@$(ip_robot_address):~/$(notdir $(executable));)

$(build_directory):
	mkdir -p $@

$(install_directory):
	mkdir -p $@/$(library_install_directory)
	mkdir -p $@/$(binary_install_directory)

clean:
	rm -rf $(build_directory)

distclean: clean
	sudo rm -rf $(install_directory)
