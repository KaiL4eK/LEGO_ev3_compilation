#!/bin/bash

# if [ 1 != 1 ]; then #skip this block
# install cross-compiler
sudo apt-get update && \
sudo apt-get install -y gcc-arm-linux-gnueabi g++-arm-linux-gnueabi ssh-askpass-gnome ssh-askpass
#cmake libcanberra-gtk-module gtk2-engines-pixbuf

# install sublime text 3 - text editor
SUBLIME_PACKAGE=sublime-text_build-3114_amd64.deb

sudo dpkg -i $SUBLIME_PACKAGE
cp EV3.sublime-build ~/.config/sublime-text-3/Packages/User

LIBRARY_DIR=ev3cleti_library

make distclean -C $LIBRARY_DIR
make install -C $LIBRARY_DIR
# fi
