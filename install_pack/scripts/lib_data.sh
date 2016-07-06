#!/bin/bash

DATA_DIR=ev3c-lib

# install lib/ and include/ dirs
sudo make install -C $DATA_DIR

# install qt project template
# EV3_TEMPLATE=ev3_proj_template
# QT_DIR=$HOME/Qt

# cp -r $EV3_TEMPLATE $QT_DIR/Tools/QtCreator/share/qtcreator/templates/wizards/projects
