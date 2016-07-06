#!/bin/bash

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

cp ev3_template_subl/main.c $project_directory/$project_name
sed "s/__project_name/$project_name/g; s/__ip_address/$robot_ip/g" "ev3_template_subl/Makefile" > "$project_directory/$project_name/Makefile"
