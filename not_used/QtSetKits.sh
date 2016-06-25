#!/bin/bash

SDKTOOL=../../Qt/Tools/QtCreator/libexec/qtcreator/sdktool

echo "Add compiler"

$SDKTOOL rmTC \
		--id "ProjectExplorer.ToolChain.Gcc:arm-linaro"

$SDKTOOL addTC \
		--id "ProjectExplorer.ToolChain.Gcc:arm-linaro" \
		--name "EV3 Compiler" \
		--path "/usr/bin/arm-linux-gnueabi-gcc" \
		--abi "arm-linux-generic-elf-32bit" \
		--supportedAbis "arm-linux-generic-elf-32bit"

echo "Add device"

$SDKTOOL rmDev \
		--id "ID_EV3_Device"

$SDKTOOL addDev \
		--id "ID_EV3_Device" \
		--name "EV3 Device" \
		--type 0 \
		--authentication 3 \
		--host "10.42.0.3" \
		--sshPort 22 \
		--freePorts "10000-10100" \
		--timeout 10 \
		--uname "robot" \
		--password "maker" \
		--osType "GenericLinuxOsType"

echo "Add kit"

$SDKTOOL rmKit \
		--id "ev3-arm-kit"

$SDKTOOL addKit \
	    --id "ev3-arm-kit" \
	    --name "EV3 Kit" \
	    --device "ID_EV3_Device" \
	    --devicetype "GenericLinuxOsType" \
	    --toolchain "ProjectExplorer.ToolChain.Gcc:arm-linaro" 