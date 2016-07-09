include ../common.mk

public_directory = public
private_directory = private
build_directory = build
examples_directory = examples

library_directory += $(build_directory)
include_directory += $(public_directory) $(private_directory)

install_directory = $(ev3cleti_lib_directory)
headers_install_directory = include
library_install_directory = lib

library_static_name = libev3cleti.a
library_static_file = $(build_directory)/$(library_static_name)

libraries = ev3c ev3cleti stdc++

cxxflags = $(cflags) -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP
ldflags = $(addprefix -L, $(library_directory)) $(addprefix -l, $(libraries))

objects_library = $(addprefix $(build_directory)/,$(patsubst %.cpp,%.o,$(wildcard *lib.cpp)))

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

clean:
	rm -rf $(build_directory)
	rm -f *~
	make clean -C $(examples_directory)
