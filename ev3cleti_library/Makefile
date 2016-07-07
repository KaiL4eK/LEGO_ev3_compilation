CC := arm-linux-gnueabi-gcc
CXX := arm-linux-gnueabi-g++
LD := $(CC)
AR := arm-linux-gnueabi-ar

cross_compiler_directory := /usr/arm-linux-gnueabi
ev3c_directory := /usr/ev3_arm

public_directory := public
private_directory := private
build_directory := build

library_directory := $(cross_compiler_directory)/lib $(ev3c_directory)/lib $(build_directory)
include_directory := $(cross_compiler_directory)/include $(ev3c_directory)/include $(public_directory) $(private_directory)

install_directory := /usr/local/ev3cleti
headers_install_directory := include
library_install_directory := lib

library_static_name := libev3cleti.a
library_static_file := $(build_directory)/$(library_static_name)

libraries := ev3cleti stdc++

cflags := $(addprefix -I, $(include_directory)) -Wall -O3 
cxxflags := $(cflags) -std=gnu++0x -D_GLIBCXX_USE_NANOSLEEP
ldflags := $(addprefix -L, $(library_directory)) $(addprefix -l, $(libraries))

tests := test_sound
tests_executable := $(addprefix $(build_directory)/, $(tests))

objects_tests := $(addprefix $(build_directory)/,$(addsuffix .o,$(tests)))
objects_library := $(addprefix $(build_directory)/,$(patsubst %.cpp,%.o,$(wildcard *lib.cpp)))

all: $(build_directory) $(library_static_file) $(tests_executable)

$(library_static_file): $(objects_library)
	$(AR) rcs $@ $^

$(tests_executable): $(objects_tests)
	$(LD) -o $@ $< $(ldflags)

$(build_directory)/%.o: %.c
	$(CC) -c -o $@ $< $(cflags)

$(build_directory)/%.o: %.cpp
	$(CXX) -c -o $@ $< $(cxxflags)

install: all $(install_directory)
	cp -r public $(install_directory)/include
	cp -r $(library_static_file) $(install_directory)/lib/

$(build_directory):
	mkdir -p $@

$(install_directory):
	mkdir -p $@/$(library_install_directory)
	mkdir -p $@/$(headers_install_directory)

clean:
	rm -rf $(build_directory)
	rm -f *~
