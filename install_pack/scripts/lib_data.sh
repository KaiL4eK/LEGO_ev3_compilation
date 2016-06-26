#!/bin/bash

INSTALL_DATA_DIR=/usr/local
DATA_DIR=ev3_arm

# install lib/ and include/ dirs
sudo cp -r $DATA_DIR $INSTALL_DATA_DIR

# install qt project template
EV3_TEMPLATE=ev3_proj_template
QT_DIR=$HOME/Qt

cp -r $EV3_TEMPLATE $QT_DIR/Tools/QtCreator/share/qtcreator/templates/wizards/projects
