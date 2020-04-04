#! /bin/bash

function logcat()
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
    local logcat_file=logcat_${cur_time}.logcat
    (adb logcat -v time |tee ${logcat_file})
}


