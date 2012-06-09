#!/bin/bash

#This script determines if the port from the config file is the port the test device is connected to


CONFIGPORT=$(sed -n 1p config)
stty -F $CONFIGPORT  cs8 57600  ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts 
TOP=$(head -1 $CONFIGPORT)
PORTTEST=$(head -1 ./scripts/porttest )
if [ "$TOP" == "$PORTTEST" ]; then
echo This Port is correct 
else
echo This Port is incorrect
fi
