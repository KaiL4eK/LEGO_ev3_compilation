#!/bin/bash

# install sublime text 3 - text editor
SUBLIME_PACKAGE=sublime-text_build-3114_amd64.deb

sudo dpkg -i $SUBLIME_PACKAGE

# install qt creator - IDE
QT_INSTALLER=qt-unified-linux-x64-online.run

chmod +x $QT_INSTALLER
./$QT_INSTALLER