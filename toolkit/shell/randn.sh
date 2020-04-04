#! /bin/bash
# 
####################################################
# Author      : longbin
# Created date: 2018-05-06 20:20:57
####################################################

function randstr()
{
	local lenth=$1
	local rstr
	if [ "" = "${lenth}" ] ;then
		lenth=16
	fi
	# rstr=$(head -n 10 /dev/urandom | md5sum | awk '{print $1}')
	rstr=$(strings /dev/urandom | grep -o '[[:alnum:]]' | head -${lenth} | tr -d '\n')
	echo "${rstr}"
}

function randint()
{
	local rint
	rint=${RANDOM}${RANDOM}
	echo "${rint}"
}

# randstr
# randint

