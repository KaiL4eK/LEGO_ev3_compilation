#!/bin/bash

./Qt/Tools/QtCreator/libexec/qtcreator/sdktool addTC \
		--id "ProjectExplorer.ToolChain.Gcc:arm-linaro" \
		--name "EV3-arm-gcc" \
		--path /usr/bin/arm-linux-gnueabi-gcc \
		--abi ar-linux-generic-elf-32bit \
		--supportedAbis ar-linux-generic-elf-32bit

./Qt/Tools/QtCreator/libexec/qtcreator/sdktool addDev \
		--id "id.ev3-device" \
		--name "EV3-dev" \
		--type "3" \
		--host "robot@10.42.0.3" \
		--password maker

./Qt/Tools/QtCreator/libexec/qtcreator/sdktool rmKit \
		--id "ev3-arm-kit"

./Qt/Tools/QtCreator/libexec/qtcreator/sdktool addKit \
	    --id "ev3-arm-kit" \
	    --name "EV3 Kit" \
	    --debuggerengine 1 \
	    --debugger /tmp/gdb-test \
	    --devicetype "General Linux" \
	    --toolchain "ProjectExplorer.ToolChain.Gcc:arm-linaro" 