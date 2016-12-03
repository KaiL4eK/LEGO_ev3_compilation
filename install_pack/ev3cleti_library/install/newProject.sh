#!/bin/bash

read -d '' main_text <<"EOF"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <ev3_core.h>

int code ( void )
{
	// Here lay code =)

	return( 0 );
}
EOF

read -d '' makefile_text <<"EOF"
# 
# Configurable area
# 

# This must be set to robots ip
ip_robot_address = __ip_address
execute_name = __project_name

ev3cleti_lib_directory = $(HOME)/ev3cleti
include $(ev3cleti_lib_directory)/ev3cleti.mk
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

echo "$main_text" > $project_directory/$project_name/code.c
makefile_text=${makefile_text/__ip_address/$robot_ip}
makefile_text=${makefile_text/__project_name/$project_name}
echo "$makefile_text" > $project_directory/$project_name/Makefile
