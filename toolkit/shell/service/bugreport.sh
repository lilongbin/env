#! /bin/bash

function bugreport()
{
    echo "adb wait-for-device"
    adb wait-for-device
    adb devices
    if [ $? != 0 ] ;then
    	echo "ERROR: adb devices excute error"
    	exit
    fi
    sleep 1
    # adb root
    # adb remount
    
    local cur_time=$(date +%Y%m%d_%H%M%S)
    local bugreport_file=bugreport_${cur_time}.bugreport
    (adb bugreport |tee ${bugreport_file})
}

