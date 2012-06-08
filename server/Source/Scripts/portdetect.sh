#!/bin/bash
#This script detects the Arduino and generates config file

echo "Remove Arduino USB then press any key"
read -n 1 -s
find /dev/ -name "tty*" > temp
echo "Insert USB then press any key"
read -n 1 -s
echo "Device location:"
find /dev/ -name "tty*" > temp2
grep -Fxvf temp temp2
grep -Fxvf temp temp2 > config
rm temp
rm temp2

