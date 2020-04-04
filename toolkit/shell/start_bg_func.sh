#! /bin/bash

####################################################
# Author      : longbin
# Created date: 2018-03-08 20:40:19
####################################################

function pbar()
{
    local flag_str=('-' '\\' '/')
    local flag_char='#'
    local bg_str=${flag_char}
    local bg_len=0
    local char_num=30
    local time_s_start=$(date +%s)
    local time_s_cur=$(date +%s)
    local time_s_used=0
    [ "${char_num}" -gt 60 ] && char_num=60

    trap 'echo; exit 0' HUP
    for ((;;))
    do
        echo -e '\r\c'
        echo -n "${time_s_used}"
        sleep 0.100000
        for ((i=1;i<=${char_num}*3;i++))
        do
            bg_len=$((i/3))
            time_s_cur=$(date +%s)
            time_s_used=$((time_s_cur - time_s_start))
            bg_string=""
            for ((j=0;j<=${bg_len};j++))
            do
                bg_string=${bg_string}${flag_char}
            done
            bg_string="${bg_string} ${flag_str[i%3]} "
            echo -e "\r[${time_s_used}s]${bg_string}\c"
            sleep 0.100000
        done
        echo -e '\r\c'
        echo -n "[${time_s_used}s]      "
        for ((i=0;i<=${char_num};i++))
        do
            echo -n ' '
        done
    done
}

function start_bg_func()
{
	if [ "-h" == "${1}" -o "--help" == "${1}" ] ;then
		echo "Usage:\n\tstart_bg_func task"
		return
	fi
	local task=$*
	if [ "" == "${task}" ] ;then
		echo "Usage:\n\tstart_bg_func task"
		return
	fi
    #start bg_func and register signal func
    pbar &
    local bg_pid=$!
    # echo $bg_pid

    # interrupt bg_func
    trap 'kill ${bg_pid} >/dev/null; echo; exit' INT

    # do something begin
	${task}
    # sleep 10
    # do something end

    # terminate bg_func
    (kill -HUP ${bg_pid} &>/dev/null; sleep 0.5)
    sleep 1
}

# start_bg_func ${task}
# start_bg_func sleep 5


