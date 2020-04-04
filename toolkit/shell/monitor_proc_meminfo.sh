#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2020-03-13 15:51:44
####################################################

PROCESS='android.hardware.gnss@1.0-service.aptiv'
PROCESS='vendor.aptiv.tbox@1.0-service'
LOG="${PROCESS}-memlog.txt"

function get_proc_PID()
{
    #ps -ef | grep ${PROCESS} | grep -v 'grep'
    PID=$(ps -ef | grep ${PROCESS} | grep -v 'grep' | head -1 | awk '{print $2;}')
    if [ "${PID}" = "" ] ;then
        echo "get PID failed;"
        return;
    fi
}

function get_proc_meminfo()
{
    local args=$*
    if [ $# -ge 1 ] ;then
        if [ "${args}" = "-h" -o "${args}" = "--help" ] ;then
            echo "sh script [proc-name]"
            return;
        fi
        PROCESS=${args}
    fi

    echo "get pid for process: ${PROCESS}"
    get_proc_PID
    if [ "${PID}" = "" ] ;then
        echo "get PID failed;"
        return;
    fi
    if [ -f "$LOG" ];then 
        rm "$LOG"
    fi
    touch ${LOG}
    echo "PID os ${PROCESS} is ${PID}" |tee -a "${LOG}"

    while [ "$PID" != "" ]    
    do
        mtime=$(date +%Y%m%d-%H:%M:%S)
        meminfo=$(cat /proc/${PID}/status | grep RSS)
        echo "#${mtime} #${PID} #${meminfo}" | tee -a "${LOG}"
        sleep 5
        get_proc_PID
    done
}

get_proc_meminfo $*

