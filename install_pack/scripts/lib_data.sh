#!/bin/bash

LIB_DIR_EV3C=ev3c-lib
LIB_DIR_EV3CLETI=ev3cleti_library

# install lib/ and include/ dirs
make -C $LIB_DIR_EV3C
sudo make install -C $LIB_DIR_EV3C

make -C $LIB_DIR_EV3CLETI
sudo make install -C $LIB_DIR_EV3CLETI

# install qt project template
# EV3_TEMPLATE=ev3_proj_template
# QT_DIR=$HOME/Qt

# cp -r $EV3_TEMPLATE $QT_DIR/Tools/QtCreator/share/qtcreator/templates/wizards/projects
