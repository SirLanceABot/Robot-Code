#!/bin/sh

# Redirect stdout and stderr to a file
exec > /mnt/usb/Vision.out
exec 2>&1

cd /home/pi
python Vision.py
