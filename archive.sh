#!/bin/bash

ver=$(cat version)
num=$(echo $ver | cut -f2 -d".")

echo $num
echo "Archiving $ver" 
tar -czvf ev3_pack-v$ver.tar.gz install_pack

(( num = num + 1 )) 
echo "0.$num" > version
