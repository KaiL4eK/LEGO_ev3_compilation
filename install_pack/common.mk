CC := arm-linux-gnueabi-gcc
CXX := arm-linux-gnueabi-g++
LD := $(CC)
AR := arm-linux-gnueabi-ar

cross_compiler_directory = /usr/arm-linux-gnueabi
ev3c_lib_directory = /usr/local/ev3c
ev3cleti_lib_directory = /usr/local/ev3cleti

library_directory = $(cross_compiler_directory)/lib $(ev3c_lib_directory)/lib
include_directory = $(cross_compiler_directory)/include $(ev3c_lib_directory)/include

cflags = $(addprefix -I, $(include_directory)) -Wall -O3
