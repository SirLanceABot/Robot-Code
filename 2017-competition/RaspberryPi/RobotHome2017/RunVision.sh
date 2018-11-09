#!/bin/sh

# If the destination directory is writeable, then redirect
# stdout and stderr to a file in that directory

if [ -w /mnt/usb ]
then
    exec > /mnt/usb/Vision.out
    exec 2>&1
fi

cd /home/pi
python Vision.py
