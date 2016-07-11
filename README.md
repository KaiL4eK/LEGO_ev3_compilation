# **EV3 Compilation tools** #

## Operation System ##

### Now ev3dev is installed on SD card ###
http://www.ev3dev.org

## Compilers ##

### Linaro toolchain for Linux (cross compilation): ###
http://www.cnx-software.com/2011/03/28/installing-linaro-arm-cross-toolchain-on-ubuntu/

### Packages for Linux (cross compilation): ###
gcc-arm-linux-gnueabi g++-arm-linux-gnueabi(for c++ libs)

## Libraries ##

### With this compiler tested next libraries: ###
EV3c: https://github.com/theZiz/ev3c  
ev3dev-c: https://github.com/in4lio/ev3dev-c  
no coroutine.h file  
ev3dev-lang-cpp: https://github.com/ddemidov/ev3dev-lang-cpp    
CFLAGS += -std=gnu++0x  
CFLAGS (for library) += -D_GLIBCXX_USE_NANOSLEEP  
+ export CC and CXX paths before cmake

## Other data ##
https://www.dropbox.com/sh/3q4xj48iel743ug/AABwnclCVfoXYJmuWXDcKfgma?dl=0

## Library functions ##
In repository they lay in  
install_pack/ev3cleti_library/public  

In installed system  
/usr/local/ev3cleti/include