#!/bin/bash

read -d '' main_text <<"EOF"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>
#include <pthread.h>

#include <math.h>

#include "Header.h"

PINIT(); 

void Setup()
{



}

void Update(char* cmd)
{




}
EOF

read -d '' Header_text <<"EOF"

void Update();
void Setup();

#define MAIN_LOOP_START()   				\
	do {						\
		ProtectOn();				\
	if(*(char*)cmd == 1)				\
            IsNeedQuit = 1; 				\
        ProtectOff();					\
    if(IsNeedQuit)					\
        break;				

#define MAIN_LOOP_END() }while(1);


//So funny code, don't beat me
#define PINIT() 					\
uint8_t IsNeedQuit = 0;					\
void Program(char* cmd)					\
{MAIN_LOOP_START() Update(); MAIN_LOOP_END()}		\
int main ( void )					\
{							\
	Setup();					\
	char Cmd = 0;					\
	InitThread();					\
	CreateThread(Program, 0, &Cmd);			\
	do 						\
	{ 						\
		if(Button_pressed(BUTTON_BACK))		\
			break;				\
	}while(1);					\
	Cmd = 1;					\
	JoinThread(0);					\
	printf("Program has stopped\n");		\
	Motor_stop_all();				\
    Lcd_clear();					\
    Lcd_release();					\
	return( 0 );					\
}												

void Program(char* cmd);


EOF

read -d '' makefile_text <<"EOF"
# 
# Configurable area
# 

# This must be set to robots ip
ip_robot_address = __ip_address
execute_name = __project_name

objects = $(patsubst %.c,%.o,$(wildcard *.c))

cross_compiler_directory = /usr/arm-linux-gnueabi
ev3cleti_lib_directory = $(HOME)/ev3cleti
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
EOF

# ------------------------------------------------------------------------------------------------

echo -n "Enter project directory [default: $HOME]:"

read project_directory

if [[ -z $project_directory ]]; then
	# Empty input
	project_directory=$HOME
else
	if [[ ! $project_directory == /* ]]; then
		# Relative path
		project_directory="$HOME/$project_directory"
	fi
fi
echo "Project directory: $project_directory"

echo -n "Enter project name [default: ev3_application]:"

read project_name

if [[ -z $project_name ]]; then
	project_name=ev3_application
fi
echo "Project name: $project_name"

mkdir -p $project_directory/$project_name

if [[ ! -d $project_directory/$project_name ]]; then
	echo 'Unable to create project directory'
	exit 1
fi

echo -n "Enter robot ip [default: 0.0.0.0]:"

read robot_ip

if [[ -z $robot_ip ]]; then
	robot_ip="0.0.0.0"
fi
echo "Robot ip: $robot_ip"

echo "$main_text" > $project_directory/$project_name/main.c
makefile_text=${makefile_text/__ip_address/$robot_ip}
makefile_text=${makefile_text/__project_name/$project_name}
echo "$Header_text" > $project_directory/$project_name/Header.h
echo "$makefile_text" > $project_directory/$project_name/Makefile
