CC = arm-linux-gnueabi-gcc
CXX = arm-linux-gnueabi-g++
LD = $(CC)
AR = arm-linux-gnueabi-ar

cross_compiler_directory = /usr/arm-linux-gnueabi
ev3cleti_lib_directory = /usr/local/ev3cleti

public_directory = public
private_directory = private
build_directory = build
examples_directory = examples

library_directory = $(cross_compiler_directory)/lib $(build_directory)
include_directory = $(cross_compiler_directory)/include $(public_directory) $(private_directory)

install_directory = $(ev3cleti_lib_directory)
headers_install_directory = include
library_install_directory = lib
binary_install_directory = bin
examples_install_directory = ev3cleti_examples

library_static_name = libev3cleti.a
library_static_file = $(build_directory)/$(library_static_name)

cflags = $(addprefix -I, $(include_directory)) -Wall -O3
cxxflags = $(cflags) -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP

objects_library  = $(addprefix $(build_directory)/,$(patsubst %.cpp,%.o,$(wildcard *.cpp)))
objects_library += $(addprefix $(build_directory)/,$(patsubst %.c,%.o,$(wildcard *.c)))

all: $(build_directory) $(library_static_file)

$(library_static_file): $(objects_library)
	$(AR) rcs $@ $^

$(build_directory)/%.o: %.c
	$(CC) -c -o $@ $< $(cflags)

$(build_directory)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(cxxflags)

install: all $(install_directory)
	cp -r public/* $(install_directory)/include
	cp -r $(library_static_file) $(install_directory)/lib/

$(build_directory):
	mkdir -p $@

$(install_directory):
	mkdir -p $@/$(library_install_directory)
	mkdir -p $@/$(headers_install_directory)
	mkdir -p $@/$(binary_install_directory)

examples:
	cp -r $(examples_directory) $(HOME)/$(examples_install_directory)

clean:
	rm -rf $(build_directory)
	rm -f *~
	make clean -C $(examples_directory)

distclean: clean
	sudo rm -rf $(ev3cleti_lib_directory)
